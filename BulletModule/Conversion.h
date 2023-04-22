#pragma once

#include <iCharacterCTRL.h>
#include "CharacterCTRL.h"
#include <iShape.h>
#include <BodyType.h>

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletDynamics/Character/btCharacterControllerInterface.h>
#include <bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include <glm/fwd.hpp>
#include <iCollisionBody.h>
#include <iVehicle.h>
#include <iConstraint.h>

// GLM -> Bullet
void CastBulletTransform(const glm::mat4& in, btTransform* out);
void CastBulletQuaternion(const glm::quat& in, btQuaternion* out);
void CastBulletVector3(const glm::vec3& in, btVector3* out);
void CastBulletScalar(const float in, btScalar* out);

// Bullet -> GLM
void CastGLMQuat(const btQuaternion& in, glm::quat* out);
void CastGLMVec3(const btVector3& in, glm::vec3* out);
void CastGLMMat4(const btTransform& in, glm::mat4* out);
void CastFloat(const btScalar& in, float* out);

// Interface -> Bullet
btCollisionShape* CastBulletShape(iShape* shape);
btConvexShape* CastBulletConvexShape(iConvexShape* shape);
btRigidBody* CastBulletRigidBody(iCollisionBody* body);
btRaycastVehicle* CastBulletVehicle(iVehicle* vehicle);
btCharacterControllerInterface* CastBulletCharacterController(
	iCharacterCTRL* characterController);
btTypedConstraint* CastBulletConstraint(iConstraint* constraint);
