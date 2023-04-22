#include "Conversion.h"
#include <BoxShape.h>
#include <PlaneShape.h>
#include <SphereShape.h>
#include <CylinderShape.h>
#include <CapsuleShape.h>
#include "RigidBody.h"
#include "Vehicle.h"
#include <ConeTwistConstraint.h>
#include <Point2PointConstraint.h>

void CastGLMQuat(const btQuaternion& in, glm::quat* out)
{
	CastFloat(in.x(), &out->x);
	CastFloat(in.y(), &out->y);
	CastFloat(in.z(), &out->z);
	CastFloat(in.w(), &out->w);
}

void CastGLMVec3(const btVector3& in, glm::vec3* out)
{
	CastFloat(0.0f, &out->p);
	CastFloat(0.0f, &out->r);
	CastFloat(0.0f, &out->s);
	CastFloat(0.0f, &out->t);
	CastFloat(in.x(), &out->x);
	CastFloat(in.y(), &out->y);
	CastFloat(in.z(), &out->z);
}

void CastFloat(const btScalar& in, float* out)
{
	*out = in;
}

void CastGLMMat4(const btTransform& in, glm::mat4* out)
{
	in.getOpenGLMatrix(&((*out)[0][0]));
}

void CastBulletTransform(const glm::mat4& in, btTransform* out)
{
	(*out).setFromOpenGLMatrix((const btScalar*)(&in[0][0]));
}

void CastBulletQuaternion(const glm::quat& in, btQuaternion* out)
{
	out->setX(in.x);
	out->setY(in.y);
	out->setZ(in.z);
	out->setW(in.w);
}

void CastBulletVector3(const glm::vec3& in, btVector3* out)
{
	out->setX(in.x);
	out->setY(in.y);
	out->setZ(in.z);
}

void CastBulletScalar(const float in, btScalar* out)
{
	*out = in;
}

btCollisionShape* CastBulletShape(iShape* shape)
{
	switch (shape->GetShapeType())
	{
	case ShapeType::Ghost:
	{
		btEmptyShape* btEmpty = new btEmptyShape();

		return btEmpty;
	} break;

	case ShapeType::Box:
	{
		BoxShape* box = BoxShape::Cast(shape);

		btVector3 halfExtents;

		CastBulletVector3(box->GetHalfExtents(), &halfExtents);

		btBoxShape* btBox = new btBoxShape(halfExtents);

		return btBox;
	} break;

	case ShapeType::Plane:
	{
		PlaneShape* plane = PlaneShape::Cast(shape);

		btVector3 normal;
		btScalar planeConstant;

		CastBulletVector3(plane->GetNormal(), &normal);
		CastBulletScalar(plane->GetDotProduct(), &planeConstant);

		btStaticPlaneShape* btPlane = new btStaticPlaneShape(normal, planeConstant);

		return btPlane;
	} break;

	case ShapeType::Sphere:
	{
		SphereShape* sphere = SphereShape::Cast(shape);

		btScalar radius;

		CastBulletScalar(sphere->GetRadius(), &radius);

		btSphereShape* btSphere = new btSphereShape(radius);

		return btSphere;

	} break;

	case ShapeType::Cylinder:
	{
		CylinderShape* mesh = CylinderShape::Cast(shape);

		btVector3 btHalfExtents;

		CastBulletVector3(mesh->GetHalfExtents(), &btHalfExtents);

		btCylinderShape* btCylinder = new btCylinderShape(btHalfExtents);

		return btCylinder;
	} break;

	case ShapeType::Capsule:
	{
		CapsuleShape* capsule = CapsuleShape::Cast(shape);

		btScalar btRadius;
		btScalar btHeight;

		CastBulletScalar(capsule->GetRadius(), &btRadius);
		CastBulletScalar(capsule->GetHeight(), &btHeight);

		btCapsuleShape* btCapsule = new btCapsuleShape(btRadius, btHeight);

		return btCapsule;
	} break;
	default:

		break;
	}

	return NULL;
}

btConvexShape* CastBulletConvexShape(iConvexShape* shape)
{

	switch (shape->GetShapeType())
	{
	case ShapeType::Capsule:
	{
		CapsuleShape* capsule = CapsuleShape::Cast(shape);

		btScalar radius;
		btScalar height;

		CastBulletScalar(capsule->GetRadius(), &radius);
		CastBulletScalar(capsule->GetHeight(), &height);

		btCapsuleShape* btCapsule = new btCapsuleShape(radius, height);

		return btCapsule;
	}
	break;

	default:
		// Not a valid shape
		assert(0);
		break;
	}

	return nullptr;
}

btRigidBody* CastBulletRigidBody(iCollisionBody* body)
{
	if (body == nullptr)
		return nullptr;

	return dynamic_cast<RigidBody*>(body)->GetBulletBody();
}

btRaycastVehicle* CastBulletVehicle(iVehicle* vehicle)
{
	return dynamic_cast<Vehicle*>(vehicle)->GetBulletVehicle();
}

btCharacterControllerInterface* CastBulletCharacterController(
	iCharacterCTRL* characterController)
{
	return dynamic_cast<CharacterCTRL*>(characterController)->GetBulletCharacterController();
}

btTypedConstraint* CastBulletConstraint(iConstraint* constraint)
{
	ConstraintType type = constraint->GetConstraintType();

	switch (type)
	{
	case ConstraintType::ConeTwist:
	{
		ConeTwistConstraint* coneTwist = ConeTwistConstraint::Cast(constraint);

		btRigidBody* btBodyA = CastBulletRigidBody(coneTwist->GetRididBodyA());
		btRigidBody* btBodyB = CastBulletRigidBody(coneTwist->GetRididBodyB());

		btTransform btTransformA;
		btTransform btTransformB;
		CastBulletTransform(coneTwist->GetTransformA(), &btTransformA);
		CastBulletTransform(coneTwist->GetTransformB(), &btTransformB);

		btConeTwistConstraint* btConeTwist;
		if (btBodyB == nullptr)
		{
			btConeTwist = new btConeTwistConstraint(*btBodyA, btTransformA);
		}
		else
		{
			btConeTwist = new btConeTwistConstraint(*btBodyA, *btBodyB, btTransformA, btTransformB);
		}

		return btConeTwist;
	}
	break;

	case ConstraintType::Gear:
		assert(0); // not implemented
		break;

	case ConstraintType::Hinge:
		assert(0); // not implemented
		break;

	case ConstraintType::Point2Point:
	{
		Point2PointConstraint* p2p = Point2PointConstraint::Cast(constraint);

		btRigidBody* btBodyA = CastBulletRigidBody(p2p->GetRididBodyA());
		btRigidBody* btBodyB = CastBulletRigidBody(p2p->GetRididBodyB());

		btVector3 btVecA;
		btVector3 btVecB;
		CastBulletVector3(p2p->GetPointA(), &btVecA);
		CastBulletVector3(p2p->GetPointB(), &btVecB);

		btPoint2PointConstraint* btP2P;
		if (btBodyB == nullptr)
		{
			btP2P = new btPoint2PointConstraint(*btBodyA, btVecA);
		}
		else
		{
			btP2P = new btPoint2PointConstraint(*btBodyA, *btBodyB, btVecA, btVecB);
		}

		return btP2P;

		break;
	}
	case ConstraintType::Slider:
		assert(0); // not implemented
		break;

	default:
		assert(0); // Don't know what this is
		break;
	}


	assert(0); // Shouldn't get here
	return nullptr;
}