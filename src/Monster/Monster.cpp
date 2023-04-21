#include "Monster.h"

Monster::Monster()
{
	alive = true;
}

Monster::~Monster()
{
}

void Monster::update(double dt)
{
	//charAnimate->SetAnimation(0);
	printf("Character: %d , ", this->id);
	this->charAnimate->UpdateTransforms(this->meshObj->BoneModelMatrices, this->meshObj->GlobalTransformations, dt);
}

void Monster::calWorldPos()
{
}

void Monster::ProcessMove()
{
}

void Monster::rotate()
{
}

void Monster::attack()
{
	charAnimate->SetAnimation(1);
}

void Monster::dead()
{
	charAnimate->SetAnimation(2);
}
