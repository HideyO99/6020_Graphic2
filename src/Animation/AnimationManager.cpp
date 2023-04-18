#include "AnimationManager.h"
#include <glm/gtx/easing.hpp>
#include <iostream>
#include <glm/gtx/matrix_decompose.hpp>

AnimationManager::AnimationManager()
{
	animationOBJList.empty();
	continuePlay = false;
}

AnimationManager::~AnimationManager()
{
}

bool AnimationManager::AddAnimation(const std::string& name, AnimationData animation)
{
	std::map<std::string, AnimationData>::iterator itFind = AnimationList.find(name);
	if (itFind != AnimationList.end())
	{
		std::cout << "Cannot add " << name << std::endl;
		return false;
	}

	AnimationList.emplace(name, animation); 

	return true;
}

bool AnimationManager::LoadCharacterAnimation(const std::string& name, CharacterAnimationData animation)
{
	std::map<std::string, CharacterAnimationData>::iterator itFind = m_CharacterAnimationsList.find(name);
	if (itFind != m_CharacterAnimationsList.end())
	{
		return false;
	}

	m_CharacterAnimationsList.emplace(name, animation);

	return true;
}


void AnimationManager::AnimationUpdate(bool& playCMD,float dt)
{
	bool isPlaying = false;

	for (size_t i = 0; i < animationOBJList.size(); i++)
	{
		cMeshObj* AnimationOBJ = animationOBJList[i];
		if (AnimationOBJ->Animation.IsCharacterAnimation)
		{
			cAnimation& animation = AnimationOBJ->Animation;
			std::map<std::string, CharacterAnimationData>::iterator charAnimation_it = m_CharacterAnimationsList.find(animation.AnimationType);
			CharacterAnimationData& animationData = charAnimation_it->second;
			if (charAnimation_it != m_CharacterAnimationsList.end())
			{
				if (animation.IsPlaying && animation.Speed != 0.0f)
				{
					animation.AnimationTime += dt * animation.Speed * animationData.TicksPerSecond;

					double clipDuration = animationData.Duration;

					if (animation.AnimationTime > clipDuration)
					{
						if (animation.IsLooping)
						{
							if (animation.Speed > 0)
							{
								animation.AnimationTime = 0.0f;
							}
							else
							{
								animation.AnimationTime = clipDuration;
							}
						}
						else
						{
							animation.AnimationTime = clipDuration;
							animation.IsPlaying = false;
						}

					}
					else if (animation.AnimationTime < 0.f)
					{
						if (animation.IsLooping)
						{
							if (animation.Speed < 0)
							{
								animation.AnimationTime = animationData.Duration;
							}
							else
							{
								animation.AnimationTime = 0.f;
							}
						}
						else
						{
							animation.AnimationTime = 0.f;
							animation.IsPlaying = false;
						}
					}

					m_GlobalInverseTransform = animationData.BoneHierarchy->globalInverseTransform;

					int keyFrameTime = (int)((animation.AnimationTime / clipDuration) * animationData.Duration);


					animation.AnimationTime = 0;
					glm::mat4 identity(1.f);

					//printf("--------------------\n");
					//printf("Time: %.4f %d/%d\n", animation.AnimationTime, keyFrameTime, (int)animationData.Duration);
					UpdateBoneHierarchy(animationData.BoneHierarchy->root, animationData, identity, animation.AnimationTime);

					//Model* model = GDP_GetModel(go->Renderer.MeshId);

					SetGameObjectBoneModelMatrices(AnimationOBJ, animationData.BoneHierarchy->root, animationData);
				}

			}
		}
		else 
		{
			cAnimation& animation = AnimationOBJ->Animation;
			std::map<std::string, AnimationData>::iterator animation_it = AnimationList.find(animation.tag);

			if (animation_it != AnimationList.end())
			{
				const AnimationData& animationData = animation_it->second;

				if (animation.IsPlaying && animation.Speed != 0.f)
				{
					isPlaying = true;
					animation.AnimationTime += dt * animation.Speed;
					if (animation.AnimationTime > animationData.Duration)
					{
						if (animation.IsLooping)
						{
							if (animation.Speed > 0)
							{
								animation.AnimationTime = 0.0f;
							}
							else
							{
								animation.AnimationTime = animationData.Duration;
							}

						}
						else if (continuePlay)
						{
							unsigned nextSeq = animation.curSeq + 1;
							if (nextSeq < animation.seq.size())
							{
								animation.tag = animation.seq[nextSeq].c_str();
								animation.AnimationTime = 0.0f;
								animation.curSeq++;
								std::cout << "play animation sequence " << animation.curSeq + 1 << std::endl;
								continue;
							}
							else
							{
								animation.AnimationTime = animationData.Duration;
								animation.IsPlaying = false;
								isPlaying = false;
							}
						}
						else
						{
							animation.AnimationTime = animationData.Duration;
							animation.IsPlaying = false;
							isPlaying = false;

						}
					}
					else if (animation.AnimationTime < 0.f)
					{
						if (animation.IsLooping)
						{
							if (animation.Speed < 0)
							{
								animation.AnimationTime = animationData.Duration;
							}
							else
							{
								animation.AnimationTime = 0.f;
							}

						}
						else if (continuePlay)
						{
							unsigned nextSeq = animation.curSeq - 1;
							if (nextSeq < animation.seq.size())
							{
								animation.tag = animation.seq[nextSeq].c_str();
								animation.AnimationTime = animationData.Duration;
								animation.curSeq--;
								std::cout << "play animation sequence " << animation.curSeq + 1 << std::endl;
								continue;
							}
							else
							{
								animation.AnimationTime = 0.f;
								animation.IsPlaying = false;
								isPlaying = false;
							}
						}
						else
						{
							animation.AnimationTime = 0.f;
							animation.IsPlaying = false;
							isPlaying = false;
						}
					}
					for (int i = 0; i < animationData.PositionKeyFrames.size(); i++)
					{
						//int boneID = animationData.PositionKeyFrames[i].boneID;
						glm::vec3 bonePos = GetAnimationPosition(animationData, animation.AnimationTime);
						glm::vec3 boneScale = GetAnimationScale(animationData, animation.AnimationTime);
						glm::quat boneRotate = glm::eulerAngles(GetAnimationRotation(animationData, animation.AnimationTime));
						//AnimationOBJ->BoneModelMatrices[boneID] = CreateModelMatrix(glm::mat4(1.f), bonePos, boneScale, boneRotate);

					}
					//AnimationOBJ->position = GetAnimationPosition(animationData, animation.AnimationTime);
					//AnimationOBJ->scale = GetAnimationScale(animationData, animation.AnimationTime);
					//AnimationOBJ->rotation = glm::eulerAngles(GetAnimationRotation(animationData, animation.AnimationTime));
				}
			}
		}
	}
	playCMD = isPlaying;

}

