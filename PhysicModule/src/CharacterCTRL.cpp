#include "CharacterCTRL.h"
#include <iCylinderShape.h>
#include "PhysicWorld.h"

CharacterCTRL::CharacterCTRL(iShape* shape, const Vec3& position, const Quat& rotation)
{
	physx::PxControllerDesc* desc;
	if (iCylinderShape* cylinder = (iCylinderShape*)shape)
	{
		physx::PxCapsuleControllerDesc capsuleDesc;
		float rad = cylinder->getHalfExtents().x;
		capsuleDesc.height = (cylinder->getHalfExtents().y * 2.f) - (rad * 2.f);
		capsuleDesc.radius = rad;
		desc = &capsuleDesc;
		desc->userData = (void*)capsuleDesc.userData;
	}
	else 
	{
		printf("error: create Character controller\n");
		return;
	}

	desc->position = physx::PxExtendedVec3(position.x, position.y, position.z);
	desc->upDirection = physx::PxVec3(0, 1, 0);
	desc->slopeLimit = 0.707f; //45 degrees
	desc->invisibleWallHeight = 2.f;
	desc->maxJumpHeight = 2.0f;
	desc->contactOffset = 0.1f;
	desc->stepOffset = 0.1f;
	desc->upDirection = physx::PxVec3(0.f,1.f,0.f);
	desc->scaleCoeff = 0.9f;
	desc->material = PhysicWorld::m_Material;
	m_Controller = PhysicWorld::m_ControllerManager->createController(*desc);
	PhysicWorld::addControllerToMap(this, m_Controller);
	SetRotation(rotation);
	m_Gravity = new Vec3(0.f, -9.8f, 0.f);
	m_Velocity = new Vec3(0.f);
}


CharacterCTRL::~CharacterCTRL()
{
	m_Controller->release();
}

void CharacterCTRL::SetWalkDirection(const Vec3& walkDirection)
{
	m_Velocity->x += m_Gravity->x;
	m_Velocity->y += m_Gravity->y;
	m_Velocity->z += m_Gravity->z;

	physx::PxVec3 dir = physx::PxVec3(walkDirection.x, m_Velocity->y + walkDirection.y, walkDirection.z);
	physx::PxControllerFilters filters;
	physx::PxControllerCollisionFlags collisionFlags =m_Controller->move(dir, 0.01f, 1.0f / 60.0f, filters);
	if (collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN) 
	{
		m_Velocity->y = 0.0f;
	}
}

void CharacterCTRL::SetPosition(const Vec3& position)
{
	m_Controller->setPosition(physx::PxExtendedVec3(position.x, position.y, position.z));
}

void CharacterCTRL::GetPosition(Vec3& position) const
{
	physx::PxExtendedVec3 pxPos = m_Controller->getPosition();
	position.x = (float)pxPos.x;
	position.y = (float)pxPos.y;
	position.z = (float)pxPos.z;
}

void CharacterCTRL::SetRotation(const Quat& rotation)
{
	physx::PxQuat pxRotation(rotation.x, rotation.y, rotation.z, rotation.w);
	physx::PxTransform transform(m_Controller->getActor()->getGlobalPose());
	transform.q = pxRotation;
	m_Controller->getActor()->setGlobalPose(transform);
}

void CharacterCTRL::GetRotation(Quat& rotation) const
{
	physx::PxQuat pxRotate = m_Controller->getActor()->getGlobalPose().q;
	rotation.x = pxRotate.x;
	rotation.y = pxRotate.y;
	rotation.z = pxRotate.z;
	rotation.w = pxRotate.w;
}

void CharacterCTRL::SetGravity(const Vec3& gravity)
{
	*m_Gravity = gravity;
}
