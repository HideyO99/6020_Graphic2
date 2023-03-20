#include "cVAOManager.h"
#include "../Texture/cTextureManager.h"

extern cTextureManager* g_pTextureManager;

cVAOManager::cVAOManager()
{
}

cVAOManager::~cVAOManager()
{
}

bool cVAOManager::loadModelToVAO(std::string filename, cModelDrawInfo* drawInfo, unsigned int shaderProgramID)
{
	GLenum error;
	
	drawInfo->meshName = filename;

	drawInfo->CalculateExtents();

	glGenVertexArrays(1, &(drawInfo->VAO_ID));
	glBindVertexArray(drawInfo->VAO_ID);

	//vertices
	glGenBuffers(1, &(drawInfo->VertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo->VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones) * drawInfo->numberOfVertices, (GLvoid*)drawInfo->pVertices, GL_STATIC_DRAW);

	//indices
	glGenBuffers(1, &(drawInfo->IndexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo->IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * drawInfo->numberOfIndices, (GLvoid*)drawInfo->pIndices, GL_STATIC_DRAW);


	//in vec4 vColor;
	GLint vColor_location = glGetAttribLocation(shaderProgramID, "vColour");
	glEnableVertexAttribArray(vColor_location);
	error = glGetError();
	glVertexAttribPointer(vColor_location,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, r));		// Offset the member variable
	error = glGetError();

	//in vec4 vPosition;			
	GLint vPosition_location = glGetAttribLocation(shaderProgramID, "vPosition");
	glEnableVertexAttribArray(vPosition_location);
	glVertexAttribPointer(vPosition_location,
		4, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, x));		// Offset the member variable

	//in vec4 vNormal;			
	GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");
	error = glGetError();
	glEnableVertexAttribArray(vNormal_location);
	error = glGetError();
	glVertexAttribPointer(vNormal_location,
		4, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, nx));		// Offset the member variable
	error = glGetError();			

	//in vec4 vUVx2;			
	GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUVx2");
	error = glGetError();
	glEnableVertexAttribArray(vUVx2_location);
	error = glGetError();
	glVertexAttribPointer(vUVx2_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, u0));		// Offset the member variable
	error = glGetError();

	//in vec4 vTangent;			
	GLint vTangent_location = glGetAttribLocation(shaderProgramID, "vTangent");
	glEnableVertexAttribArray(vTangent_location);
	glVertexAttribPointer(vTangent_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, tx));		// Offset the member variable

	//in vec4 vBiNormal;		
	GLint vBiNormal_location = glGetAttribLocation(shaderProgramID, "vBiNormal");
	glEnableVertexAttribArray(vBiNormal_location);
	glVertexAttribPointer(vBiNormal_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, bx));		// Offset the member variable

	//in vec4 vBoneID;			
	GLint vBoneID_location = glGetAttribLocation(shaderProgramID, "vBoneID");
	glEnableVertexAttribArray(vBoneID_location);
	glVertexAttribPointer(vBoneID_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, vBoneID[0]));		// Offset the member variable

	//in vec4 vBoneWeight;		
	GLint vBoneWeight_location = glGetAttribLocation(shaderProgramID, "vBoneWeight");
	glEnableVertexAttribArray(vBoneWeight_location);
	glVertexAttribPointer(vBoneWeight_location,
		4, GL_FLOAT,
		GL_FALSE,
		sizeof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones),						// Stride	(number of bytes)
		(void*)offsetof(cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, vBoneWeight[0]));		// Offset the member variable

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vPosition_location);
	glDisableVertexAttribArray(vNormal_location);
	glDisableVertexAttribArray(vColor_location);
	glDisableVertexAttribArray(vUVx2_location);
	glDisableVertexAttribArray(vTangent_location);
	glDisableVertexAttribArray(vBiNormal_location);
	glDisableVertexAttribArray(vBoneID_location);
	glDisableVertexAttribArray(vBoneWeight_location);

	this->mapModelNametoVAOID[drawInfo->meshName] = *drawInfo;

	return true;
}