//void AnimationManager::BoneAnimationUpdate(bool& playCMD, float dt)
//{
//	//todo
//	bool isPlaying = false;
//
//	for (size_t i = 0; i < animationOBJList.size(); i++)
//	{
//		cMeshObj* AnimationOBJ = animationOBJList[i];
//
//		cAnimation& animation = AnimationOBJ->Animation;
//		std::map<std::string, AnimationData>::iterator animation_it = AnimationList.find(animation.tag);
//
//		if (animation_it != AnimationList.end())
//		{
//			const AnimationData& animationData = animation_it->second;
//
//			if (animation.IsPlaying && animation.Speed != 0.f)
//			{
//				isPlaying = true;
//				animation.AnimationTime += dt * animation.Speed;
//				if (animation.AnimationTime > animationData.Duration)
//				{
//					if (animation.IsLooping)
//					{
//						if (animation.Speed > 0)
//						{
//							animation.AnimationTime = 0.0f;
//						}
//						else
//						{
//							animation.AnimationTime = animationData.Duration;
//						}
//
//					}
//					else if (continuePlay)
//					{
//						unsigned nextSeq = animation.curSeq + 1;
//						if (nextSeq < animation.seq.size())
//						{
//							animation.tag = animation.seq[nextSeq].c_str();
//							animation.AnimationTime = 0.0f;
//							animation.curSeq++;
//							std::cout << "play animation sequence " << animation.curSeq + 1 << std::endl;
//							continue;
//						}
//						else
//						{
//							animation.AnimationTime = animationData.Duration;
//							animation.IsPlaying = false;
//							isPlaying = false;
//						}
//					}
//					else
//					{
//						animation.AnimationTime = animationData.Duration;
//						animation.IsPlaying = false;
//						isPlaying = false;
//
//					}
//				}
//				else if (animation.AnimationTime < 0.f)
//				{
//					if (animation.IsLooping)
//					{
//						if (animation.Speed < 0)
//						{
//							animation.AnimationTime = animationData.Duration;
//						}
//						else
//						{
//							animation.AnimationTime = 0.f;
//						}
//
//					}
//					else if (continuePlay)
//					{
//						unsigned nextSeq = animation.curSeq - 1;
//						if (nextSeq < animation.seq.size())
//						{
//							animation.tag = animation.seq[nextSeq].c_str();
//							animation.AnimationTime = animationData.Duration;
//							animation.curSeq--;
//							std::cout << "play animation sequence " << animation.curSeq + 1 << std::endl;
//							continue;
//						}
//						else
//						{
//							animation.AnimationTime = 0.f;
//							animation.IsPlaying = false;
//							isPlaying = false;
//						}
//					}
//					else
//					{
//						animation.AnimationTime = 0.f;
//						animation.IsPlaying = false;
//						isPlaying = false;
//					}
//				}
//
//				AnimationOBJ->position = GetAnimationPosition(animationData, animation.AnimationTime);
//				AnimationOBJ->scale = GetAnimationScale(animationData, animation.AnimationTime);
//				AnimationOBJ->rotation = glm::eulerAngles(GetAnimationRotation(animationData, animation.AnimationTime));
//			}
//		}
//	}
//	playCMD = isPlaying;
//}

