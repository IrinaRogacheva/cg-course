#pragma once
#include "GeometryObjectWithInitialTransformImpl.h"
#include "Solve4.h"

class HyperbolicParaboloid : public CGeometryObjectWithInitialTransformImpl
{
public:
	HyperbolicParaboloid(CMatrix4d const& tranform = CMatrix4d());

	virtual bool Hit(CRay const& ray, CIntersection& intersection) const;

private:
	CVector3d CalculateNormal(CVector3d point) const;
};
