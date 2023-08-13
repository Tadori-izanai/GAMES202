#include "denoiser.h"

Denoiser::Denoiser() : m_useTemportal(false) {}

void Denoiser::Reprojection(const FrameInfo &frameInfo) {
    int height = m_accColor.m_height;
    int width = m_accColor.m_width;
    Matrix4x4 preWorldToScreen =
        m_preFrameInfo.m_matrix[m_preFrameInfo.m_matrix.size() - 1];
    Matrix4x4 preWorldToCamera =
        m_preFrameInfo.m_matrix[m_preFrameInfo.m_matrix.size() - 2];
#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // TODO: Reproject
            m_valid(x, y) = false;
            m_misc(x, y) = Float3(0.f);

            int currId = frameInfo.m_id(x, y);
            if (currId == -1) {
                continue;
            }

            Matrix4x4 currObjectToWorld = frameInfo.m_matrix[currId];
            auto objectCoord = Inverse(currObjectToWorld)(frameInfo.m_position(x, y), Float3::EType::Point);
            Matrix4x4 preObjectToWorld = m_preFrameInfo.m_matrix[currId];
            auto preScreenCoord = preWorldToScreen(
                preObjectToWorld(objectCoord, Float3::EType::Point), Float3::EType::Point
            );

            int preX = preScreenCoord.x;
            int preY = preScreenCoord.y;
            if (preX < 0 || preX >= width || preY < 0 || preY >= height) {
                continue;
            }
            if ((int) m_preFrameInfo.m_id(preX, preY) != currId) {
                continue;
            }
            m_misc(x, y) = m_accColor(preX, preY);
            m_valid(x, y) = true;
        }
    }
    std::swap(m_misc, m_accColor);
}

void Denoiser::TemporalAccumulation(const Buffer2D<Float3> &curFilteredColor) {
    int height = m_accColor.m_height;
    int width = m_accColor.m_width;
    int kernelRadius = 3;
#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!m_valid(x, y)) {
                m_misc(x, y) = curFilteredColor(x, y);  // C_{i}
                continue;
            }

            // TODO: Temporal clamp
            Float3 color = m_accColor(x, y);            // C_{i - 1}
            float cnt = 0.0f;
            Float3 sumSamples(0.0f);
            Float3 sumSampleSquares(0.0f);
            for (int dx = -kernelRadius; dx <= kernelRadius; dx += 1) {
                for (int dy = -kernelRadius; dy <= kernelRadius; dy += 1) {
                    int xx = x + dx;
                    int yy = y + dy;
                    if (xx < 0 || xx >= width || yy < 0 || yy >= height) {
                        continue;
                    }
                    auto c = curFilteredColor(xx, yy);
                    sumSamples += c;
                    sumSampleSquares += c * c;
                    cnt += 1.0f;
                }
            }
            auto mu = sumSamples / cnt;
            auto sigma = SafeSqrt((sumSampleSquares - mu * cnt) / (cnt - 1.0f));
            color = clamp(color, mu - sigma * m_colorBoxK, mu + sigma * m_colorBoxK);

            // TODO: Exponential moving average
//            float alpha = 1.0f;
            float alpha = m_alpha;
            m_misc(x, y) = Lerp(color, curFilteredColor(x, y), alpha);
        }
    }
    std::swap(m_misc, m_accColor);
}

Buffer2D<Float3> Denoiser::Filter(const FrameInfo &frameInfo) {
    int height = frameInfo.m_beauty.m_height;
    int width = frameInfo.m_beauty.m_width;
    Buffer2D<Float3> filteredImage = CreateBuffer2D<Float3>(width, height);
    int kernelRadius = 16;
#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // TODO: Joint bilateral filter
            filteredImage(x, y) = frameInfo.m_beauty(x, y);

            float sumOfWeight = 0.0f;
            Float3 sumOfWeightedValues(0.0f);
            for (int dy = -kernelRadius; dy <= kernelRadius; dy += 1) {
                for (int dx = -kernelRadius; dx <= kernelRadius; dx += 1) {
                    int yy = y + dy;
                    int xx = x + dx;
                    if (xx < 0 || xx >= width || yy < 0 || yy >= height) {
                        continue;
                    }

                    auto distSquare = float(dy * dy + dx * dx);
                    auto diffCoord = frameInfo.m_position(xx, yy) - frameInfo.m_position(x, y);
                    auto diffColor = frameInfo.m_beauty(xx, yy) - frameInfo.m_beauty(x, y);
                    auto diffNormal = 0.f;
                    auto diffPlane = Dot(
//                        frameInfo.m_normal(xx, yy),
                        frameInfo.m_normal(x, y),
                        safeNormalize(frameInfo.m_position(x, y) - frameInfo.m_position(xx, yy))
                    );
                    if (Length(frameInfo.m_normal(xx, yy)) > 1e-5 && Length(frameInfo.m_normal(x, y)) > 1e-5) {
                        diffNormal = SafeAcos(Dot(frameInfo.m_normal(xx, yy), frameInfo.m_normal(x, y)));
                    }

                    float gaussian = -distSquare / (2.0f * m_sigmaCoord * m_sigmaCoord);
                    float coordinate = -Dot(diffCoord, diffCoord) / (2.0f * m_sigmaCoord * m_sigmaCoord);
                    float color = -Dot(diffColor, diffColor) / (2.0f * m_sigmaColor * m_sigmaColor);
                    float normal = -(diffNormal * diffNormal) / (2.0f * m_sigmaNormal * m_sigmaNormal);
                    float plane = -(diffPlane * diffPlane) / (2.0f * m_sigmaPlane * m_sigmaPlane);

//                    float weight = exp(gaussian + color + normal + plane);
                    float weight = exp(coordinate + color + normal + plane);
                    sumOfWeightedValues += frameInfo.m_beauty(xx, yy) * weight;
                    sumOfWeight += weight;
                }
            }
            filteredImage(x, y) = sumOfWeightedValues / sumOfWeight;
        }
    }
    return filteredImage;
}

