#include "stdafx.h"
#include "Torus.h"
#include "Solve4.h"
#include "Intersection.h"
#include "Ray.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

CTorus::CTorus(double radius, double smallRadius, CMatrix4d const& transform)
	: CGeometryObjectWithInitialTransformImpl(transform)
{
	assert(radius >= 0);
	assert(smallRadius >= 0);

	m_radius = radius;
	m_smallRadius = smallRadius;
	CMatrix4d initialTransform;
	SetInitialTransform(initialTransform);
}

bool CTorus::Hit(CRay const& ray, CIntersection& intersection) const
{
	CRay invRay = Transform(ray, GetInverseTransform());

	std::vector<double> intersections = GetIntersections(invRay.GetDirection(), invRay.GetStart());
	const double HIT_TIME_EPSILON = 1e-8;
	std::erase_if(intersections, [=](double x) { return x <= HIT_TIME_EPSILON; });

	double t = 0;
	if (std::min_element(std::begin(intersections), std::end(intersections)) != std::end(intersections))
	{
		t = *std::min_element(std::begin(intersections), std::end(intersections));

		CVector3d hitPoint = ray.GetPointAtTime(t);
		CVector3d hitPointInObjectSpace = invRay.GetPointAtTime(t);
		CVector3d const& hitNormalInObjectSpace = CalculateNormal(hitPointInObjectSpace);
		CVector3d hitNormal = GetNormalMatrix() * hitNormalInObjectSpace;
		CHitInfo hit(
			t, *this,
			hitPoint, hitPointInObjectSpace,
			hitNormal, hitNormalInObjectSpace);
		intersection.AddHit(hit);
	}

	return intersection.GetHitsCount() > 0;
}

std::vector<double> CTorus::GetIntersections(const CVector3d& direction, const CVector3d& origin) const
{
	double ox = origin.x;
	double oy = origin.y;
	double oz = origin.z;

	double dx = direction.x;
	double dy = direction.y;
	double dz = direction.z;

	double sumOfDirectionsSquares = pow(dx, 2) + pow(dy, 2) + pow(dz, 2);
	double e = pow(ox, 2) + pow(oy, 2) + pow(oz, 2) - pow(m_radius, 2) - pow(m_smallRadius, 2);
	double f = ox * dx + oy * dy + oz * dz;
	double fourRadiusSquares = 4.0 * pow(m_radius, 2);

	double c4 = pow(sumOfDirectionsSquares, 2);
	double c3 = 4 * sumOfDirectionsSquares * f;
	double c2 = 2 * sumOfDirectionsSquares * e + 4 * pow(f, 2) + fourRadiusSquares * pow(dy, 2);
	double c1 = 4 * f * e + 2 * fourRadiusSquares * oy * dy;
	double c0 = pow(e, 2) - fourRadiusSquares * (pow(m_smallRadius, 2) - pow(oy, 2));

	EquationRoot4 roots = Solve4(c4, c3, c2, c1, c0);

	std::vector<double> v(roots.roots, roots.roots + sizeof roots.roots / sizeof roots.roots[0]);
	return v;
}

CVector3d CTorus::CalculateNormal(CVector3d point) const
{
	CVector3d normal;
	normal.x = 4 * point.x * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) + pow(m_radius, 2) - pow(m_smallRadius, 2)) - 8 * pow(m_radius, 2) * point.x;
	normal.y = 4 * point.y * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) + pow(m_radius, 2) - pow(m_smallRadius, 2));
	normal.z = 4 * point.z * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) + pow(m_radius, 2) - pow(m_smallRadius, 2)) - 8 * pow(m_radius, 2) * point.z;
	normal.Normalize();
	return normal;
}
