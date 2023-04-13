#include "c3DModelFileLoader.h"
#include "../Texture/cTextureManager.h"

extern cTextureManager* g_pTextureManager;

void CastToGLM(const aiMatrix4x4& in, glm::mat4& out);

//bool c3DModelFileLoader::loadFBXFileAsync(std::string filename, std::string meshName, cModelDrawInfo* modelDrawInfo, unsigned int shaderProgramID)
//{
//	const aiScene* scene = m_Importer.ReadFile(filename, ASSIMP_LOAD_FLAGS);
//
//	aiNode* node = scene->mRootNode;
//	//for (int i = 0; i < node->mNumChildren; i++)
//	//{
//	//	aiNode* child = node->mChildren[i];
//
//	//	//Find channel data from our node name:
//	//	child->mName;
//	//}
//
//	if (scene == 0 || !scene->HasMeshes())
//	{
//		return false;
//	}
//
//	//get texture
//	std::string test;
//	if (scene->HasMaterials())
//	{
//		for (int i = 0; i < scene->mNumMaterials; i++)
//		{
//			const aiMaterial* pMat = scene->mMaterials[i];
//			if (pMat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
//			{
//				aiString path;
//				if (pMat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS)
//				{
//					std::string tmp = path.data;
//					tmp = tmp.substr(tmp.find_last_of("/\\") + 1, std::string::npos);
//					if (::g_pTextureManager->isNotExistTexture(tmp))
//					{
//						::g_pTextureManager->create2DTextureFromFreeImgLib(tmp);
//					}
//				}
//			}
//		}
//	}
//
//
//	std::vector<cModelDrawInfo*> vecModelDraw;
//	//cMeshObj* meshObj = findMeshObjAddr(meshName);
//
//	for (int i = 0; i < scene->mNumMeshes; i++)
//	{
//		cModelDrawInfo* modelDrawInfo = new cModelDrawInfo();
//		aiMesh* mesh = scene->mMeshes[i];
//		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
//		aiString texturePath;
//		loadMesh(mesh, modelDrawInfo, meshObj);
//		if (i == 0)
//		{
//			//loadModelToVAO(i_mapModel->first, modelDrawInfo, shaderProgramID);
//		}
//		else
//		{
//			cMeshObj* childMeshObj = new cMeshObj();
//			childMeshObj->instanceName = mesh->mName.C_Str();
//			childMeshObj->meshName = mesh->mName.C_Str();
//			meshObj->vecChildMesh.push_back(childMeshObj);
//			//loadModelToVAO(mesh->mName.C_Str(), modelDrawInfo, shaderProgramID);
//		}
//		if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS)
//		{
//			std::string tmp = texturePath.data;
//			modelDrawInfo->TextureFile = tmp.substr(tmp.find_last_of("/\\") + 1, std::string::npos);
//		}
//		vecModelDraw.push_back(modelDrawInfo);
//	}
//	//mapModeltoMultiMesh.emplace(i_mapModel->first, vecModelDraw);
//
//
//	return true;
//}
//
//bool c3DModelFileLoader::loadMesh(const aiMesh* mesh, cModelDrawInfo* modelDrawInfo, cMeshObj* meshObj)
//{
//	modelDrawInfo->numberOfVertices = mesh->mNumVertices;
//	modelDrawInfo->numberOfIndices = mesh->mNumFaces * 3;
//	modelDrawInfo->numberOfTriangles = mesh->mNumFaces;
//	modelDrawInfo->pVertices = new cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones[modelDrawInfo->numberOfVertices];
//
//	int totalBoneWeights = 0;
//	std::vector<BoneVertexData> boneVertexData;
//	boneVertexData.resize(mesh->mNumVertices);
//	int boneCount = 0;
//
//	if (mesh->HasBones())
//	{
//		unsigned int numBone = mesh->mNumBones;
//		meshObj->hasBone = true;
//		for (int i_bones = 0; i_bones < numBone; i_bones++)
//		{
//			const aiBone* bone = mesh->mBones[i_bones];
//			std::string boneName = bone->mName.C_Str();
//			int boneIDx = 0;
//			std::map<std::string, int>::iterator it = meshObj->boneNameToIdMap.find(boneName);
//			if (it == meshObj->boneNameToIdMap.end())
//			{
//				boneIDx = boneCount;
//				boneCount++;
//				BoneInfo boneInfo;
//				boneInfo.name = boneName;
//				meshObj->boneInfoVec.push_back(boneInfo);
//				CastToGLM(bone->mOffsetMatrix, meshObj->boneInfoVec[boneIDx].boneOffset);
//				meshObj->boneNameToIdMap.emplace(boneName, boneIDx);
//				meshObj->BoneModelMatrices.push_back(glm::mat4(1.f));
//
//			}
//			else
//			{
//				boneIDx = meshObj->boneNameToIdMap[boneName];
//			}
//			//todo
//
//			for (int i = 0; i < bone->mNumWeights; i++)
//			{
//				const aiVertexWeight vw = bone->mWeights[i];
//				float weight = vw.mWeight;
//				int vertID = vw.mVertexId;
//				boneVertexData[vertID].AddBoneInfo(boneIDx, weight);
//				//modelDrawInfo->pVertices[vw.mVertexId];
//			}
//			//modelDrawInfo->pVertices[i_vertices].vBoneID[0] = mesh->mBones[i_bones]->mWeights[]
//			//modelDrawInfo->pVertices[i_vertices].vBoneWeight[0] = mesh->mBones[i_vertices]->mWeights[0].mWeight;
//		}
//
//	}
//
//	for (int i_vertices = 0; i_vertices < modelDrawInfo->numberOfVertices; i_vertices++)
//	{
//		modelDrawInfo->pVertices[i_vertices].x = mesh->mVertices[i_vertices].x;
//		modelDrawInfo->pVertices[i_vertices].y = mesh->mVertices[i_vertices].y;
//		modelDrawInfo->pVertices[i_vertices].z = mesh->mVertices[i_vertices].z;
//
//		if (mesh->HasNormals())
//		{
//			modelDrawInfo->pVertices[i_vertices].nx = mesh->mNormals[i_vertices].x;
//			modelDrawInfo->pVertices[i_vertices].ny = mesh->mNormals[i_vertices].y;
//			modelDrawInfo->pVertices[i_vertices].nz = mesh->mNormals[i_vertices].z;
//		}
//		else
//		{
//			modelDrawInfo->pVertices[i_vertices].nx = 0;
//			modelDrawInfo->pVertices[i_vertices].ny = 0;
//			modelDrawInfo->pVertices[i_vertices].nz = 0;
//		}
//
//		if (mesh->HasVertexColors(i_vertices))
//		{
//			modelDrawInfo->pVertices[i_vertices].r = mesh->mColors[i_vertices]->r;
//			modelDrawInfo->pVertices[i_vertices].g = mesh->mColors[i_vertices]->g;
//			modelDrawInfo->pVertices[i_vertices].b = mesh->mColors[i_vertices]->b;
//			modelDrawInfo->pVertices[i_vertices].a = mesh->mColors[i_vertices]->a;
//		}
//		else
//		{
//			modelDrawInfo->pVertices[i_vertices].r = 0;
//			modelDrawInfo->pVertices[i_vertices].g = 0;
//			modelDrawInfo->pVertices[i_vertices].b = 0;
//			modelDrawInfo->pVertices[i_vertices].a = 1;
//		}
//
//		if (mesh->HasTextureCoords(0))
//		{
//			modelDrawInfo->pVertices[i_vertices].u0 = mesh->mTextureCoords[0][i_vertices].x;
//			modelDrawInfo->pVertices[i_vertices].v0 = mesh->mTextureCoords[0][i_vertices].y;
//		}
//		else
//		{
//			modelDrawInfo->pVertices[i_vertices].u0 = 0;
//			modelDrawInfo->pVertices[i_vertices].v0 = 0;
//		}
//
//		if (mesh->HasTextureCoords(1))
//		{
//			modelDrawInfo->pVertices[i_vertices].u1 = mesh->mTextureCoords[1][i_vertices].x;
//			modelDrawInfo->pVertices[i_vertices].v1 = mesh->mTextureCoords[1][i_vertices].y;
//		}
//		else
//		{
//			modelDrawInfo->pVertices[i_vertices].u1 = 0;
//			modelDrawInfo->pVertices[i_vertices].v1 = 0;
//		}
//
//		modelDrawInfo->pVertices[i_vertices].vBoneWeight[0] = boneVertexData[i_vertices].weights[0];
//		modelDrawInfo->pVertices[i_vertices].vBoneWeight[1] = boneVertexData[i_vertices].weights[1];
//		modelDrawInfo->pVertices[i_vertices].vBoneWeight[2] = boneVertexData[i_vertices].weights[2];
//		modelDrawInfo->pVertices[i_vertices].vBoneWeight[3] = boneVertexData[i_vertices].weights[3];
//		modelDrawInfo->pVertices[i_vertices].vBoneID[0] = boneVertexData[i_vertices].ids[0];
//		modelDrawInfo->pVertices[i_vertices].vBoneID[1] = boneVertexData[i_vertices].ids[1];
//		modelDrawInfo->pVertices[i_vertices].vBoneID[2] = boneVertexData[i_vertices].ids[2];
//		modelDrawInfo->pVertices[i_vertices].vBoneID[3] = boneVertexData[i_vertices].ids[3];
//
//	}
//
//	unsigned int vertex_element_index_index = 0;
//	modelDrawInfo->pIndices = new unsigned int[modelDrawInfo->numberOfIndices];
//	for (unsigned int i = 0; i != modelDrawInfo->numberOfTriangles; i++)
//	{
//		modelDrawInfo->pIndices[vertex_element_index_index + 0] = mesh->mFaces[i].mIndices[0];
//		modelDrawInfo->pIndices[vertex_element_index_index + 1] = mesh->mFaces[i].mIndices[1];
//		modelDrawInfo->pIndices[vertex_element_index_index + 2] = mesh->mFaces[i].mIndices[2];
//		vertex_element_index_index += 3;
//	}
//	return true;
//
//}

