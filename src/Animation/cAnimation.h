#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>
#include <map>

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
	PositionKeyFrame(glm::vec3 Pos, float time, EasingType type = None, int boneID = 0) : Pos(Pos), time(time), type(type), boneID(boneID) {}
	glm::vec3 Pos;
	float time;
	EasingType type;
	int boneID;
};

struct ScaleKeyFrame
{
	ScaleKeyFrame(glm::vec3 Scale, float time, EasingType type = None, int boneID = 0) : Scale(Scale), time(time), type(type), boneID(boneID){}
	glm::vec3 Scale;
	float time;
	EasingType type;
	int boneID;
};

struct RotationKeyFrame
{
	RotationKeyFrame(glm::quat Rotation, float time, EasingType type = None, int boneID = 0) : Rotation(Rotation), time(time), type(type), boneID(boneID){}
	glm::quat Rotation;
	float time;
	//bool useSlerp;
	EasingType type;
	int boneID;
};

struct AnimationData
{
	std::vector<PositionKeyFrame> PositionKeyFrames;
	std::vector<ScaleKeyFrame> ScaleKeyFrames;
	std::vector<RotationKeyFrame> RotationKeyFrames;
	float Duration;
};

//struct BoneAnimationData
//{
//	std::string Name;
//
//	BoneHierarchy* boneHierarchy;
//	
//	std::vector<BoneInfo> boneInfoVec;
//	std::map<std::string, int> boneNameToIdMap;
//
//	std::vector<AnimationData*> Channels;
//	//std::vector<PositionKeyFrame> PositionKeyFrames;
//	//std::vector<ScaleKeyFrame> ScaleKeyFrames;
//	//std::vector<RotationKeyFrame> RotationKeyFrames;
//	float Duration;
//};


class cAnimation
{
public:
	cAnimation();
	~cAnimation();

	std::string tag;
	std::vector<std::string> seq;
	unsigned int curSeq;
	float AnimationTime;
	bool IsPlaying;
	bool IsLooping;
	float Speed;


};