void AnimationManager::play(bool isPlay)
{
	for (size_t i = 0; i < animationOBJList.size(); i++)
	{
		cMeshObj* AnimationOBJ = animationOBJList[i];

		cAnimation& animation = AnimationOBJ->Animation;
		animation.IsPlaying = isPlay;
	}
}

void AnimationManager::setSpeed(float speedX)
{
	for (size_t i = 0; i < animationOBJList.size(); i++)
	{
		cMeshObj* AnimationOBJ = animationOBJList[i];

		cAnimation& animation = AnimationOBJ->Animation;
		animation.Speed = speedX;
	}
}

void AnimationManager::setSequence(unsigned int& sequence)
{
	for (size_t i = 0; i < animationOBJList.size(); i++)
	{
		cMeshObj* AnimationOBJ = animationOBJList[i];

		cAnimation& animation = AnimationOBJ->Animation;
		if (sequence < animation.seq.size())
		{
			animation.tag = animation.seq[sequence].c_str();
			animation.AnimationTime = 0.f;
			animation.curSeq = sequence;
		}
		else
		{
			sequence = animation.seq.size() - 1;
		}
	}
}

void AnimationManager::getSequence(unsigned int& sequence)
{
	sequence = animationOBJList[0]->Animation.curSeq;
}

void AnimationManager::UpdateBoneHierarchy(BoneNode* node, CharacterAnimationData& data, const glm::mat4& parentTransformationMatrix, float keyFrameTime)
{
	if (node == nullptr)
		return;

	std::string nodeName(node->name);
	glm::mat4 transformationMatrix = node->transformation;

	AnimationData* animNode = FindAnimationData(nodeName, data);

	if (animNode != nullptr)
	{
		// Calculate the position of this node.
		glm::vec3 position = GetAnimationPosition(*animNode, keyFrameTime);
		glm::vec3 scale = GetAnimationScale(*animNode, keyFrameTime);
		glm::quat rotation = GetAnimationRotation(*animNode, keyFrameTime);

		// Calculate our transformation matrix
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

	}

	glm::mat4 globalTransformation = parentTransformationMatrix * transformationMatrix;

	// Store the transformation matrix in the Animation component info.
	//Check if this is a bone, and get the bone offset
	std::map<std::string, int>::iterator boneIt = data.boneNameToIdMap.find(nodeName);
	if (boneIt != data.boneNameToIdMap.end())
	{
		int boneIdx = boneIt->second;
		BoneInfo& bi = data.boneInfoVec[boneIdx];
		bi.finalTransformation = m_GlobalInverseTransform * globalTransformation * bi.boneOffset; // m_GlobalInverseTransform* globalTransformation* bi.boneOffset;
	}

	glm::vec3 globalScale, globalTranslation, ignore3;
	glm::vec4 ignore4;
	glm::quat globalOrientation;

	bool success = glm::decompose(globalTransformation, globalScale, globalOrientation, globalTranslation, ignore3, ignore4);

	for (int i = 0; i < node->children.size(); i++)
	{
		UpdateBoneHierarchy(node->children[i], data, globalTransformation, keyFrameTime);
	}
}

void AnimationManager::SetGameObjectBoneModelMatrices(cMeshObj* meshObj, BoneNode* node, const CharacterAnimationData& data)
{
	meshObj->BoneModelMatrices.resize(data.boneInfoVec.size());
	for (int i = 0; i < data.boneInfoVec.size(); i++)
	{
		meshObj->BoneModelMatrices[i] = data.boneInfoVec[i].finalTransformation;
	}
}

AnimationData* AnimationManager::FindAnimationData(const std::string& nodeName, const CharacterAnimationData& data)
{
	for (int i = 0; i < data.Channels.size(); i++)
	{
		if (nodeName == data.Channels[i]->Name)
			return data.Channels[i];
	}
	return nullptr;
}

int AnimationManager::FindPositionKeyIndex(const AnimationData& animation, float time)
{
	for (size_t i = 0; i < animation.PositionKeyFrames.size(); i++)
	{
		if (animation.PositionKeyFrames[i].time > time) 
		{
			return i - 1;
		}
	}

	return animation.PositionKeyFrames.size() - 1;
}