Buffer2D<Float3> Denoiser::waveletFilter(const FrameInfo &frameInfo) {
    int height = frameInfo.m_beauty.m_height;
    int width = frameInfo.m_beauty.m_width;
    Buffer2D<Float3> filteredImage = CreateBuffer2D<Float3>(width, height);

    Buffer2D<Float3> preFilteredImage = CreateBuffer2D<Float3>(width, height);
    preFilteredImage.Copy(frameInfo.m_beauty);
    filteredImage.Copy(preFilteredImage);

    const int kernelRadius = 2;
    float mask1d[] = {1.0/16, 1.0/4, 3.0/8, 1.0/4, 1.0/16};
//    float mask1d[] = {1, 1, 1, 1, 1};
    float mask[5][5];
    for (int i = 0; i < 5; i += 1) {
        for (int j = 0; j < 5; j += 1) {
            mask[i][j] = mask1d[i] * mask1d[j];
        }
    }
    int gap = 1;
    int passes = 5;

#pragma omp parallel for
    while (passes--) {
        for (int y = 0; y < height; y += 1) {
            for (int x = 0; x < width; x += 1) {
                float sumOfWeight = 0.0f;
                Float3 sumOfWeightedValues(0.0f);

                for (int dy = -kernelRadius; dy <= kernelRadius; dy += 1) {
                    for (int dx = -kernelRadius; dx <= kernelRadius; dx += 1) {
                        int yy = y + dy * gap;
                        int xx = x + dx * gap;
                        if (xx < 0 || xx >= width || yy < 0 || yy >= height) {
                            continue;
                        }

                        auto distSquare = float((yy - y) * (yy - y) + (xx - x) * (xx - x));
                        auto diffCoord = frameInfo.m_position(xx, yy) - frameInfo.m_position(x, y);
                        auto diffColor = preFilteredImage(xx, yy) - preFilteredImage(x, y);
                        auto diffNormal = 0.f;
                        auto diffPlane = Dot(
//                            frameInfo.m_normal(xx, yy),
                            frameInfo.m_normal(x, y),
                            safeNormalize(frameInfo.m_position(x, y) - frameInfo.m_position(xx, yy))
                        );
                        if (Length(frameInfo.m_normal(xx, yy)) > 1e-5 && Length(frameInfo.m_normal(x, y)) > 1e-5) {
                            diffNormal = SafeAcos(Dot(frameInfo.m_normal(xx, yy), frameInfo.m_normal(x, y)));
                        }

                        float gaussian = -distSquare / (2.0f * m_sigmaCoord * m_sigmaCoord);
                        float coordinate = -Dot(diffCoord, diffCoord) / (2.0f * m_sigmaCoord * m_sigmaCoord);
                        float color = -Dot(diffColor, diffColor) / (2.0f * m_sigmaColor * m_sigmaColor);
                        float normal = -(diffNormal * diffNormal) / (2.0f * m_sigmaNormal * m_sigmaNormal);
                        float plane = -(diffPlane * diffPlane) / (2.0f * m_sigmaPlane * m_sigmaPlane);

                        float weight
//                            = exp(gaussian + color + normal + plane) * mask[dx + kernelRadius][dy + kernelRadius];
                            = exp(coordinate + color + normal + plane) * mask[dx + kernelRadius][dy + kernelRadius];
                        sumOfWeightedValues += preFilteredImage(xx, yy) * weight;
                        sumOfWeight += weight;
                    }
                }
                filteredImage(x, y) = sumOfWeightedValues / sumOfWeight;
            }
        }
        preFilteredImage.Copy(filteredImage);
        gap *= 2;
    }

    return filteredImage;
}

void Denoiser::Init(const FrameInfo &frameInfo, const Buffer2D<Float3> &filteredColor) {
    m_accColor.Copy(filteredColor);
    int height = m_accColor.m_height;
    int width = m_accColor.m_width;
    m_misc = CreateBuffer2D<Float3>(width, height);
    m_valid = CreateBuffer2D<bool>(width, height);
}

void Denoiser::Maintain(const FrameInfo &frameInfo) { m_preFrameInfo = frameInfo; }

Buffer2D<Float3> Denoiser::ProcessFrame(const FrameInfo &frameInfo) {
    // Filter current frame
    Buffer2D<Float3> filteredColor;
//    filteredColor = Filter(frameInfo);
    filteredColor = waveletFilter(frameInfo);

    // Reproject previous frame color to current
    if (m_useTemportal) {
        Reprojection(frameInfo);
        TemporalAccumulation(filteredColor);
    } else {
        Init(frameInfo, filteredColor);
    }

    // Maintain
    Maintain(frameInfo);
    if (!m_useTemportal) {
        m_useTemportal = true;
    }
    return m_accColor;
}
