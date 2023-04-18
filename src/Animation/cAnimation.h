#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>
#include <map>
#include <assimp/scene.h>
#include "../BoneHierarchy.h"


struct BoneInfo
{
	std::string name;
	glm::mat4 boneOffset;
	glm::mat4 globalTransformation;
	glm::mat4 finalTransformation; 
};

struct BoneVertexData
{
	BoneVertexData()
	{
		ids[0] = 0;
		ids[1] = 0;
		ids[2] = 0;
		ids[3] = 0;
		weights[0] = 0.f;
		weights[1] = 0.f;
		weights[2] = 0.f;
		weights[3] = 0.f;
	}

	unsigned int ids[4];
	float weights[4];

	void AddBoneInfo(int id, float weight)
	{
		int numIds = sizeof(ids) / sizeof(ids[0]);
		for (int i = 0; i < numIds; i++)
		{
			if (weights[i] == 0.f)
			{
				ids[i] = id;
				weights[i] = weight;
				return;
			}
		}

		assert(0);
	}
};

enum EasingType
{
	EaseIn,
	EaseOut,
	EaseInOut,
	None
};

struct PositionKeyFrame
{
	PositionKeyFrame()
		: Pos(0.f), time(0.f), type(None) { }
	PositionKeyFrame(glm::vec3 Pos, float time, EasingType type = None) : Pos(Pos), time(time), type(type) {}
	glm::vec3 Pos;
	float time;
	EasingType type;
	//int boneID;
};

struct ScaleKeyFrame
{
	ScaleKeyFrame()
		: Scale(0.f), time(0.f), type(None) { }
	ScaleKeyFrame(glm::vec3 Scale, float time, EasingType type = None) : Scale(Scale), time(time), type(type) {}
	glm::vec3 Scale;
	float time;
	EasingType type;
	//int boneID;
};

struct RotationKeyFrame
{
	RotationKeyFrame()
		: Rotation(1.0f, 0.f, 0.f, 0.f), time(0.f), useSlerp(true) { }
	RotationKeyFrame(glm::quat Rotation, float time, bool useSlerp = true) : Rotation(Rotation), time(time), useSlerp(useSlerp) {}
	glm::quat Rotation;
	float time;
	bool useSlerp;
	//EasingType type;
	//int boneID;
};

struct AnimationData
{
	AnimationData() {}
	std::vector<PositionKeyFrame> PositionKeyFrames;
	std::vector<ScaleKeyFrame> ScaleKeyFrames;
	std::vector<RotationKeyFrame> RotationKeyFrames; 
	std::string Name;
	float Duration;
	double TicksPerSecond;
};

struct CharacterAnimationData
{
	CharacterAnimationData(const aiScene* scene) : AIScene(scene) { }
	const aiScene* AIScene;

	BoneHierarchy* BoneHierarchy;			// Utilized for Animation 

	std::vector<BoneInfo> boneInfoVec;
	std::map<std::string, int> boneNameToIdMap;
	//cMeshObj* meshObj;
	std::vector<AnimationData*> Channels;
	std::string Name;
	double Duration;
	double TicksPerSecond;
};


class cAnimation
{
public:
	cAnimation() {}
	~cAnimation() {}

	std::vector<glm::mat4> GlobalTransformations;
	bool IsCharacterAnimation;
	std::string AnimationType;
	float AnimationTime;
	bool IsPlaying;
	bool IsLooping;
	float Speed;

	std::string tag;
	std::vector<std::string> seq;
	unsigned int curSeq;

};

