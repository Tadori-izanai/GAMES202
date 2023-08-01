attribute vec3 aVertexPosition;
attribute vec3 aNormalPosition;
attribute vec2 aTextureCoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uMoveWithCamera;

// begin edit
uniform mat3 uPrecomputeLR;
uniform mat3 uPrecomputeLG;
uniform mat3 uPrecomputeLB;
attribute mat3 aPrecomputeLT;
varying highp vec3 vColor;
//
varying highp mat3 vPrecomputeLT;

varying highp vec2 vTextureCoord;
varying highp vec3 vFragPos;
varying highp vec3 vNormal;

float innerProd(mat3 A, mat3 B) {
    float s = 0.0;
    for (int i = 0; i < 3; i += 1) {
        s += dot(A[i], B[i]);
    }
    return s;
}

void main() {
    vNormal = aNormalPosition;
    vTextureCoord = aTextureCoord;
    mat4 viewMatrix = uViewMatrix;
    viewMatrix = mat4(mat3(viewMatrix));
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aVertexPosition, 1.0);
    // gl_Position = uProjectionMatrix * viewMatrix * uModelMatrix * uMoveWithCamera * vec4(aVertexPosition, 1.0);

    vFragPos = aVertexPosition;//gl_Position.xyz;

    vPrecomputeLT = aPrecomputeLT;
    // begin edit
    vColor[0] = innerProd(aPrecomputeLT, uPrecomputeLR);
    vColor[1] = innerProd(aPrecomputeLT, uPrecomputeLG);
    vColor[2] = innerProd(aPrecomputeLT, uPrecomputeLB);
    //
}
