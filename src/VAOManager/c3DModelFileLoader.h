#pragma once
#include <map>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "cModelDrawInfo.h"
#include "../MeshObj/cMeshObj.h"


#define MODEL_LOAD_BUFFER 10000
#define ASSIMP_LOAD_FLAGS	(aiProcess_Triangulate | \
							 aiProcess_GenSmoothNormals | \
							 aiProcess_PopulateArmatureData | \
							 aiProcess_FixInfacingNormals | \
							 aiProcess_LimitBoneWeights)

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

	bool loadMesh(const aiMesh* mesh, cModelDrawInfo* modelDrawInfo, cMeshObj* meshObj);

	aiScene* m_AssimpScene;
	Assimp::Importer m_Importer;
};

