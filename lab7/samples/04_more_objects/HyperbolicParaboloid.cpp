#include "stdafx.h"
#include "HyperbolicParaboloid.h"
#include "Intersection.h"
#include "Ray.h"
#include "Solve4.h"

HyperbolicParaboloid::HyperbolicParaboloid(CMatrix4d const& transform)
	: CGeometryObjectWithInitialTransformImpl(transform)
{
	CMatrix4d initialTransform;
	SetInitialTransform(initialTransform);
}

bool HyperbolicParaboloid::Hit(CRay const& ray, CIntersection& intersection) const
{
	CRay invRay = Transform(ray, GetInverseTransform());
	const double HIT_TIME_EPSILON = 1e-8;

	double ox = invRay.GetStart().x;
	double oy = invRay.GetStart().y;
	double oz = invRay.GetStart().z;

	double dx = invRay.GetDirection().x;
	double dy = invRay.GetDirection().y;
	double dz = invRay.GetDirection().z;

	double a = pow(dx, 2) - pow(dz, 2);
	double b = 2 * ox * dx - 2 * oz * dz - dy;
	double c = pow(ox, 2) - pow(oz, 2) - oy;

	EquationRoot4 roots = Solve2(a, b, c);

	std::vector<double> hitPoints;
	for (size_t i = 0; i < roots.numRoots; i++)
	{
		double hitY = oy + dy * roots.roots[i];
		double hitX = ox + dx * roots.roots[i];
		if ((roots.roots[i] > HIT_TIME_EPSILON && hitY <= 1 && hitY >= -1 && hitX <= 1 && hitX >= -1))
		{
			hitPoints.push_back(roots.roots[i]);
		}
	}

	std::sort(hitPoints.begin(), hitPoints.end());

	double hitTime;
	for (unsigned i = 0; i < hitPoints.size(); ++i)
	{
		hitTime = hitPoints[i];

		CVector3d hitPoint = ray.GetPointAtTime(hitTime);
		CVector3d hitPointInObjectSpace = invRay.GetPointAtTime(hitTime);
		CVector3d hitNormalInObjectSpace = CalculateNormal(hitPointInObjectSpace);
		CVector3d n = invRay.GetDirection() * hitNormalInObjectSpace;
		if (n.x + n.y + n.z > 0)
		{
			hitNormalInObjectSpace = -hitNormalInObjectSpace;
		}
		CVector3d hitNormal = GetNormalMatrix() * hitNormalInObjectSpace;

		CHitInfo hit(
			hitTime, *this,
			hitPoint, hitPointInObjectSpace,
			hitNormal, hitNormalInObjectSpace);

		intersection.AddHit(hit);
	}

	return intersection.GetHitsCount() > 0;
}

CVector3d HyperbolicParaboloid::CalculateNormal(CVector3d point) const
{
	CVector3d normal;
	normal.x = 2 * point.x;
	normal.y = -1;
	normal.z = -2 * point.z;
	normal.Normalize();
	return normal;
}