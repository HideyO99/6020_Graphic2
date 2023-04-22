#pragma once
#include "global.h"

enum class shapeType
{
	Box,
	Capsule,
	Cylinder,
	Ghost,
	Plane,
	Sphere,
};

class iShape
{
public:
	virtual ~iShape() {}
	shapeType getShapeType()
	{
		return m_shapeType;
	}

private:
	shapeType m_shapeType;
	iShape(const iShape&);
	iShape& operator=(const iShape&) {}

protected:
	iShape(shapeType type) 
	{
		m_shapeType = type;
	}
};

class iConvexShape : public iShape
{
public:
	virtual ~iConvexShape() {}

protected:
	iConvexShape(shapeType shapeType)
		: iShape(shapeType)
	{ }

private:
	iConvexShape(const iConvexShape&) = delete;
	iConvexShape& operator=(const iConvexShape&) {}
};

class iConcaveShape : public iShape
{
public:
	virtual ~iConcaveShape() {}

protected:
	iConcaveShape(shapeType shapeType)
		: iShape(shapeType)
	{ }

private:
	iConcaveShape(const iConcaveShape&) = delete;
	iConcaveShape& operator=(const iConcaveShape&) {}
};
