#include "Character.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
AssimpScene::AssimpScene(const std::string filename, unsigned int flags)
{
	m_Scene = m_Importer.ReadFile(filename.c_str(), flags);

	Animations = m_Scene->mAnimations;
	Cameras = m_Scene->mCameras;
	Lights = m_Scene->mLights;
	Materials = m_Scene->mMaterials;
	Meshes = m_Scene->mMeshes;
	Textures = m_Scene->mTextures;

	RootNode = m_Scene->mRootNode;
}

AssimpScene::~AssimpScene()
{
}

Character::Character()
{
	m_IsPlaying = true;
	m_AnimationSpeed = 1.f;
	m_CurrentTimeInSeconds = 0.f;
	m_CurrentAnimation = 0;
	m_NumAnimationsLoaded = 0;
	m_UpdateMode = Kinematic;
}

Character::~Character()
{
}

void Character::LoadCharacterFromAssimp(const std::string filename)
{
	m_Scene = new AssimpScene(filename, CHARACTERPROCESSFLAG);

	aiMatrix4x4 g = m_Scene->RootNode->mTransformation;
	aiMatrix4x4 inverse = g.Inverse();

	// Node hierarchy for rendering
	m_RootNode = CreateNodeHierarchy(m_Scene->RootNode);
	CastToGLM(inverse, m_GlobalInverseTransform);

	if (m_Scene->HasMeshes())
	{
		// Load all the meshes
		unsigned int numMeshes = m_Scene->NumMeshes();
		std::string fname = filename.substr(filename.find_last_of('/\\') + 1, std::string::npos);
		std::cout << "Loading " << fname << std::endl;
		std::cout << "\t" << numMeshes << " meshes" << std::endl;

		for (int i = 0; i < m_Scene->NumMeshes(); i++)
		{
			aiMesh* mesh = m_Scene->Meshes[i];

			std::cout << "\tMesh:" << mesh->mName.C_Str() << std::endl;

		};

		{
			aiMesh* mesh = m_Scene->Meshes[0];
			if (!LoadAssimpMesh(mesh))
			{
				std::cout << "Failed to load mesh!" << std::endl;
			}
		}
	}
	std::cout << "Load FBX is done" << std::endl;
}

void Character::LoadAnimationFromAssimp(const std::string filename)
{
	const aiScene* scene = m_AnimationImporter.ReadFile(filename, 0);

	unsigned int numAnimations = scene->mNumAnimations;
	std::cout << "Loading animation: " << numAnimations << std::endl;
	aiAnimation* animation = scene->mAnimations[0];
	LoadAssimpAnimation(animation);
}

void Character::UpdateTransforms(std::vector<glm::mat4>& transforms, std::vector<glm::mat4>& globals, float dt)
{
	if (m_IsPlaying && m_AnimationSpeed != 0.0f)
	{
		m_CurrentTimeInSeconds += dt * m_AnimationSpeed;
		m_CurrentTimeInSeconds = fmod(m_CurrentTimeInSeconds, m_DurationInSeconds[m_CurrentAnimation]);

		int keyFrameTime = (int)((m_CurrentTimeInSeconds / m_DurationInSeconds[m_CurrentAnimation]) *
			m_DurationInTicks[m_CurrentAnimation]);

		glm::mat4 identity(1.f);

		if (m_TransitionTime > 0.0f)
		{
			m_TransitionTime -= dt;
			if (m_TransitionTime < 0.0f)
			{
				m_TransitionTime = 0.0f;
			}
		}

		//printf("--------------------\n");
		//printf("Time: %.4f %d/%d\n", m_CurrentTimeInSeconds, keyFrameTime, (int)m_DurationInTicks);

		if (m_UpdateMode == Kinematic)
		{
			UpdateBoneHierarchy(m_RootNode, identity, keyFrameTime);
			transforms.resize(m_BoneInfoVec.size());
			globals.resize(m_BoneInfoVec.size());
			int numTransforms = m_BoneInfoVec.size();
			for (int i = 0; i < numTransforms; i++)
			{
				transforms[i] = m_BoneInfoVec[i].finalTransformation;
				globals[i] = m_BoneInfoVec[i].globalTransformation;
			}
		}
		else
		{
			// Should be able to do this once during load:
			// Calculate Transform data from NodeHierarchy 

			// Update from Ragdoll info
			//int numTransforms = m_BoneInfoVec.size();
			//glm::mat4 bodyPartTransform;
			//for (int i = 0; i < numTransforms; i++)
			//{
			//	m_Ragdoll->GetBodyPartTransform(i, bodyPartTransform);
			//	transforms[i] = bodyPartTransform; // * NodeTransform
			//}
		}
	}
}

