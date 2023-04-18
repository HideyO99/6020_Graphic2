#include "Model.h"

Model::Model()
{
}


Model::~Model()
{
}

Model::Model(const char* filepath)
{
	const aiScene* scene = m_AssimpImporter.ReadFile(filepath, ASSIMP_LOAD_FLAGS | aiProcess_ConvertToLeftHanded);

	aiMesh* mesh = scene->mMeshes[0];

	if (scene == 0 || !scene->HasMeshes())
		return;

	unsigned int numVerticesInVertArray = mesh->mNumVertices;
	unsigned int numIndicesInIndexArray = mesh->mNumFaces * 3;
	sVertex_RGBA_XYZ_N_UV_T_BiN_Bones* pTempVertArray = new sVertex_RGBA_XYZ_N_UV_T_BiN_Bones[numIndicesInIndexArray * 2];
	GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];
	int count = 0;
	int index = 0;

	for (unsigned int vertIdx = 0; vertIdx != mesh->mNumVertices; vertIdx++)
	{
		aiVector3D position = mesh->mVertices[vertIdx];
		Vertices.push_back(glm::vec3(position.x, position.y, position.z));
	}

	unsigned int vertArrayIndex = 0;
	for (unsigned int faceIdx = 0; faceIdx != mesh->mNumFaces; faceIdx++)
	{
		aiFace face = mesh->mFaces[faceIdx];

		for (int idx = 0; idx != 3; idx++)
		{
			unsigned int index = face.mIndices[idx];

			Triangles.push_back(index);

			aiVector3D position = mesh->mVertices[index];
			pTempVertArray[vertArrayIndex].Pos.x = position.x;
			pTempVertArray[vertArrayIndex].Pos.y = position.y;
			pTempVertArray[vertArrayIndex].Pos.z = position.z;
			pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

			if (mesh->HasTextureCoords(0)) {
				aiVector3D textureCoord = mesh->mTextureCoords[0][index];
				pTempVertArray[vertArrayIndex].TexUVx2.x = textureCoord.x;
				pTempVertArray[vertArrayIndex].TexUVx2.y = textureCoord.y;
				pTempVertArray[vertArrayIndex].TexUVx2.z = textureCoord.z;
				pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
			}
			else
			{
				pTempVertArray[vertArrayIndex].TexUVx2.x = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.y = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.z = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
			}

			aiVector3D normal = mesh->mNormals[index];
			pTempVertArray[vertArrayIndex].Normal.x = normal.x;
			pTempVertArray[vertArrayIndex].Normal.y = normal.y;
			pTempVertArray[vertArrayIndex].Normal.z = normal.z;
			pTempVertArray[vertArrayIndex].Normal.w = 0.0f;

			pTempVertArray[vertArrayIndex].BoneID.x = 0;
			pTempVertArray[vertArrayIndex].BoneID.y = 1;
			pTempVertArray[vertArrayIndex].BoneID.z = 2;
			pTempVertArray[vertArrayIndex].BoneID.w = 3;

			glm::vec4 weights;
			weights.x = abs(0.f - position.y);
			weights.y = abs(70.f - position.y);
			weights.z = abs(140.f - position.y);
			weights.w = abs(200.f - position.y);

			weights = glm::normalize(weights);

			pTempVertArray[vertArrayIndex].BoneWeight.x = weights.x;
			pTempVertArray[vertArrayIndex].BoneWeight.y = weights.y;
			pTempVertArray[vertArrayIndex].BoneWeight.z = weights.z;
			pTempVertArray[vertArrayIndex].BoneWeight.w = weights.w;



			pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

			vertArrayIndex++;
		}
	}

	NumTriangles = mesh->mNumFaces;

	glGenVertexArrays(1, &VBO);
	glBindVertexArray(VBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glGenBuffers(1, &VertBuffID);
	glGenBuffers(1, &IndBuffID);

	glBindBuffer(GL_ARRAY_BUFFER, VertBuffID);

	unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones); ;
	glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);

	unsigned int bytesInOneVertex = sizeof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones);
	unsigned int byteOffsetToPosition = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, Pos);
	unsigned int byteOffsetToUVCoords = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, TexUVx2);
	unsigned int byteOffsetToNormal = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, Normal);
	unsigned int byteOffsetToBoneIds = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, BoneID);
	unsigned int byteOffsetToBoneWeights = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, BoneWeight);
	unsigned int byteOffsetToTangent = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, Tangent);
	unsigned int byteOffsetToBiNormal = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, BiNormal);
	unsigned int byteOffsetToRGBA = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, RGBA);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneIds);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneWeights);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToTangent);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBiNormal);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToRGBA);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndBuffID);

	unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] pTempVertArray;
	delete[] pIndexArrayLocal;
}