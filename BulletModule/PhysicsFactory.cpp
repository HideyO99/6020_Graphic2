#include "PhysicsFactory.h"
#include "RigidBody.h"
#include "Conversion.h"
#include "CharacterCTRL.h"
#include "Vehicle.h"
#include "PhysicsWorld.h"

#include <bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

PhysicsFactory::PhysicsFactory()
	: iPhysicsFactory()
{
	printf("Created Bullet Physics Factory!\n");
}

PhysicsFactory::~PhysicsFactory()
{
}

iPhysicsWorld* PhysicsFactory::CreateWorld()
{
	PhysicsWorld* world = new PhysicsWorld();

	m_VehicleRaycaster = world->GetVehicleRaycaster();
	m_DynamicsWorld = world->GetDynamicsWorld();
	return world;
}

iRigidBody* PhysicsFactory::CreateRigidBody(const RigidBodyDesc& desc, iShape* shape)
{
	return new RigidBody(desc, shape);
}

iVehicle* PhysicsFactory::CreateVehicle(iRigidBody* chassis)
{
	// We may need to create our Tuning values here
	btRigidBody* btChassis = CastBulletRigidBody(chassis);
	return new Vehicle(btChassis, m_VehicleRaycaster);
}

iCharacterCTRL* PhysicsFactory::CreateCharacterController(
	iConvexShape* shape, float stepHeight, const Vec3& up)
{
	btPairCachingGhostObject* btGhostObject = new btPairCachingGhostObject();
	btConvexShape* btShape;
	btScalar btStepHeight;
	btVector3 btUp;

	btShape = CastBulletConvexShape(shape);
	CastBulletScalar(stepHeight, &btStepHeight);
	CastBulletVector3(up, &btUp);

	iCharacterCTRL* cc = new CharacterCTRL(btGhostObject, btShape, btStepHeight, btUp);

	// Do this after Creating the Character Controller
	btGhostObject->setCollisionShape(btShape);
	btGhostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-5, 5, -5)));

	return cc;
}