Model* Character::GetModel(int index)
{
	assert(index >= 0);
	assert(index < m_Models.size());
	return m_Models[index];
}

void Character::LoadAssimpBones(const aiMesh* assimpMesh)
{
	// Record Vertex Weights
	int totalWeights = 0;
	m_BoneVertexData.resize(assimpMesh->mNumVertices);
	int boneCount = 0;

	int numBones = assimpMesh->mNumBones;
	for (int i = 0; i < numBones; i++)
	{
		aiBone* bone = assimpMesh->mBones[i];

		int boneIdx = 0;
		std::string boneName = bone->mName.C_Str();

		std::cout << i << ":" << boneName << std::endl;

		std::map<std::string, int>::iterator it = m_BoneNameToIdMap.find(boneName);
		if (it == m_BoneNameToIdMap.end())
		{
			boneIdx = boneCount;
			boneCount++;
			BoneInfo bi;
			bi.name = boneName;
			m_BoneInfoVec.push_back(bi);

			CastToGLM(bone->mOffsetMatrix, m_BoneInfoVec[boneIdx].boneOffset);
			m_BoneNameToIdMap.emplace(boneName, boneIdx);
		}
		else
		{
			boneIdx = it->second;
		}

		for (int weightIdx = 0; weightIdx < bone->mNumWeights; weightIdx++)
		{
			const aiVertexWeight vw = bone->mWeights[i];
			float weight = vw.mWeight;
			int vertID = vw.mVertexId;
			m_BoneVertexData[vertID].AddBoneInfo(boneIdx, weight);
		}
	}
}

bool Character::LoadAssimpMesh(const aiMesh* mesh)
{
	if (mesh == nullptr)
		return false;

	//Bones
	LoadAssimpBones(mesh);

	std::string meshName(mesh->mName.data);
	printf("%s\n", meshName.c_str());

	// Data
	unsigned int numFaces = mesh->mNumFaces;
	unsigned int numIndicesInIndexArray = numFaces * 3;
	sVertex_RGBA_XYZ_N_UV_T_BiN_Bones* pTempVertArray = new sVertex_RGBA_XYZ_N_UV_T_BiN_Bones[numIndicesInIndexArray * 2];
	GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];

	int vertArrayIndex = 0;
	for (int faceIndex = 0; faceIndex < numFaces; faceIndex++)
	{
		const aiFace& face = mesh->mFaces[faceIndex];

		unsigned int numIndices = face.mNumIndices;
		for (int indicesIndex = 0; indicesIndex < numIndices; indicesIndex++)
		{
			unsigned int vertexIndex = face.mIndices[indicesIndex];

			const aiVector3D& vertex = mesh->mVertices[vertexIndex];

			pTempVertArray[vertArrayIndex].Pos.x = vertex.x;
			pTempVertArray[vertArrayIndex].Pos.y = vertex.y;
			pTempVertArray[vertArrayIndex].Pos.z = vertex.z;
			pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

			if (mesh->HasNormals())
			{
				const aiVector3D& normal = mesh->mNormals[vertexIndex];
				pTempVertArray[vertArrayIndex].Normal.x = normal.x;
				pTempVertArray[vertArrayIndex].Normal.y = normal.y;
				pTempVertArray[vertArrayIndex].Normal.z = normal.z;
				pTempVertArray[vertArrayIndex].Normal.w = 0.f;
			}
			else
			{
				pTempVertArray[vertArrayIndex].Normal.x = 1.f;
				pTempVertArray[vertArrayIndex].Normal.y = 0.f;
				pTempVertArray[vertArrayIndex].Normal.z = 0.f;
				pTempVertArray[vertArrayIndex].Normal.w = 0.f;
			}

			//if (assimpMesh->HasTextureCoords(0))
			//{
				//const aiVector3D& uvCoord = assimpMesh->mTextureCoords[0][vertexIndex];
			pTempVertArray[vertArrayIndex].TexUVx2.x = 0;
			pTempVertArray[vertArrayIndex].TexUVx2.y = 0;
			pTempVertArray[vertArrayIndex].TexUVx2.z = 0;
			pTempVertArray[vertArrayIndex].TexUVx2.w = 0;
			//}

			// Use a BoneInformation Map to get bone info and store the values here

			// Copy instead of reference, try..
			BoneVertexData bvd = m_BoneVertexData[vertexIndex];
			pTempVertArray[vertArrayIndex].BoneID.x = bvd.ids[0];
			pTempVertArray[vertArrayIndex].BoneID.y = bvd.ids[1];
			pTempVertArray[vertArrayIndex].BoneID.z = bvd.ids[2];
			pTempVertArray[vertArrayIndex].BoneID.w = bvd.ids[3];

			pTempVertArray[vertArrayIndex].BoneWeight.x = bvd.weights[0];
			pTempVertArray[vertArrayIndex].BoneWeight.y = bvd.weights[1];
			pTempVertArray[vertArrayIndex].BoneWeight.z = bvd.weights[2];
			pTempVertArray[vertArrayIndex].BoneWeight.w = bvd.weights[3];

			pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

			vertArrayIndex++;
		}
	}

	Model* model = new Model();
	//int testNumTriangles = triangles.size();
	model->NumTriangles = numFaces;
	glGenVertexArrays(1, &model->VBO);
	glBindVertexArray(model->VBO);
	//CheckGLError();

	glGenBuffers(1, &model->VertBuffID);
	glGenBuffers(1, &model->IndBuffID);
	//CheckGLError();

	glBindBuffer(GL_ARRAY_BUFFER, model->VertBuffID);
	//CheckGLError();

	unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones);
	glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);
	//CheckGLError();

	unsigned int bytesInOneVertex = sizeof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones);
	unsigned int byteOffsetToPosition = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, Pos);
	unsigned int byteOffsetToUVCoords = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, TexUVx2);
	unsigned int byteOffsetToNormal = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, Normal);
	unsigned int byteOffsetToBoneWeights = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, BoneWeight);
	unsigned int byteOffsetToBoneIds = offsetof(sVertex_RGBA_XYZ_N_UV_T_BiN_Bones, BoneID);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	//CheckGLError();

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneWeights);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToBoneIds);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IndBuffID);

	unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] pTempVertArray;
	delete[] pIndexArrayLocal;

	//printf("  - Done Generating Buffer data");

	//printf("  - Finished Loading model \"%s\" with %d vertices, %d triangles, id is: %d\n", mesh->mName.C_Str(), mesh->mNumVertices, model->NumTriangles, model->Vbo);
	m_MeshToIdMap[meshName] = m_Models.size();
	m_Models.push_back(model);

	return true;
}