bool c3DModelFileLoader::loadPLYFile(std::string filename, cModelDrawInfo* modelDrawInfo, std::string error)
{
	sTrianglePLY* pTheModelTriangleArray = NULL;
	std::ifstream modelFile;

	modelFile.open(filename, std::ifstream::in);
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
	modelFile >> modelDrawInfo->numberOfVertices;

	while (modelFile >> nextToken)
	{
		if (nextToken == "face")
		{
			break;
		}
	}
	modelFile >> modelDrawInfo->numberOfTriangles;

	while (modelFile >> nextToken)
	{
		if (nextToken == "end_header")
		{
			break;
		}
	}

	modelDrawInfo->pVertices = new cModelDrawInfo::sVertex_RGBA_XYZ_N_UV_T_BiN_Bones[modelDrawInfo->numberOfVertices];

	for (unsigned int i = 0; i != modelDrawInfo->numberOfVertices; i++)
	{

		modelFile >> modelDrawInfo->pVertices[i].x;
		modelFile >> modelDrawInfo->pVertices[i].y;
		modelFile >> modelDrawInfo->pVertices[i].z;

		modelFile >> modelDrawInfo->pVertices[i].nx;
		modelFile >> modelDrawInfo->pVertices[i].ny;
		modelFile >> modelDrawInfo->pVertices[i].nz;

		modelFile >> modelDrawInfo->pVertices[i].r;
		modelFile >> modelDrawInfo->pVertices[i].g;
		modelFile >> modelDrawInfo->pVertices[i].b;
		modelFile >> modelDrawInfo->pVertices[i].a;

		modelFile >> modelDrawInfo->pVertices[i].u0;
		modelFile >> modelDrawInfo->pVertices[i].v0;

	}

	pTheModelTriangleArray = new sTrianglePLY[modelDrawInfo->numberOfTriangles];
	for (unsigned int count = 0; count != modelDrawInfo->numberOfTriangles; count++)
	{
		unsigned int discard = 0;
		modelFile >> discard;

		modelFile >> pTheModelTriangleArray[count].vertexIndices[0];
		modelFile >> pTheModelTriangleArray[count].vertexIndices[1];
		modelFile >> pTheModelTriangleArray[count].vertexIndices[2];
	}

	modelFile.close();

	modelDrawInfo->numberOfIndices = modelDrawInfo->numberOfTriangles * 3;
	modelDrawInfo->pIndices = new unsigned int[modelDrawInfo->numberOfIndices];

	unsigned int vertex_element_index_index = 0;

	for (unsigned int i = 0; i != modelDrawInfo->numberOfTriangles; i++)
	{
		modelDrawInfo->pIndices[vertex_element_index_index + 0] = pTheModelTriangleArray[i].vertexIndices[0];
		modelDrawInfo->pIndices[vertex_element_index_index + 1] = pTheModelTriangleArray[i].vertexIndices[1];
		modelDrawInfo->pIndices[vertex_element_index_index + 2] = pTheModelTriangleArray[i].vertexIndices[2];
		vertex_element_index_index += 3;
	}
	return true;
}

void CastToGLM(const aiMatrix4x4& in, glm::mat4& out)
{
	out = glm::mat4(in.a1, in.b1, in.c1, in.d1,
		in.a2, in.b2, in.c2, in.d2,
		in.a3, in.b3, in.c3, in.d3,
		in.a4, in.b4, in.c4, in.d4);
}