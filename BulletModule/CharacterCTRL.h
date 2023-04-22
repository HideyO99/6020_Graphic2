#pragma once

#include <iCharacterCTRL.h>
#include <iShape.h>
#include "Conversion.h"
#include <global.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <glm/glm.hpp>

class CharacterCTRL : public iCharacterCTRL
{
public:
    CharacterCTRL(btPairCachingGhostObject* btGhostObject, btConvexShape* shape, btScalar stepHeight, const btVector3& up);
    virtual ~CharacterCTRL();

    virtual void SetWalkDirection(const Vec3& walkDirection) override;
    virtual void SetVelocityForTimeInterval(const Vec3& velocity, float timeInterval);
    virtual void Warp(const Vec3& origin);
    virtual bool CanJump() const;
    virtual void Jump(const Vec3& dir);
    virtual bool OnGround() const;

    btPairCachingGhostObject* GetGhostObject() {
        return m_GhostObject;
    }

    btCharacterControllerInterface* GetBulletCharacterController() {
        return m_CharacterController;
    }

private:
    CharacterCTRL() {}
    
    btPairCachingGhostObject* m_GhostObject;
    btKinematicCharacterController* m_CharacterController;

    CharacterCTRL(const CharacterCTRL& other) { }
    CharacterCTRL& operator=(const CharacterCTRL& other0) {
        return *this;
    }

    Vec3* m_Gravity;
    Vec3* m_Velocity;
};