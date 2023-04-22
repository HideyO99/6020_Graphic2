#pragma once

#include "global.h"

class iCharacterCTRL
{
public:
	virtual ~iCharacterCTRL() {}

	virtual void SetWalkDirection(const Vec3& walkDirection) = 0;
	virtual void SetVelocityForTimeInterval(const glm::vec3& velocity, float timeInterval) = 0;
	virtual void Warp(const glm::vec3& origin) = 0;
	virtual bool CanJump() const = 0;
	virtual void Jump(const glm::vec3& dir = glm::vec3(0, 0, 0)) = 0;
	virtual bool OnGround() const = 0;

protected:
	iCharacterCTRL() {}

private:
	iCharacterCTRL(const iCharacterCTRL&) { }
	iCharacterCTRL& operator=(const iCharacterCTRL&) {
		return *this;
	}
};