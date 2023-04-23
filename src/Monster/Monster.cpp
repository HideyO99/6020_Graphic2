#include "Monster.h"

Monster::Monster()
{
	alive = true;
	task = new stateMachine();
	velocity = glm::vec3(1, 0, 1);
	dir = glm::vec3(0, 0, 1);
	targetPos = glm::vec3(0);
}

Monster::~Monster()
{
}

void Monster::update(double dt)
{
	//charAnimate->SetAnimation(0);
	printf("Character: %d , ", this->id);
	this->charAnimate->UpdateTransforms(this->meshObj->BoneModelMatrices, this->meshObj->GlobalTransformations, dt);
	
	if (task->GetCurrentState()->GetType() == Dead)
	{
		return;
	}

	if (task->GetCurrentState()->GetType() == Pursue)
	{
		ProcessMove();
	}
}

void Monster::calWorldPos()
{
	const float TILESIZE = 10.0f;  // How big each tile is in the world (each grid is 10 units)
	const float offset = -5.0f;  // 
	// Each mesh is 500x500 but that's too big
	//const float MESHTOWORLDSCALE = TILESIZE / 5.f;

	float cellXLocation = ((PosCol * TILESIZE) + offset);
	float cellYLocation = ((PosRow * TILESIZE) + offset);
	//todo
	this->meshObj->position.x = cellXLocation;
	this->meshObj->position.y = 3.f;
	this->meshObj->position.z = cellYLocation;
	//this->meshObj->scale = glm::vec3(MESHTOWORLDSCALE);
}

void Monster::calRLPos()
{
	const float TILESIZE = 10.0f;  // How big each tile is in the world (each grid is 10 units)
	const float offset = -5.0f;  // 
	// Each mesh is 500x500 but that's too big
	//const float MESHTOWORLDSCALE = TILESIZE / 5.f;

	float cellXLocation = this->meshObj->position.x;
	float cellYLocation = this->meshObj->position.z;

	this->PosCol = (cellXLocation - offset) / TILESIZE;
	this->PosRow = (cellYLocation - offset) / TILESIZE;

}

void Monster::ProcessMove()
{
	//float step = 0.1f;
	task->SetState(new PursueState(0));
	//steering->Seek(targetPos);
	//meshObj->position += dir * step;
	//meshObj->rotation.y = asin(dir.x);
	//calRLPos();
}

void Monster::rotate()
{
	task->SetState(new IdleState(glm::vec3(0),1));
}

void Monster::attack()
{
	task->SetState(new AttackState(0));
}

void Monster::dead()
{
	task->SetState(new DeadState());
}
