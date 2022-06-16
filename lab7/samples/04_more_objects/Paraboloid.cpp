#include "stdafx.h"
#include "Paraboloid.h"
#include "Solve4.h"

CParaboloid::CParaboloid(CMatrix4d const& transform)
	: CGeometryObjectWithInitialTransformImpl(transform)
{
	CMatrix4d initialTransform;
	SetInitialTransform(initialTransform);
}

bool CParaboloid::Hit(CRay const& ray, CIntersection& intersection) const
{
	enum class HitSurface
	{
		HIT_SIDE,
		HIT_CAP,
	};

	struct HitPoint
	{
		double hitTime;
		HitSurface hitSurface;
	};

	CRay invRay = Transform(ray, GetInverseTransform());
	const double HIT_TIME_EPSILON = 1e-8;

	double ox = invRay.GetStart().x;
	double oy = invRay.GetStart().y;
	double oz = invRay.GetStart().z;

	double dx = invRay.GetDirection().x;
	double dy = invRay.GetDirection().y;
	double dz = invRay.GetDirection().z;

	double a = pow(dx, 2) + pow(dz, 2);
	double b = 2 * ox * dx + 2 * oz * dz - dy;
	double c = pow(ox, 2) + pow(oz, 2) - oy;

	EquationRoot4 roots = Solve2(a, b, c);

	double hitTime, hitY;
	std::vector<HitPoint> hitPoints;
	for (size_t i = 0; i < roots.numRoots; i++)
	{
		hitTime = roots.roots[i];
		hitY = oy + dy * hitTime;
		if (hitTime > HIT_TIME_EPSILON && hitY <= 1)
		{
			hitPoints.push_back(HitPoint(hitTime, HitSurface::HIT_SIDE));
		}
	}

	if (std::abs(dy) > HIT_TIME_EPSILON)
	{
		hitTime = (1 - oy) / dy;
		if (hitTime > HIT_TIME_EPSILON)
		{
			double hitX = ox + dx * hitTime;
			double hitZ = oz + dz * hitTime;
			if (pow(hitX, 2) + pow(hitZ, 2) < 1)
			{
				hitPoints.push_back(HitPoint(hitTime, HitSurface::HIT_CAP));
			}
		}
	}

	if (hitPoints.size() == 2)
	{
		if (hitPoints[0].hitTime > hitPoints[1].hitTime)
		{
			HitPoint tmp(hitPoints[0]);
			hitPoints[0] = hitPoints[1];
			hitPoints[1] = tmp;
		}
	}

	for (unsigned i = 0; i < hitPoints.size(); ++i)
	{
		double const& hitTime = hitPoints[i].hitTime;

		CVector3d hitPoint = ray.GetPointAtTime(hitTime);
		CVector3d hitPointInObjectSpace = invRay.GetPointAtTime(hitTime);
		CVector3d hitNormalInObjectSpace = CalculateNormal(hitPointInObjectSpace);

		if (hitPoints[i].hitSurface == HitSurface::HIT_SIDE)
		{
			CVector3d n = invRay.GetDirection() * hitNormalInObjectSpace;
			if (n.x + n.y + n.z > 0)
			{
				hitNormalInObjectSpace = -hitNormalInObjectSpace;
			}
		}
		else
		{
			hitNormalInObjectSpace = CVector3d(0, 1, 0);
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

CVector3d CParaboloid::CalculateNormal(CVector3d point) const
{
	CVector3d normal;
	normal.x = 2 * point.x;
	normal.y = -1;
	normal.z = 2 * point.z;
	normal.Normalize();
	return normal;
}