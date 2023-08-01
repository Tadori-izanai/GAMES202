class PRTMaterial extends Material {
    constructor(vertexShader, fragmentShader) {
        super({
            'uPrecomputeLR': {type: 'precomputeL', value: null},
            'uPrecomputeLG': {type: 'precomputeL', value: null},
            'uPrecomputeLB': {type: 'precomputeL', value: null},
            'uMoveWithCamera': { type: 'updatedInRealTime', value: null }
        }, [
            'aPrecomputeLT'
        ], vertexShader, fragmentShader, null);
    }
}

async function buildPRTMaterial(vertexPath, fragmentPath) {
    let vertexShader = await getShaderString(vertexPath);
    let fragmentShader = await getShaderString(fragmentPath);

    return new PRTMaterial(vertexShader, fragmentShader);
}
