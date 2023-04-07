#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct BoneNode
{
	void AddChild(BoneNode* child)
	{
		child->parent = this;
		children.push_back(child);
	}

	int boneId;
	std::string name;
	BoneNode* parent;
	glm::mat4 transformation;
	glm::mat4 finalTransformation;
	std::vector<BoneNode*> children;
};

struct BoneHierarchy
{
	BoneNode* root;
	glm::mat4 globalInverseTransform;
};