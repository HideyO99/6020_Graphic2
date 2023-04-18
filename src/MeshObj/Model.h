#pragma once
#include "..\globalOpenGL.h"
#include <glm/glm.hpp>
#include <vector>
#include "vertex_type.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ASSIMP_LOAD_FLAGS	(aiProcess_Triangulate | \
							 aiProcess_GenSmoothNormals | \
							 aiProcess_PopulateArmatureData | \
							 aiProcess_FixInfacingNormals | \
							 aiProcess_LimitBoneWeights)

class Model
{
public:
	Model();
	Model(const char* filepath);
	~Model();

	GLuint VBO;
	GLuint VertBuffID;
	GLuint IndBuffID;

	std::vector<glm::vec3> Vertices;
	std::vector<int> Triangles;

	unsigned int NumTriangles;

private:
	Assimp::Importer m_AssimpImporter;
};