BoneNode* Character::CreateNodeHierarchy(aiNode* animNode, int depth)
{
	BoneNode* newNode = new BoneNode();
	newNode->name = std::string(animNode->mName.data);
	CastToGLM(animNode->mTransformation, newNode->transformation);

	for (int i = 0; i < depth; i++)
		printf(" ");
	printf("%s (%d)\n", newNode->name.c_str(), animNode->mNumChildren);

	for (int i = 0; i < animNode->mNumChildren; i++)
	{
		BoneNode* childNode = CreateNodeHierarchy(animNode->mChildren[i], depth + 1);
		newNode->AddChild(childNode);
	}

	return newNode;
}

void Character::LoadAssimpAnimation(const aiAnimation* animation)
{
	if (animation == nullptr)
		return;

	if (m_NumAnimationsLoaded >= 10)
		return;

	unsigned int numChannels = animation->mNumChannels;

	// Hacking in which animation channel to add
	std::vector<AnimationData*>& channels = m_Channels[m_NumAnimationsLoaded];

	m_DurationInTicks[m_NumAnimationsLoaded] = animation->mDuration;
	m_TicksPerSecond[m_NumAnimationsLoaded] = animation->mTicksPerSecond;
	m_DurationInSeconds[m_NumAnimationsLoaded] =
		m_DurationInTicks[m_NumAnimationsLoaded] / m_TicksPerSecond[m_NumAnimationsLoaded];

	m_NumAnimationsLoaded++;

	channels.resize(numChannels);
	for (int i = 0; i < numChannels; i++)
	{
		const aiNodeAnim* nodeAnim = animation->mChannels[i];
		std::string name(nodeAnim->mNodeName.data);
		printf("%s\n", name.c_str());

		m_BoneNameToAnimationMap[name] = i;

		unsigned int numPositionKeys = nodeAnim->mNumPositionKeys;
		unsigned int numRotationKeys = nodeAnim->mNumRotationKeys;
		unsigned int numScalingKeys = nodeAnim->mNumScalingKeys;

		AnimationData* animData = new AnimationData();
		channels[i] = animData;
		animData->Name = name;

		animData->PositionKeyFrames.resize(numPositionKeys);
		animData->RotationKeyFrames.resize(numRotationKeys);
		animData->ScaleKeyFrames.resize(numScalingKeys);

		for (int keyIdx = 0; keyIdx < numPositionKeys; keyIdx++)
		{
			const aiVectorKey& posKey = nodeAnim->mPositionKeys[keyIdx];
			animData->PositionKeyFrames[keyIdx].time = posKey.mTime;
			animData->PositionKeyFrames[keyIdx].Pos.x = posKey.mValue.x;
			animData->PositionKeyFrames[keyIdx].Pos.y = posKey.mValue.y;
			animData->PositionKeyFrames[keyIdx].Pos.z = posKey.mValue.z;
		}

		for (int keyIdx = 0; keyIdx < numRotationKeys; keyIdx++)
		{
			const aiQuatKey& rotKey = nodeAnim->mRotationKeys[keyIdx];
			animData->RotationKeyFrames[keyIdx].time = rotKey.mTime;
			animData->RotationKeyFrames[keyIdx].Rotation.x = rotKey.mValue.x;
			animData->RotationKeyFrames[keyIdx].Rotation.y = rotKey.mValue.y;
			animData->RotationKeyFrames[keyIdx].Rotation.z = rotKey.mValue.z;
			animData->RotationKeyFrames[keyIdx].Rotation.w = rotKey.mValue.w;
		}

		for (int keyIdx = 0; keyIdx < numScalingKeys; keyIdx++)
		{
			const aiVectorKey& scaleKey = nodeAnim->mScalingKeys[keyIdx];
			animData->ScaleKeyFrames[keyIdx].time = scaleKey.mTime;
			animData->ScaleKeyFrames[keyIdx].Scale.x = scaleKey.mValue.x;
			animData->ScaleKeyFrames[keyIdx].Scale.y = scaleKey.mValue.y;
			animData->ScaleKeyFrames[keyIdx].Scale.z = scaleKey.mValue.z;
		}
	}
}

