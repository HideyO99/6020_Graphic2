#include "Monster.h"

Monster::Monster()
{
	alive = true;
	task = new stateMachine();
	
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
	//task->SetState(new PursueState(0));
}

void Monster::rotate()
{
}

void Monster::attack()
{
	task->SetState(new AttackState(0));
}

void Monster::dead()
{
	task->SetState(new DeadState());
}
