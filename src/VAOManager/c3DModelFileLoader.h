#pragma once
#include <map>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "cModelDrawInfo.h"
#include "../MeshObj/cMeshObj.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>


#define MODEL_LOAD_BUFFER 10000
#define ASSIMP_LOAD_FLAGS	(aiProcess_Triangulate | \
							 aiProcess_JoinIdenticalVertices)

class c3DModelFileLoader
{
public:
	struct sTrianglePLY
	{
		// The 3 vertex index values from the ply file
		unsigned int vertexIndices[3];
	};

	//bool loadFBXFileAsync(std::string filename, std::string meshName, cModelDrawInfo* modelDrawInfo, unsigned int shaderProgramID);
	//bool loadMesh(const aiMesh* mesh, cModelDrawInfo* modelDrawInfo, cMeshObj* meshObj);
	bool loadPLYFile(std::string filename, cModelDrawInfo* modelDrawInfo, std::string error);
	bool loadFBXFile(std::string filename, std::string meshName, cModelDrawInfo* modelDrawInfo, unsigned int shaderProgramID, std::vector<cModelDrawInfo>* p_vecMotoDrawInfo_ReadyToSendToGPU);
	void LoadAssimpAnimation(CharacterAnimationData& characterAnimation, const aiAnimation* animation, BoneHierarchy* boneHierarchy);
	bool loadMesh(const aiMesh* mesh, cModelDrawInfo* modelDrawInfo, cMeshObj* meshObj, CharacterAnimationData& animationData);

	aiScene* m_AssimpScene;
	Assimp::Importer m_Importer;
};

