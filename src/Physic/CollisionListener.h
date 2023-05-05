#pragma once
#include <iPhysicsWorld.h>

class CollisionListener : public iCollisionListener
{
public:
	CollisionListener() {}
	virtual ~CollisionListener() {}
	virtual void NotifyCollision(iCollisionBody* body1, iCollisionBody* body2) override;

};