int AnimationManager::FindScaleKeyIndex(const AnimationData& animation, float time)
{
	for (size_t i = 0; i < animation.ScaleKeyFrames.size(); i++)
	{
		if (animation.ScaleKeyFrames[i].time > time)
		{
			return i - 1;
		}
	}

	return animation.ScaleKeyFrames.size() - 1;
}

int AnimationManager::FindRotationKeyIndex(const AnimationData& animation, float time)
{
	for (size_t i = 0; i < animation.RotationKeyFrames.size(); i++)
	{
		if (animation.RotationKeyFrames[i].time > time)
		{
			return i - 1;
		}
	}

	return animation.RotationKeyFrames.size() - 1;
}

glm::vec3 AnimationManager::GetAnimationPosition(const AnimationData& animation, float time)
{
	int currentPosKFIndex = FindPositionKeyIndex(animation, time);
	if ((animation.PositionKeyFrames.size() == 1)|| (currentPosKFIndex == animation.PositionKeyFrames.size() - 1))
	{
		return animation.PositionKeyFrames[currentPosKFIndex].Pos;
	}

	int nextPosKFIndex = currentPosKFIndex + 1;
	PositionKeyFrame currentKF = animation.PositionKeyFrames[currentPosKFIndex];
	PositionKeyFrame nextKF = animation.PositionKeyFrames[nextPosKFIndex];
	float diff = nextKF.time - currentKF.time;
	float ratio = (time - currentKF.time) / diff;

	switch (currentKF.type)
	{
	case EaseIn:
		ratio = glm::sineEaseIn(ratio);
		break;
	case EaseOut:
		ratio = glm::sineEaseOut(ratio);
		break;
	case EaseInOut:
		ratio = glm::sineEaseInOut(ratio);
		break;
	case None:
		break;
	default:
		break;
	}

	return glm::mix(currentKF.Pos, nextKF.Pos, ratio);
}

glm::vec3 AnimationManager::GetAnimationScale(const AnimationData& animation, float time)
{
	int currentScaleKFIndex = FindScaleKeyIndex(animation, time);
	if ((animation.ScaleKeyFrames.size() == 1) || (currentScaleKFIndex == animation.ScaleKeyFrames.size() - 1))
	{
		return animation.ScaleKeyFrames[currentScaleKFIndex].Scale;
	}

	int nextScaleKFIndex = currentScaleKFIndex + 1;
	ScaleKeyFrame currentKF = animation.ScaleKeyFrames[currentScaleKFIndex];
	ScaleKeyFrame nextKF = animation.ScaleKeyFrames[nextScaleKFIndex];
	float diff = nextKF.time - currentKF.time;
	float ratio = (time - currentKF.time) / diff;

	switch (currentKF.type)
	{
	case EaseIn:
		ratio = glm::sineEaseIn(ratio);
		break;
	case EaseOut:
		ratio = glm::sineEaseOut(ratio);
		break;
	case EaseInOut:
		ratio = glm::sineEaseInOut(ratio);
		break;
	case None:
		break;
	default:
		break;
	}

	return glm::mix(currentKF.Scale, nextKF.Scale, ratio);
}

glm::quat AnimationManager::GetAnimationRotation(const AnimationData& animation, float time)
{
	int currentRotatationKFIndex = FindRotationKeyIndex(animation, time);
	if ((animation.RotationKeyFrames.size() == 1) || (currentRotatationKFIndex == animation.RotationKeyFrames.size() - 1))
	{
		return animation.RotationKeyFrames[currentRotatationKFIndex].Rotation;
	}

	int nextRotatationKFIndex = currentRotatationKFIndex + 1;
	RotationKeyFrame currentKF = animation.RotationKeyFrames[currentRotatationKFIndex];
	RotationKeyFrame nextKF = animation.RotationKeyFrames[nextRotatationKFIndex];
	float diff = nextKF.time - currentKF.time;
	float ratio = (time - currentKF.time) / diff;

	if (ratio < 0.0f) ratio = 0.0f;
	if (ratio > 1.0f) ratio = 1.0f;

	glm::quat result;
	result = glm::slerp(currentKF.Rotation, nextKF.Rotation, ratio);
	result = glm::normalize(result);

	return result;
}

glm::mat4 AnimationManager::CreateModelMatrix(const glm::mat4& parentModelMatrix, const glm::vec3& translate, const glm::vec3& scale, const glm::quat& rotate)
{
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.f), translate);
	glm::mat4 RotationMatrix = glm::mat4_cast(rotate);
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.f), scale);
	return parentModelMatrix * TranslationMatrix * RotationMatrix * ScaleMatrix;
}
