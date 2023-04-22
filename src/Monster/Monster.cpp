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
	const float TILESIZE = 10.0f;  // How big each tile is in the world (each grid is 10 units)
	const float offset = -5.0f;  // 
	// Each mesh is 500x500 but that's too big
	const float MESHTOWORLDSCALE = TILESIZE / 5.f;

	float cellXLocation = ((PosCol * TILESIZE) + offset);
	float cellYLocation = ((PosRow * TILESIZE) + offset);
	//todo
	this->meshObj->position.x = cellXLocation;
	this->meshObj->position.y = 3.f;
	this->meshObj->position.z = cellYLocation;
	//this->meshObj->scale = glm::vec3(MESHTOWORLDSCALE);
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
