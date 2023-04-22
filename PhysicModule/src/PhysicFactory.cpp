#include "PhysicFactory.h"

PhysicFactory::PhysicFactory()
	: iPhysicFactory()
{
}

PhysicFactory::~PhysicFactory()
{
}

iPhysicWorld* PhysicFactory::createWorld()
{
	/*phyworld = new PhysicWorld();*/
	return new PhysicWorld();
}

iRigidBody* PhysicFactory::createRigidBody(const iRigidBodyDesc& desc, iShape* shape)
{
	return new RigidBody(desc, shape);
}

iCharacterCTRL* PhysicFactory::CreateCharacterController(iShape* shape, Vec3& pos, const Quat& rotate)
{
	//charCTRL = new CharacterCTRL(shape, pos, rotate);
	//charCTRL = this->phyworld;
	return new CharacterCTRL(shape, pos, rotate);
}
