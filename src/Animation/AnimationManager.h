#pragma once
#include "cAnimation.h"
#include "../MeshObj/cMeshObj.h"
#include <map>

class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	bool AddAnimation(const std::string& name, AnimationData animation);
	//bool AddBoneAnimation(const std::string& name, BoneAnimationData boneanimation);
	void AnimationUpdate(bool& playCMD,float dt);
	//void BoneAnimationUpdate(bool& playCMD, float dt);
	void play(bool isPlay);
	void setSpeed(float speedX);
	void setSequence(unsigned int& sequence);
	void getSequence(unsigned int& sequence);
	//void AnimationUpdate(const std::vector<cMeshObj*>& animationOBJList, float dt);

	std::vector<cMeshObj*> animationOBJList;
	bool continuePlay;

private:
	int FindPositionKeyIndex(const AnimationData& animation, float time);
	int FindScaleKeyIndex(const AnimationData& animation, float time);
	int FindRotationKeyIndex(const AnimationData& animation, float time);

	glm::vec3 GetAnimationPosition(const AnimationData& animation, float time);
	glm::vec3 GetAnimationScale(const AnimationData& animation, float time);
	glm::quat GetAnimationRotation(const AnimationData& animation, float time);

	glm::mat4 CreateModelMatrix(const glm::mat4& parentModelMatrix, const glm::vec3& translate, const glm::vec3& scale, const glm::quat& rotate);

	std::map<std::string, AnimationData> AnimationList;
	//std::map<std::string, BoneAnimationData> BoneAnimationList;
	glm::mat4 m_GlobalInverseTransform;
};

