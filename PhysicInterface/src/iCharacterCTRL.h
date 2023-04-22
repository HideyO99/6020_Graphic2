#pragma once

#include "global.h"

class iCharacterCTRL
{
public:
	virtual ~iCharacterCTRL() {}

	virtual void SetWalkDirection(const Vec3& walkDirection) = 0;
	virtual void SetPosition(const Vec3& position) = 0;
	virtual void GetPosition(Vec3& position) const = 0;

	virtual void SetRotation(const Quat& rotation) = 0;
	virtual void GetRotation(Quat& rotation) const = 0;

	virtual void SetGravity(const Vec3& gravity) = 0;
protected:
	iCharacterCTRL() {}

private:
	iCharacterCTRL(const iCharacterCTRL&) { }
	iCharacterCTRL& operator=(const iCharacterCTRL&) {
		return *this;
	}
};