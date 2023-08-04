class WebGLRenderer {
    meshes = [];
    shadowMeshes = [];
    lights = [];

    constructor(gl, camera) {
        this.gl = gl;
        this.camera = camera;
    }

    addLight(light) {
        this.lights.push({
            entity: light,
            meshRender: new MeshRender(this.gl, light.mesh, light.mat)
        });
    }
    addMeshRender(mesh) { this.meshes.push(mesh); }
    addShadowMeshRender(mesh) { this.shadowMeshes.push(mesh); }

    render() {
        const gl = this.gl;

        gl.clearColor(0.0, 0.0, 0.0, 1.0); // Clear to black, fully opaque
        gl.clearDepth(1.0); // Clear everything
        gl.enable(gl.DEPTH_TEST); // Enable depth testing
        gl.depthFunc(gl.LEQUAL); // Near things obscure far things

        console.assert(this.lights.length != 0, "No light");
        console.assert(this.lights.length == 1, "Multiple lights");

        const timer = Date.now() * 0.0001;

        for (let l = 0; l < this.lights.length; l++) {
            // Draw light
            this.lights[l].meshRender.mesh.transform.translate = this.lights[l].entity.lightPos;
            this.lights[l].meshRender.draw(this.camera);

            // Shadow pass
            if (this.lights[l].entity.hasShadowMap == true) {
                for (let i = 0; i < this.shadowMeshes.length; i++) {
                    this.shadowMeshes[i].draw(this.camera);
                }
            }

            // Camera pass
            for (let i = 0; i < this.meshes.length; i++) {
                this.gl.useProgram(this.meshes[i].shader.program.glShaderProgram);
                this.gl.uniform3fv(this.meshes[i].shader.program.uniforms.uLightPos, this.lights[l].entity.lightPos);

                for (let k in this.meshes[i].material.uniforms) {

                    let cameraModelMatrix = mat4.create();
                    mat4.fromRotation(cameraModelMatrix, timer * 5, [0, 1, 0]);         // todo(bonus): uncomment it

                    if (k == 'uMoveWithCamera') { // The rotation of the skybox
                        gl.uniformMatrix4fv(
                            this.meshes[i].shader.program.uniforms[k],
                            false,
                            cameraModelMatrix);
                    }

                    // Bonus - Fast Spherical Harmonic Rotation
                    let precomputeL_RGBMat3 = getRotationPrecomputeL(precomputeL[guiParams.envmapId], cameraModelMatrix);
                    // let precomputeL_RGBMat3 = getMat3ValueFromRGB(precomputeL[guiParams.envmapId])

                    console.log(precomputeL_RGBMat3[1]);
                    if (k == 'uPrecomputeLR') {
                        gl.uniformMatrix3fv(this.meshes[i].shader.program.uniforms[k], false, precomputeL_RGBMat3[0]);
                    } else if (k == 'uPrecomputeLG') {
                        gl.uniformMatrix3fv(this.meshes[i].shader.program.uniforms[k], false, precomputeL_RGBMat3[1]);
                    } else if (k == 'uPrecomputeLB') {
                        gl.uniformMatrix3fv(this.meshes[i].shader.program.uniforms[k], false, precomputeL_RGBMat3[2]);
                    }
                }

                this.meshes[i].draw(this.camera);
            }
        }

    }
}