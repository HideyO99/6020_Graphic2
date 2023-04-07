#include "cAssimpModel.h"

//cAssimpModel::cAssimpModel()
//{
//}
//
//cAssimpModel::~cAssimpModel()
//{
//}
//
//void cAssimpModel::loadModelFromFile(const std::string& filename)
//{
//	const aiScene* scene = m_Importer.ReadFile(filename,
//		aiProcess_GenNormals |
//		aiProcess_LimitBoneWeights |
//		aiProcess_Triangulate |
//		aiProcess_JoinIdenticalVertices);
//	
//	// Create our Bone Hierarchy
//	aiNode* node = scene->mRootNode;
//	for (int i = 0; i < node->mNumChildren; i++)
//	{
//		aiNode* child = node->mChildren[i];
//
//		//Find channel data from our node name:
//		child->mName;
//	}
//
//	if (scene->HasMeshes())
//	{
//		for (int i = 0; i < scene->mNumMeshes; i++)
//		{
//			aiMesh* mesh = scene->mMeshes[i];
//			loadMesh(mesh);
//		}
//	}
//}
//
//void cAssimpModel::loadMesh(const aiMesh* mesh)
//{
//	if (mesh == nullptr)
//	{
//		return;
//	}
//    printf("LoadAssimpMesh %s\n", mesh->mName.C_Str());
//
//    unsigned int numFaces = mesh->mNumFaces;
//
//    unsigned int numIndicesInIndexArray = numFaces * 3;
//
//    sVertex_p4t4n4* pTempVertArray = new sVertex_p4t4n4[numIndicesInIndexArray * 2];
//    GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];
//
//    std::vector<unsigned int> triangles;
//    int vertArrayIndex = 0;
//    for (int faceIndex = 0; faceIndex < numFaces; faceIndex++)
//    {
//        const aiFace& face = mesh->mFaces[faceIndex];
//
//        // You can assert here to ensure mNumIndices is 3
//        // Unless you support non 3 face models 
//
//        for (int indicesIndex = 0; indicesIndex < face.mNumIndices; indicesIndex++)
//        {
//            unsigned int vertexIndex = face.mIndices[indicesIndex];
//            triangles.push_back(vertexIndex);
//
//            const aiVector3D& vertex = mesh->mVertices[vertexIndex];
//
//            pTempVertArray[vertArrayIndex].Pos.x = vertex.x;
//            pTempVertArray[vertArrayIndex].Pos.y = vertex.y;
//            pTempVertArray[vertArrayIndex].Pos.z = vertex.z;
//            pTempVertArray[vertArrayIndex].Pos.w = 1.0f;
//
//            if (mesh->HasNormals())
//            {
//                const aiVector3D& normal = mesh->mNormals[vertexIndex];
//                pTempVertArray[vertArrayIndex].Normal.x = normal.x;
//                pTempVertArray[vertArrayIndex].Normal.y = normal.y;
//                pTempVertArray[vertArrayIndex].Normal.z = normal.z;
//                pTempVertArray[vertArrayIndex].Normal.w = 0.f;
//            }
//            else
//            {
//                pTempVertArray[vertArrayIndex].Normal.x = 1.f;
//                pTempVertArray[vertArrayIndex].Normal.y = 0.f;
//                pTempVertArray[vertArrayIndex].Normal.z = 0.f;
//                pTempVertArray[vertArrayIndex].Normal.w = 0.f;
//            }
//
//            if (mesh->HasTextureCoords(0))
//            {
//                const aiVector3D& uvCoord = mesh->mTextureCoords[0][vertexIndex];
//                pTempVertArray[vertArrayIndex].TexUVx2.x = uvCoord.x;
//                pTempVertArray[vertArrayIndex].TexUVx2.y = uvCoord.y;
//            }
//            if (mesh->HasTextureCoords(1))
//            {
//                const aiVector3D& uvCoord = mesh->mTextureCoords[1][vertexIndex];
//                pTempVertArray[vertArrayIndex].TexUVx2.z = uvCoord.x;
//                pTempVertArray[vertArrayIndex].TexUVx2.w = uvCoord.y;
//            }
//
//            vertArrayIndex++;
//        }
//    }
//
//
//    printf("  - Done loading Model data");
//    GLenum result;
//    Model* model = new Model();
//    model->NumTriangles = numFaces;
//    glGenVertexArrays(1, &model->Vbo);
//    glBindVertexArray(model->Vbo);
//    result = glGetError();
//
//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glEnableVertexAttribArray(2);
//    result = glGetError();
//
//    glGenBuffers(1, &model->VertexBufferId);
//    glGenBuffers(1, &model->IndexBufferId);
//    result = glGetError();
//
//    glBindBuffer(GL_ARRAY_BUFFER, model->VertexBufferId);
//    result = glGetError();
//
//    unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4t4n4);
//    glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);
//    result = glGetError();
//
//    unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4);
//    unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4, Pos);
//    unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4, Normal);
//    unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4, TexUVx2);
//
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
//    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
//    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IndexBufferId);
//
//    unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);
//
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);
//
//    glBindVertexArray(0);
//
//    delete[] pTempVertArray;
//    delete[] pIndexArrayLocal;
//
//    printf("  - Done Generating Buffer data");
//
//    printf("  - Finished Loading model \"%s\" with %d vertices, %d triangles, id is: %d\n", mesh->mName.C_Str(), model->vertices.size(), model->NumTriangles, model->Vbo);
//    id = gModelVec.size();
//    gModelVec.push_back(model);
//    return true;
//
//}