bool cVAOManager::loadModelList(std::string filename, unsigned int shaderProgramID)
{
	cXML modelListXML;
	bool result;

	result = modelListXML.loadModelFromXML(filename, mapInstanceNametoMeshObj, pVecInstanceMeshObj);
	if (!result)
	{
		return false;
	}

	std::map<std::string, std::string>::iterator i_mapModel;

	for (i_mapModel = modelListXML.mapModelNameAndPath.begin(); i_mapModel != modelListXML.mapModelNameAndPath.end(); i_mapModel++)
	{
		std::string fileType = i_mapModel->second.substr(i_mapModel->second.find('.')+1,std::string::npos);
		if (fileType == "ply")
		{
			cModelDrawInfo modelDrawInfo;
			//cMeshObj meshObj;
			std::string error = "";

			result = loadPLYFile(i_mapModel->second, modelDrawInfo, error);
			if (!result)
			{
				std::cout << "cannot load " << i_mapModel->first << std::endl;
				std::cout << "error " << error << std::endl;
				return false;
			}
			result = loadModelToVAO(i_mapModel->first, &modelDrawInfo, shaderProgramID);
			if (!result)
			{
				std::cout << "cannot load " << i_mapModel->first << std::endl;
				return false;
			}
		}
		if (fileType == "fbx")
		{
			cModelDrawInfo* modelDrawInfo = new cModelDrawInfo();
			//cMeshObj meshObj;
			
			//result = loadFBXFile(i_mapModel->second, modelDrawInfo, shaderProgramID);
			result = loadFBXFile(i_mapModel, modelDrawInfo, shaderProgramID);
		}
		//mapModelNametoMeshObj.emplace(i_mapModel->first, meshObj);
		std::cout << i_mapModel->first << " is loaded" << std::endl;
	}
	cameraEyeFromXML = modelListXML.cameraEyeFromXML;
	

	return true;
}

bool cVAOManager::FindDrawInfo(std::string filename, cModelDrawInfo& drawInfo)
{
	std::map<std::string, cModelDrawInfo>::iterator i_DrawInfo = mapModelNametoVAOID.find(filename);
	
	if (i_DrawInfo==mapModelNametoVAOID.end())
	{
		return false;
	}

	drawInfo = i_DrawInfo->second;

	return true;
}

//bool cVAOManager::loadFBXFile(std::string filename, cModelDrawInfo* modelDrawInfo, unsigned int shaderProgramID)
bool cVAOManager::loadFBXFile(std::map<std::string, std::string>::iterator i_mapModel, cModelDrawInfo* modelDrawInfo, unsigned int shaderProgramID)
{
	const aiScene* scene = m_Importer.ReadFile(i_mapModel->second, ASSIMP_LOAD_FLAGS);

	aiNode* node = scene->mRootNode;
	for (int i = 0; i < node->mNumChildren; i++)
	{
		aiNode* child = node->mChildren[i];

		//Find channel data from our node name:
		child->mName;
	}

	if (scene == 0 || !scene->HasMeshes())
	{
		return false;
	}

	//get texture
	std::string test;
	if (scene->HasMaterials())
	{
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			const aiMaterial* pMat = scene->mMaterials[i];
			if (pMat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString path;
				if (pMat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS)
				{
					std::string tmp = path.data;
					tmp = tmp.substr(tmp.find_last_of("/\\") + 1, std::string::npos);
					if (::g_pTextureManager->isNotExistTexture(tmp))
					{
						::g_pTextureManager->create2DTextureFromFreeImgLib(tmp);
					}
				}
			}
		}
	}


	std::vector<cModelDrawInfo*> vecModelDraw;

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		cModelDrawInfo* modelDrawInfo = new cModelDrawInfo();
		aiMesh* mesh = scene->mMeshes[i];
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		aiString texturePath;
		loadMesh(mesh, modelDrawInfo);
		if(i==0)
		{ 
			loadModelToVAO(i_mapModel->first, modelDrawInfo, shaderProgramID);
		}
		else
		{
			loadModelToVAO(mesh->mName.C_Str(), modelDrawInfo, shaderProgramID);
		}
		if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS)
		{
			std::string tmp = texturePath.data;
			modelDrawInfo->TextureFile = tmp.substr(tmp.find_last_of("/\\") + 1, std::string::npos);
		}
		vecModelDraw.push_back(modelDrawInfo);
	}
	mapModeltoMultiMesh.emplace(i_mapModel->first, vecModelDraw);


	return true;
}