AnimationData* Character::FindAnimationData(const std::string& nodeName, int animation)
{
	std::map<std::string, int>::iterator animIt = m_BoneNameToAnimationMap.find(nodeName);

	for (int i = 0; i < m_Channels[animation].size(); i++)
	{
		if (nodeName == m_Channels[animation][i]->Name)
		{
			return m_Channels[animation][i];
		}
	}

	if (animIt != m_BoneNameToAnimationMap.end())
	{
		int breakhereplz = 0;
	}

	return nullptr;
}

void Character::UpdateBoneHierarchy(BoneNode* node, const glm::mat4& parentTransformationMatrix, float keyFrameTime)
{
	if (node == nullptr)
		return;

	std::string nodeName(node->name);

	glm::mat4 transformationMatrix = node->transformation;

	AnimationData* animNode = FindAnimationData(nodeName, m_CurrentAnimation);
	AnimationData* animNode2 = FindAnimationData(nodeName, m_PreviousAnimation);
	if (animNode != nullptr)
	{
		// Calculate the position of this node.
		glm::vec3 position = GetAnimationPosition(*animNode, keyFrameTime);
		glm::vec3 scale = GetAnimationScale(*animNode, keyFrameTime);
		glm::quat rotation = GetAnimationRotation(*animNode, keyFrameTime);

		if (animNode2 != nullptr && m_TransitionTime > 0.0f)
		{
			glm::vec3 position2 = GetAnimationPosition(*animNode2, keyFrameTime);
			glm::vec3 scale2 = GetAnimationScale(*animNode2, keyFrameTime);
			glm::quat rotation2 = GetAnimationRotation(*animNode2, keyFrameTime);

			float currRatio = 1.0f - m_TransitionTime;
			float prevRatio = m_TransitionTime;

			position = position * currRatio + position2 * prevRatio;
			scale = scale * currRatio + scale2 * prevRatio;
			rotation = glm::slerp(rotation2, rotation, currRatio);
		}

		// Calculate our transformation matrix
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}
	glm::mat4 globalTransformation = parentTransformationMatrix * transformationMatrix;

	if (m_BoneNameToIdMap.find(nodeName) != m_BoneNameToIdMap.end())
	{
		//printf("%d : %s\n", boneIt->second, nodeName.c_str());
		int boneIdx = m_BoneNameToIdMap[nodeName];
		m_BoneInfoVec[boneIdx].finalTransformation = m_GlobalInverseTransform * globalTransformation * m_BoneInfoVec[boneIdx].boneOffset;
		m_BoneInfoVec[boneIdx].globalTransformation = globalTransformation;
	}

	for (int i = 0; i < node->children.size(); i++)
	{
		UpdateBoneHierarchy(node->children[i], globalTransformation, keyFrameTime);
	}
}

int Character::FindPositionKeyFrameIndex(const AnimationData& animation, float keyFrameTime)
{
	for (int i = 0; i < animation.PositionKeyFrames.size() - 1; i++)
	{
		if (animation.PositionKeyFrames[i + 1].time > keyFrameTime)
			return i;
	}

	return 0;
}

