#pragma once
#include "globalOpenGL.h"
#include "Shader/cShaderManager.h"
#include <sstream>

const int NUM_BONES = 30;
struct BoneShader {
    GLuint BoneMatrices[NUM_BONES];
    GLuint BoneRotationMatrices[NUM_BONES];
};

void setupBoneShaderLocation(GLuint shaderID);