#pragma once
#include "..\globalOpenGL.h"
#include "cModelDrawInfo.h"
#include "..\XML\cXML.h"
#include <string>
#include <map>
#include <fstream>
#include "../MeshObj/cMeshObj.h"
#include "../BoneHierarchy.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MODEL_LOAD_BUFFER 10000
#define ASSIMP_LOAD_FLAGS	(aiProcess_Triangulate | \
							 aiProcess_GenSmoothNormals | \
							 aiProcess_PopulateArmatureData | \
							 aiProcess_FixInfacingNormals | \
							 aiProcess_LimitBoneWeights)

class cVAOManager
{
public:
	cVAOManager();
	~cVAOManager();

	struct sTrianglePLY
	{
		// The 3 vertex index values from the ply file
		unsigned int vertexIndices[3];
	};

	bool loadModelToVAO(std::string filename, cModelDrawInfo* drawInfo, unsigned int shaderProgramID);
	bool loadModelList(std::string filename, unsigned int shaderProgramID);
	bool FindDrawInfo(std::string filename, cModelDrawInfo& drawInfo);
	//bool loadFBXFile(std::string filename, cModelDrawInfo* modelDrawInfo, unsigned int shaderProgramID);
	bool loadFBXFile(std::map<std::string, std::string>::iterator i_instanceToModel, std::string meshName, cModelDrawInfo* modelDrawInfo, unsigned int shaderProgramID);
	bool loadMesh(const aiMesh* mesh, cModelDrawInfo* modelDrawInfo, cMeshObj* meshObj);
	bool loadPLYFile(std::string filename, cModelDrawInfo& modelDrawInfo, std::string error);
	bool setInstanceObjScale(std::string meshObjName, float value);
	bool setInstanceObjWireframe(std::string meshObjName, bool value);
	bool setInstanceObjRGB(std::string meshObjName, glm::vec4 value);
	bool setInstanceObjVisible(std::string meshObjName, bool value);
	bool setInstanceObjLighting(std::string meshObjName, bool value);
	bool setInstanceObjSpecularPower(std::string meshObjName, glm::vec4 value);
	bool setInstanceObjBone(std::string meshObjName);

	bool setInstanceObjPosition(std::string meshObjName, glm::vec4 value);

	glm::vec4 getInstanceObjPosition(std::string meshObjName);

	cMeshObj* findMeshObjAddr(std::string meshObjName);

	bool setTexture(std::string meshObjName, std::string textureFile, int arrPos);

	bool setTextureRatio(std::string meshObjName, int arrPos, float ratio);

	bool bindingChild(std::string meshChildObjName, std::string meshParentObjName);

	bool setSkyBoxFlag(std::string meshObjName, bool flag);

	bool setIslandModelFlag(std::string meshObjName, bool flag);

	bool setUseRGBColorFlag(std::string meshObjName, bool flag);

	bool setDungeonTexture(std::string meshObjName, std::string textureFile);

	bool setTorchTexture(std::string meshObjName, std::string textureFile, std::string markTextureFile);

	std::map<std::string, cMeshObj*> mapInstanceNametoMeshObj;
	std::map<std::string, std::vector<cModelDrawInfo*>> mapModeltoMultiMesh;
	std::vector<cMeshObj*> pVecInstanceMeshObj;
	glm::vec3 cameraEyeFromXML;

private:
	std::map<std::string, cModelDrawInfo> mapModelNametoVAOID;
	aiScene* m_AssimpScene;
	Assimp::Importer m_Importer;
};