bool cVAOManager::loadMesh(const aiMesh* mesh, cModelDrawInfo* modelDrawInfo)
{
	modelDrawInfo->numberOfVertices = mesh->mNumVertices;
	modelDrawInfo->numberOfIndices = mesh->mNumFaces * 3;
	modelDrawInfo->numberOfTriangles = mesh->mNumFaces;
	modelDrawInfo->pVertices = new cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones[modelDrawInfo->numberOfVertices];

	for (int i_vertices = 0; i_vertices < modelDrawInfo->numberOfVertices; i_vertices++)
	{
		modelDrawInfo->pVertices[i_vertices].x = mesh->mVertices[i_vertices].x;
		modelDrawInfo->pVertices[i_vertices].y = mesh->mVertices[i_vertices].y;
		modelDrawInfo->pVertices[i_vertices].z = mesh->mVertices[i_vertices].z;

		if (mesh->HasNormals())
		{
			modelDrawInfo->pVertices[i_vertices].nx = mesh->mNormals[i_vertices].x;
			modelDrawInfo->pVertices[i_vertices].ny = mesh->mNormals[i_vertices].y;
			modelDrawInfo->pVertices[i_vertices].nz = mesh->mNormals[i_vertices].z;
		}
		else
		{
			modelDrawInfo->pVertices[i_vertices].nx = 0;
			modelDrawInfo->pVertices[i_vertices].ny = 0;
			modelDrawInfo->pVertices[i_vertices].nz = 0;
		}

		if (mesh->HasVertexColors(i_vertices))
		{
			modelDrawInfo->pVertices[i_vertices].r = mesh->mColors[i_vertices]->r;
			modelDrawInfo->pVertices[i_vertices].g = mesh->mColors[i_vertices]->g;
			modelDrawInfo->pVertices[i_vertices].b = mesh->mColors[i_vertices]->b;
			modelDrawInfo->pVertices[i_vertices].a = mesh->mColors[i_vertices]->a;
		}
		else
		{
			modelDrawInfo->pVertices[i_vertices].r = 0;
			modelDrawInfo->pVertices[i_vertices].g = 0;
			modelDrawInfo->pVertices[i_vertices].b = 0;
			modelDrawInfo->pVertices[i_vertices].a = 1;
		}

		if (mesh->HasTextureCoords(0))
		{
			modelDrawInfo->pVertices[i_vertices].u0 = mesh->mTextureCoords[0][i_vertices].x;
			modelDrawInfo->pVertices[i_vertices].v0 = mesh->mTextureCoords[0][i_vertices].y;
		}
		else
		{
			modelDrawInfo->pVertices[i_vertices].u0 = 0;
			modelDrawInfo->pVertices[i_vertices].v0 = 0;
		}

		if (mesh->HasTextureCoords(1))
		{
			modelDrawInfo->pVertices[i_vertices].u1 = mesh->mTextureCoords[1][i_vertices].x;
			modelDrawInfo->pVertices[i_vertices].v1 = mesh->mTextureCoords[1][i_vertices].y;
		}
		else
		{
			modelDrawInfo->pVertices[i_vertices].u1 = 0;
			modelDrawInfo->pVertices[i_vertices].v1 = 0;
		}

		modelDrawInfo->pVertices[i_vertices].vBoneWeight[0] = 0;
		modelDrawInfo->pVertices[i_vertices].vBoneWeight[1] = 0;
		modelDrawInfo->pVertices[i_vertices].vBoneWeight[2] = 0;
		modelDrawInfo->pVertices[i_vertices].vBoneWeight[3] = 0;
		modelDrawInfo->pVertices[i_vertices].vBoneID[0] = 0;
		modelDrawInfo->pVertices[i_vertices].vBoneID[1] = 0;
		modelDrawInfo->pVertices[i_vertices].vBoneID[2] = 0;
		modelDrawInfo->pVertices[i_vertices].vBoneID[3] = 0;
	}

	if (mesh->HasBones())
	{
		unsigned int numBone = mesh->mNumBones;
		for (int i_bones = 0; i_bones < numBone; i_bones++)
		{
			const aiBone* bone = mesh->mBones[i_bones];
			for (int i = 0; i < bone->mNumWeights; i++)
			{
				//const aiVertexWeight vw = bone->mWeights[i];
				//modelDrawInfo->pVertices[vw.mVertexId];
			}
			//modelDrawInfo->pVertices[i_vertices].vBoneID[0] = mesh->mBones[i_bones]->mWeights[]
			//modelDrawInfo->pVertices[i_vertices].vBoneWeight[0] = mesh->mBones[i_vertices]->mWeights[0].mWeight;
		}

	}

	unsigned int vertex_element_index_index = 0;
	modelDrawInfo->pIndices = new unsigned int[modelDrawInfo->numberOfIndices];
	for (unsigned int i = 0; i != modelDrawInfo->numberOfTriangles; i++)
	{
		modelDrawInfo->pIndices[vertex_element_index_index + 0] = mesh->mFaces[i].mIndices[0];
		modelDrawInfo->pIndices[vertex_element_index_index + 1] = mesh->mFaces[i].mIndices[1];
		modelDrawInfo->pIndices[vertex_element_index_index + 2] = mesh->mFaces[i].mIndices[2];
		vertex_element_index_index += 3;
	}
	return true;
}

