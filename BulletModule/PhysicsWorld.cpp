#include "PhysicsWorld.h"
#include "Conversion.h"

#include <bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h>
#include <bullet/BulletDynamics/Character/btCharacterControllerInterface.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include "CharacterCTRL.h"

PhysicsWorld::PhysicsWorld()
	: iPhysicsWorld()
{
	m_CollisionConfiguration = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
	m_Broadphase = new btDbvtBroadphase();
	m_Solver = new btSequentialImpulseConstraintSolver;
	m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher,
		m_Broadphase, m_Solver, m_CollisionConfiguration);

	m_DynamicsWorld->setDebugDrawer(m_DebugDrawer);
	int debugMode = 1;
	m_DynamicsWorld->getDebugDrawer()->setDebugMode(debugMode);

	m_VehicleRaycaster = new btDefaultVehicleRaycaster(m_DynamicsWorld);
	m_OverlappingPairCallback = new OverlappingPairCallback();

	m_DynamicsWorld->getPairCache()->setInternalGhostPairCallback(m_OverlappingPairCallback);
}

PhysicsWorld::~PhysicsWorld()
{
	delete m_DynamicsWorld;
	delete m_CollisionConfiguration;
	delete m_Dispatcher;
	delete m_Broadphase;
	delete m_Solver;
}

void PhysicsWorld::SetGravity(const Vec3& gravity)
{
	btVector3 btGravity;
	CastBulletVector3(gravity, &btGravity);
	m_DynamicsWorld->setGravity(btGravity);
}

void PhysicsWorld::AddBody(iCollisionBody* body)
{
	btRigidBody* bulletBody = CastBulletRigidBody(body);
	m_DynamicsWorld->addRigidBody(bulletBody);
}

void PhysicsWorld::RemoveBody(iCollisionBody* body)
{
	btRigidBody* bulletBody = CastBulletRigidBody(body);
	m_DynamicsWorld->removeRigidBody(bulletBody);
}

void PhysicsWorld::AddVehicle(iVehicle* vehicle)
{
	btRaycastVehicle* bulletVehicle = CastBulletVehicle(vehicle);
	m_DynamicsWorld->addAction(bulletVehicle);
}

void PhysicsWorld::RemoveVehicle(iVehicle* vehicle)
{
	btRaycastVehicle* bulletVehicle = CastBulletVehicle(vehicle);
	m_DynamicsWorld->removeAction(bulletVehicle);
}


void PhysicsWorld::AddCharacterController(iCharacterCTRL* characterController)
{
	btCharacterControllerInterface* btCharacterController =
		CastBulletCharacterController(characterController);
	CharacterCTRL* cc = dynamic_cast<CharacterCTRL*>(characterController);

	btPairCachingGhostObject* btGhostObject = cc->GetGhostObject();
	m_DynamicsWorld->addCollisionObject(btGhostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	m_DynamicsWorld->addAction(btCharacterController);
}

void PhysicsWorld::RemoveCharacterController(iCharacterCTRL* characterController)
{
	CharacterCTRL* cc = dynamic_cast<CharacterCTRL*>(characterController);
	btPairCachingGhostObject* btGhostObject = cc->GetGhostObject();
	btCharacterControllerInterface* btCharacterController =
		CastBulletCharacterController(characterController);
	m_DynamicsWorld->removeAction(btCharacterController);
	m_DynamicsWorld->removeCollisionObject(btGhostObject);

}

void PhysicsWorld::AddConstraint(iConstraint* constraint)
{
	btTypedConstraint* btConstraint = CastBulletConstraint(constraint);
	m_DynamicsWorld->addConstraint(btConstraint);
}

void PhysicsWorld::RemoveConstraint(iConstraint* constraint)
{
	btTypedConstraint* btConstraint = CastBulletConstraint(constraint);
	m_DynamicsWorld->removeConstraint(btConstraint);
}


void PhysicsWorld::TimeStep(float dt)
{
	m_DynamicsWorld->stepSimulation(dt);
}

btDefaultVehicleRaycaster* PhysicsWorld::GetVehicleRaycaster()
{
	return m_VehicleRaycaster;
}