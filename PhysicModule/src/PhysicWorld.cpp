#include "PhysicWorld.h"

PhysicWorld::PhysicWorld()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
	if (!m_Foundation)
	{
		throw("PxCreateFoundation failes!");
	}

	m_Pvd = PxCreatePvd(*m_Foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	m_ToleranceScale.length = 100;
	m_ToleranceScale.speed = 981;
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd);
	if (!m_Physics)
	{
		throw("PxCreatePhysics failed!");
	}
	physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f);
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);
	
	//PVD
	m_PVDClient = m_Scene->getScenePvdClient();
	if (m_PVDClient)
	{
		m_PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		m_PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		m_PVDClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// Extensions
	if (!PxInitExtensions(*m_Physics, m_Pvd))
	{
		throw("PxInitExtensions failed!");
	}

	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
}

PhysicWorld::~PhysicWorld()
{
	m_Physics->release();
	m_Foundation->release();

}

void PhysicWorld::setGravity(const Vec3& gravity)
{
	//m_gravity = gravity;
}

void PhysicWorld::addBody(iCollision* body)
{
	if (body == nullptr)
	{
		return;
	}
	
	switch (body->getBodyType())
	{
	case BodyType::RigidBody:
		addRigid(body);
		break;
	default:
		break;
	}
}

void PhysicWorld::removeBody(iCollision* body)
{
	if (body == nullptr)
	{
		return;
	}

	switch (body->getBodyType())
	{
	case BodyType::RigidBody:
		removeRigid(body);
		break;
	default:
		break;
	}
	
}

void PhysicWorld::AddCharacterController(iCharacterCTRL* characterController)
{
	if (characterController == nullptr)
		return;
	CharacterCTRL* character = (CharacterCTRL*)characterController;
	m_CharacterControllers.push_back(character);
}

void PhysicWorld::RemoveCharacterController(iCharacterCTRL* characterController)
{
	std::map<iCharacterCTRL*, physx::PxController*>::iterator controllerIT = m_MapControllers.find(characterController);
	if (controllerIT != m_MapControllers.end())
	{
		physx::PxController* pxController = controllerIT->second;

		pxController->release();
		m_MapControllers.erase(characterController);
		for (int i = 0; i < m_CharacterControllers.size(); i++)
		{
			if (m_CharacterControllers[i] == characterController)
			{
				m_CharacterControllers.erase(m_CharacterControllers.begin() + i);
				break;
			}
		}

	}


}

void PhysicWorld::timeStep(float dt)
{
	m_Scene->simulate(dt);
	m_Scene->fetchResults(true);

}

void PhysicWorld::addToCollisionListener(iCollisionListener* listener)
{
	m_collisionListener = listener;
}

void PhysicWorld::resetWorld()
{
	for (int i = 0; i < m_actor.size(); i++)
	{
		std::map<physx::PxRigidActor*, physx::PxTransform>::iterator it = m_mapTransform.find(m_actor[i]->pActor);
		physx::PxTransform transform = it->second;
		if (!m_actor[i]->isStatic())
		{
			physx::PxRigidDynamic* dynamicActor = (physx::PxRigidDynamic*)m_actor[i]->pActor;
			dynamicActor->setLinearVelocity(physx::PxVec3(0.f));
			dynamicActor->setAngularVelocity(physx::PxVec3(0.f));
		}
		m_actor[i]->pActor->setGlobalPose(transform);
	}
}

//physx::PxMaterial PhysicWorld::getMaterail()
//{
//	return m_Material;
//}
//
//physx::PxControllerManager PhysicWorld::getControllerManager()
//{
//	return m_ControllerManager;
//}

void PhysicWorld::addControllerToMap(iCharacterCTRL* charCon, physx::PxController* phyCon)
{
	m_MapControllers.emplace(charCon, phyCon);
}

void PhysicWorld::addRigid(iCollision* body)
{
	RigidBody* rigid = RigidBody::cast(body);
	iShape* shape = rigid->getShape();
	physx::PxVec3 pos = physx::PxVec3(rigid->getPosition().x, rigid->getPosition().y, rigid->getPosition().z);
	physx::PxQuat rotate = physx::PxQuat(rigid->getRotation().x, rigid->getRotation().y, rigid->getRotation().z, rigid->getRotation().w);
	physx::PxTransform transform(pos, rotate);

	if (shape->getShapeType() == shapeType::Sphere)
	{
		iSphereShape* sphere = iSphereShape::cast(shape);
		rigid->pShape = m_Physics->createShape(physx::PxSphereGeometry(sphere->getRad()), *m_Material);
	}
	else if (shape->getShapeType() == shapeType::Box)
	{
		BoxShape* box = BoxShape::cast(shape);
		float halfExtentX = box->getHalfExtents().x;
		float halfExtentY = box->getHalfExtents().y;
		float halfExtentZ = box->getHalfExtents().z;
		rigid->pShape = m_Physics->createShape(physx::PxBoxGeometry(halfExtentX, halfExtentY, halfExtentZ), *m_Material);
	}
	else if (shape->getShapeType() == shapeType::Cylinder)
	{
		iCylinderShape* cylinder = iCylinderShape::cast(shape);
		float halfExtentX = cylinder->getHalfExtents().x;
		float halfExtentY = cylinder->getHalfExtents().y;
		rigid->pShape = m_Physics->createShape(physx::PxCapsuleGeometry(halfExtentX, halfExtentY), *m_Material);
	}
	else if (shape->getShapeType() == shapeType::Plane)
	{
		//iPlaneShape* plane = iPlaneShape::cast(shape);
		//physx::PxVec3 normal = physx::PxVec3(plane->getNormal().x, plane->getNormal().y, plane->getNormal().z);
		//rigid->pShape = m_Physics->createShape(physx::PxPlaneGeometry(normal,0));
	}
	if (rigid->isStatic())
	{
		rigid->pActor = m_Physics->createRigidStatic(transform);
		rigid->pActor->attachShape(*rigid->pShape);
	}
	else
	{
		physx::PxRigidDynamic* dynamicActor = m_Physics->createRigidDynamic(transform);
		dynamicActor->attachShape(*rigid->pShape);
		physx::PxRigidBodyExt::updateMassAndInertia(*dynamicActor, rigid->m_mass);
		rigid->pActor = dynamicActor;
	}
	m_actor.push_back(rigid);
	m_Scene->addActor(*rigid->pActor);
	m_mapTransform.emplace(rigid->pActor, transform);
	rigid->pShape->release();

}

void PhysicWorld::removeRigid(iCollision* body)
{
	RigidBody* rigid = RigidBody::cast(body);
	rigid->pActor->release();
	m_actor.erase(std::find(m_actor.begin(), m_actor.end(), rigid));
	delete body;
}