bool cVAOManager::loadPLYFile(std::string filename, cModelDrawInfo& modelDrawInfo, std::string error)
{
	sTrianglePLY* pTheModelTriangleArray = NULL;

	std::ifstream modelFile(filename);
	if (!modelFile.is_open())
	{
		error = " cannot open " + filename;
		return false;
	}

	char buffer[MODEL_LOAD_BUFFER];
	modelFile.getline(buffer, MODEL_LOAD_BUFFER);
	std::string nextToken;

	while (modelFile >> nextToken)
	{
		if (nextToken == "vertex")
		{
			break;
		}
	}
	modelFile >> modelDrawInfo.numberOfVertices;

	while (modelFile >> nextToken)
	{
		if (nextToken == "face")
		{
			break;
		}
	}
	modelFile >> modelDrawInfo.numberOfTriangles;

	while (modelFile >> nextToken)
	{
		if (nextToken == "end_header")
		{
			break;
		}
	}

	modelDrawInfo.pVertices = new cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones[modelDrawInfo.numberOfVertices];

	for (unsigned int i = 0; i != modelDrawInfo.numberOfVertices; i++)
	{

		modelFile >> modelDrawInfo.pVertices[i].x;
		modelFile >> modelDrawInfo.pVertices[i].y;
		modelFile >> modelDrawInfo.pVertices[i].z;

		modelFile >> modelDrawInfo.pVertices[i].nx;
		modelFile >> modelDrawInfo.pVertices[i].ny;
		modelFile >> modelDrawInfo.pVertices[i].nz;

		modelFile >> modelDrawInfo.pVertices[i].r;
		modelFile >> modelDrawInfo.pVertices[i].g;
		modelFile >> modelDrawInfo.pVertices[i].b;
		modelFile >> modelDrawInfo.pVertices[i].a;

		modelFile >> modelDrawInfo.pVertices[i].u0;
		modelFile >> modelDrawInfo.pVertices[i].v0;

	}

	pTheModelTriangleArray = new sTrianglePLY[modelDrawInfo.numberOfTriangles];
	for (unsigned int count = 0; count != modelDrawInfo.numberOfTriangles; count++)
	{
		unsigned int discard = 0;
		modelFile >> discard;

		modelFile >> pTheModelTriangleArray[count].vertexIndices[0];
		modelFile >> pTheModelTriangleArray[count].vertexIndices[1];
		modelFile >> pTheModelTriangleArray[count].vertexIndices[2];
	}

	modelFile.close();

	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;
	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int vertex_element_index_index = 0;

	for (unsigned int i = 0; i != modelDrawInfo.numberOfTriangles; i++)
	{
		modelDrawInfo.pIndices[vertex_element_index_index + 0] = pTheModelTriangleArray[i].vertexIndices[0];
		modelDrawInfo.pIndices[vertex_element_index_index + 1] = pTheModelTriangleArray[i].vertexIndices[1];
		modelDrawInfo.pIndices[vertex_element_index_index + 2] = pTheModelTriangleArray[i].vertexIndices[2];
		vertex_element_index_index += 3;
	}
	return true;
}

bool cVAOManager::setInstanceObjScale(std::string meshObjName, float value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh== mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;

	pCurrentMeshObject->scale = value;

	return true;
}

bool cVAOManager::setInstanceObjWireframe(std::string meshObjName, bool value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;

	pCurrentMeshObject->isWireframe = value;

	return true;
}

bool cVAOManager::setInstanceObjRGB(std::string meshObjName, glm::vec4 value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;

	pCurrentMeshObject->color_RGBA = value;

	return true;
}

bool cVAOManager::setInstanceObjVisible(std::string meshObjName, bool value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;

	pCurrentMeshObject->isVisible = value;

	return true;
}

