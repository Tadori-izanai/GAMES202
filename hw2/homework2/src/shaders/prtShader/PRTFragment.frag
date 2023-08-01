#ifdef GL_ES
precision mediump float;
#endif

// uniform samplerCube skybox;

varying highp vec2 vTextureCoord;
varying highp vec3 vFragPos;
varying highp vec3 vNormal;

varying highp vec3 vColor;

vec4 gammaCorrection(vec3 color) {
    vec4 converted;

    converted[3] = 1.0;    
    for (int i = 0; i < 3; i += 1) {
        if (color[i] <= 0.0031308) {
            converted[i] = 12.92 * color[i];
        } else {
            converted[i] = (1.0 + 0.055) * pow(color[i], 1.0/2.4) - 0.055;
        }
    }

    return converted;
}

void main() {
    gl_FragColor = gammaCorrection(vColor);
}
