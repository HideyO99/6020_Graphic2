#pragma once
#include <iCharacterCTRL.h>
#include <iShape.h>
#include <global.h>
#include <Physx/PxPhysicsAPI.h>
//#include "PhysicWorld.h"


class CharacterCTRL : public iCharacterCTRL
{
public:
	CharacterCTRL(iShape* shape, const Vec3& position, const Quat& rotation);
	virtual ~CharacterCTRL();

    virtual void SetWalkDirection(const Vec3& walkDirection) override;
    virtual void SetPosition(const Vec3& position) override;
    virtual void GetPosition(Vec3& position) const override;

    virtual void SetRotation(const Quat& rotation) override;
    virtual void GetRotation(Quat& rotation) const override;

    virtual void SetGravity(const Vec3& gravity) override;

    //PhysicWorld* phyworld;
private:
    physx::PxController* m_Controller;
    Vec3* m_Gravity;
    Vec3* m_Velocity;
};