bool cVAOManager::setInstanceObjLighting(std::string meshObjName, bool value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->bDoNotLight = !value;
	return true;
}

bool cVAOManager::setInstanceObjSpecularPower(std::string meshObjName, glm::vec4 value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->specular_colour_and_power = value;

	return true;
}

bool cVAOManager::setInstanceObjPosition(std::string meshObjName, glm::vec4 value)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->position = value;

	return true;
}
glm::vec4 cVAOManager::getInstanceObjPosition(std::string meshObjName)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return glm::vec4(0);
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	
	return glm::vec4(pCurrentMeshObject->position,1.f);

}

cMeshObj* cVAOManager::findMeshObjAddr(std::string meshObjName)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return nullptr;
	}
	return itCurrentMesh->second;
}

bool cVAOManager::setTexture(std::string meshObjName, std::string textureFile, int arrPos)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->bUse_RGBA_colour = false;
	pCurrentMeshObject->textures[arrPos] = textureFile;
	pCurrentMeshObject->textureRatios[arrPos] = 1.0f;

	return true;
}

bool cVAOManager::setTextureRatio(std::string meshObjName, int arrPos, float ratio)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->textureRatios[arrPos] = ratio;

	return true;
}

bool cVAOManager::bindingChild(std::string meshChildObjName, std::string meshParentObjName)
{
	std::map<std::string, cMeshObj* >::iterator itParentMesh = mapInstanceNametoMeshObj.find(meshParentObjName);
	std::map<std::string, cMeshObj* >::iterator itChildMesh = mapInstanceNametoMeshObj.find(meshChildObjName);
	if (itParentMesh == mapInstanceNametoMeshObj.end() || itChildMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}

	cMeshObj* pParentMeshObject = itParentMesh->second;
	cMeshObj* pChildMeshObject = itChildMesh->second;
	pParentMeshObject->vecChildMesh.push_back(pChildMeshObject);
	mapInstanceNametoMeshObj.erase(meshChildObjName);

	return true;
}

bool cVAOManager::setSkyBoxFlag(std::string meshObjName, bool flag)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->isSkybox = flag; 

	return true;
}

bool cVAOManager::setIslandModelFlag(std::string meshObjName, bool flag)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->isIslandModel = flag;

	return true;
}

bool cVAOManager::setUseRGBColorFlag(std::string meshObjName, bool flag)
{
	std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	{
		return false;
	}
	cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	pCurrentMeshObject->bUse_RGBA_colour = flag;

	return true;
}