int Character::FindScaleKeyFrameIndex(const AnimationData& animation, float keyFrameTime)
{
	for (int i = 0; i < animation.ScaleKeyFrames.size() - 1; i++)
	{
		if (animation.ScaleKeyFrames[i + 1].time > keyFrameTime)
			return i;
	}

	return 0;
}

int Character::FindRotationKeyFrameIndex(const AnimationData& animation, float keyFrameTime)
{
	for (int i = 0; i < animation.RotationKeyFrames.size() - 1; i++)
	{
		if (animation.RotationKeyFrames[i + 1].time > keyFrameTime)
			return i;
	}

	return 0;
}

glm::vec3 Character::GetAnimationPosition(const AnimationData& animation, float keyFrameTime)
{

	if (animation.PositionKeyFrames.size() == 1)
		return animation.PositionKeyFrames[0].Pos;

	int positionKeyFrameIndex = FindPositionKeyFrameIndex(animation, keyFrameTime);
	int nextPositionKeyFrameIndex = positionKeyFrameIndex + 1;
	PositionKeyFrame positionKeyFrame = animation.PositionKeyFrames[positionKeyFrameIndex];
	PositionKeyFrame nextPositionKeyFrame = animation.PositionKeyFrames[nextPositionKeyFrameIndex];
	float difference = nextPositionKeyFrame.time - positionKeyFrame.time;
	float ratio = (keyFrameTime - positionKeyFrame.time) / difference;

	if (ratio < 0.0f) ratio = 0.0f;
	if (ratio > 1.0f) ratio = 1.0f;

	glm::vec3 result = (nextPositionKeyFrame.Pos - positionKeyFrame.Pos) * ratio + positionKeyFrame.Pos;



	return result;
}

glm::vec3 Character::GetAnimationScale(const AnimationData& animation, float keyFrameTime)
{
	if (animation.ScaleKeyFrames.size() == 1)
		return animation.ScaleKeyFrames[0].Scale;

	int scaleKeyFrameIndex = FindScaleKeyFrameIndex(animation, keyFrameTime);
	int nextScaleKeyFrameIndex = scaleKeyFrameIndex + 1;
	ScaleKeyFrame scaleKeyFrame = animation.ScaleKeyFrames[scaleKeyFrameIndex];
	ScaleKeyFrame nextScaleKeyFrame = animation.ScaleKeyFrames[nextScaleKeyFrameIndex];
	float difference = nextScaleKeyFrame.time - scaleKeyFrame.time;
	float ratio = (keyFrameTime - scaleKeyFrame.time) / difference;

	if (ratio < 0.0f) ratio = 0.0f;
	if (ratio > 1.0f) ratio = 1.0f;

	glm::vec3 result = (nextScaleKeyFrame.Scale - scaleKeyFrame.Scale) * ratio + scaleKeyFrame.Scale;

	return result;
}

glm::quat Character::GetAnimationRotation(const AnimationData& animation, float keyFrameTime)
{
	if (animation.RotationKeyFrames.size() == 1)
		return animation.RotationKeyFrames[0].Rotation;

	int rotationKeyFrameIndex = FindRotationKeyFrameIndex(animation, keyFrameTime);
	int nextRotationKeyFrameIndex = rotationKeyFrameIndex + 1;
	RotationKeyFrame rotationKeyFrame = animation.RotationKeyFrames[rotationKeyFrameIndex];
	RotationKeyFrame nextRotationKeyFrame = animation.RotationKeyFrames[nextRotationKeyFrameIndex];
	float difference = nextRotationKeyFrame.time - rotationKeyFrame.time;
	float ratio = (keyFrameTime - rotationKeyFrame.time) / difference;

	if (ratio < 0.0f) ratio = 0.0f;
	if (ratio > 1.0f) ratio = 1.0f;

	glm::quat result;
	result = glm::slerp(rotationKeyFrame.Rotation, nextRotationKeyFrame.Rotation, ratio);
	result = glm::normalize(result);

	return result;
}

void Character::CastToGLM(const aiVector3D& in, glm::vec3& out)
{
	out.x = in.x;
	out.y = in.y;
	out.z = in.z;
}

void Character::CastToGLM(const aiQuaternion& in, glm::quat& out)
{
	out.w = in.w;
	out.x = in.x;
	out.y = in.y;
	out.z = in.z;
}

void Character::CastToGLM(const aiMatrix4x4& in, glm::mat4& out)
{
	out = glm::transpose(glm::make_mat4(&in.a1));
}
