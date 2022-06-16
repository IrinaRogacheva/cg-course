#pragma once
#include "GeometryObjectWithInitialTransformImpl.h"
#include "Intersection.h"
#include "Ray.h"
#include <complex>
#include <vector>

class CParaboloid : public CGeometryObjectWithInitialTransformImpl
{
public:
	CParaboloid(CMatrix4d const& transform = CMatrix4d());

	virtual bool Hit(CRay const& ray, CIntersection& intersection) const;

private:
	CVector3d CalculateNormal(CVector3d point) const;
};