bool cVAOManager::setDungeonTexture(std::string meshObjName, std::string textureFile)
{
	//std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.find(meshObjName);
	//if (itCurrentMesh == mapInstanceNametoMeshObj.end())
	//{
	//	return false;
	//}
	for (std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.begin(); itCurrentMesh != mapInstanceNametoMeshObj.end(); itCurrentMesh++)
	{
		if ((itCurrentMesh->second->meshName == "floorA") || (itCurrentMesh->second->meshName == "floorB") || (itCurrentMesh->second->meshName == "floorC")||
			(itCurrentMesh->second->meshName == "wall") || (itCurrentMesh->second->meshName == "door") )
		{
			itCurrentMesh->second->bUse_RGBA_colour = false;
			itCurrentMesh->second->textures[0] = textureFile;
			itCurrentMesh->second->textureRatios[0] = 1;
			itCurrentMesh->second->scale = 0.01;
			itCurrentMesh->second->position.x = itCurrentMesh->second->position.x * 5;
			itCurrentMesh->second->position.z = itCurrentMesh->second->position.z * 5;
			itCurrentMesh->second->isVisible = true;

		}
		if ((itCurrentMesh->second->meshName == "torch"))
		{
			itCurrentMesh->second->bUse_RGBA_colour = false;
			itCurrentMesh->second->textures[0] = textureFile;
			itCurrentMesh->second->textureRatios[0] = 1;
			itCurrentMesh->second->scale = 0.03;
			itCurrentMesh->second->position.x = itCurrentMesh->second->position.x * 5;
			itCurrentMesh->second->position.y = 3.f;
			itCurrentMesh->second->position.z = itCurrentMesh->second->position.z * 5;
			itCurrentMesh->second->isVisible = true;
		}
		if ((itCurrentMesh->second->meshName == "debug_light"))
		{
			itCurrentMesh->second->bUse_RGBA_colour = false;
			itCurrentMesh->second->textures[0] = textureFile;
			itCurrentMesh->second->textureRatios[0] = 1;
			itCurrentMesh->second->scale = 0.5;
			itCurrentMesh->second->position.x = itCurrentMesh->second->position.x * 5;
			itCurrentMesh->second->position.y = 4.f;
			itCurrentMesh->second->position.z = itCurrentMesh->second->position.z * 5;
			itCurrentMesh->second->isVisible = false;
		}
		if ((itCurrentMesh->second->meshName == "terrain"))
		{
			itCurrentMesh->second->scale = 20;
			itCurrentMesh->second->position.x = -124.8f;
			itCurrentMesh->second->position.y = -40.f;
			itCurrentMesh->second->position.z = -1.6f;
			itCurrentMesh->second->rotation.y = 0.f;
			itCurrentMesh->second->isWireframe = false;
			itCurrentMesh->second->color_RGBA = glm::vec4(1,1,0,1);
			itCurrentMesh->second->bUse_RGBA_colour = true;
		}
		if ((itCurrentMesh->second->meshName == "water"))
		{
			itCurrentMesh->second->scale = 1.1f;
			itCurrentMesh->second->position.x = -167.7f;
			itCurrentMesh->second->position.y = -10.5f;
			itCurrentMesh->second->position.z = -54.f;
			itCurrentMesh->second->rotation.y = -5.9f;
			itCurrentMesh->second->rotation.z = 0.0f;
			itCurrentMesh->second->isWireframe = false;
			itCurrentMesh->second->color_RGBA = glm::vec4(1, 1, 0, 1);
			itCurrentMesh->second->bUse_RGBA_colour = true;
#if MOVINGTEXTURE
			itCurrentMesh->second->isMovingTexture = true;
#endif
		}
		if ((itCurrentMesh->second->meshName == "boss"))
		{
			itCurrentMesh->second->scale = 0.8;
			itCurrentMesh->second->position.x = -27.5f;
			itCurrentMesh->second->position.y = 1.f;
			itCurrentMesh->second->position.z = 0.f;
			itCurrentMesh->second->rotation.y = 0.f;
			itCurrentMesh->second->rotation.z = 0.0f;
			itCurrentMesh->second->isWireframe = false;
			itCurrentMesh->second->color_RGBA = glm::vec4(1, 1, 0, 1);
			itCurrentMesh->second->bUse_RGBA_colour = true;
		}

		//if ((itCurrentMesh->second->meshName == "projecter"))
		//{
		//	itCurrentMesh->second->bUse_RGBA_colour = false;
		//	itCurrentMesh->second->textures[0] = textureFile;
		//	itCurrentMesh->second->textureRatios[0] = 1;
		//	itCurrentMesh->second->scale = 0.03;
		//	itCurrentMesh->second->position.x = itCurrentMesh->second->position.x;//* 5;
		//	itCurrentMesh->second->position.z = itCurrentMesh->second->position.z;//* 5;
		//	itCurrentMesh->second->isVisible = true;
		//}
	}
	//cMeshObj* pCurrentMeshObject = itCurrentMesh->second;
	//pCurrentMeshObject->bUse_RGBA_colour = false;
	//pCurrentMeshObject->textures[arrPos] = textureFile;
	//pCurrentMeshObject->textureRatios[arrPos] = 1.0f;

	return true;
}

bool cVAOManager::setTorchTexture(std::string meshObjName, std::string textureFile, std::string markTextureFile)
{
	for (std::map<std::string, cMeshObj* >::iterator itCurrentMesh = mapInstanceNametoMeshObj.begin(); itCurrentMesh != mapInstanceNametoMeshObj.end(); itCurrentMesh++)
	{
		if ((itCurrentMesh->second->meshName == "flame"))
		{
			itCurrentMesh->second->bUse_RGBA_colour = false;
			itCurrentMesh->second->textures[0] = textureFile;
			itCurrentMesh->second->textures[7] = markTextureFile;
			itCurrentMesh->second->textureRatios[0] = 1;
			itCurrentMesh->second->scale = 7.5;
			itCurrentMesh->second->position.x = itCurrentMesh->second->position.x * 5;
			itCurrentMesh->second->position.y = 4.5f;
			itCurrentMesh->second->position.z = itCurrentMesh->second->position.z * 5;
			itCurrentMesh->second->isVisible = true;
			itCurrentMesh->second->bDoNotLight = true;
		}
	}
	return true;
}