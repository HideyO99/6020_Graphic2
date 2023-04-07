#include "boneShader.h"

BoneShader gBoneShader;

void setupBoneShaderLocation(GLuint shaderID)
{
    for (int i = 0; i < NUM_BONES; i++)
    {
        std::stringstream ssBoneMatrices;
        ssBoneMatrices << "BoneMatrices[" << i << "]";
        gBoneShader.BoneMatrices[i] = glGetUniformLocation(shaderID, ssBoneMatrices.str().c_str());

        std::stringstream ssBoneRotationMatrices;
        ssBoneRotationMatrices << "BoneRotationMatrices[" << i << "]";
        gBoneShader.BoneRotationMatrices[i] = glGetUniformLocation(shaderID, ssBoneRotationMatrices.str().c_str());

    }
}