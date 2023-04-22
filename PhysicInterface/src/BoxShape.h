#pragma once
#include "iShape.h"


class BoxShape : public iShape
{
public:
	BoxShape(const Vec3& halfExtents);
	virtual ~BoxShape();

	const Vec3& GetHalfExtents() const;

	static BoxShape* Cast(iShape* shape);

protected:
	BoxShape(ShapeType shapeType)
		: iShape(shapeType) {}

private:
	Vec3 m_HalfExtents;
	BoxShape(const BoxShape&) : iShape(ShapeType::Box) {}
	BoxShape& operator=(const BoxShape&) {
		return *this;
	}
};
