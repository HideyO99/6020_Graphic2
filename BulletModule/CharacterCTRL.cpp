#include "CharacterCTRL.h"

CharacterCTRL::CharacterCTRL(
	btPairCachingGhostObject* btGhostObject,
	btConvexShape* shape,
	btScalar stepHeight,
	const btVector3& up)
	: m_GhostObject(btGhostObject)
{
	m_CharacterController = new btKinematicCharacterController(
		m_GhostObject, shape, stepHeight, up);
	m_CharacterController->setUseGhostSweepTest(false);

	m_CharacterController->setMaxPenetrationDepth(0);
}

CharacterCTRL::~CharacterCTRL()
{
}

void CharacterCTRL::SetWalkDirection(const Vec3& walkDirection)
{
	btVector3 btWalkDirection;
	CastBulletVector3(walkDirection, &btWalkDirection);
	m_CharacterController->setWalkDirection(btWalkDirection);
}

void CharacterCTRL::SetVelocityForTimeInterval(const Vec3& velocity, float timeInterval)
{
	btVector3 btVelocity;
	CastBulletVector3(velocity, &btVelocity);
	m_CharacterController->setVelocityForTimeInterval(btVelocity, btScalar(timeInterval));
}

void CharacterCTRL::Warp(const Vec3& origin)
{
	btVector3 btOrigin;
	CastBulletVector3(origin, &btOrigin);
	m_CharacterController->warp(btOrigin);
}

bool CharacterCTRL::CanJump() const
{
	return m_CharacterController->canJump();
}

void CharacterCTRL::Jump(const Vec3& dir)
{
	btVector3 btDir;
	CastBulletVector3(dir, &btDir);
	m_CharacterController->jump(btDir);
}

bool CharacterCTRL::OnGround() const
{
	return m_CharacterController->onGround();
}