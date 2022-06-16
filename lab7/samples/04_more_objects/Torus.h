#pragma once
#include "GeometryObjectWithInitialTransformImpl.h"
#include <complex>

class CTorus : public CGeometryObjectWithInitialTransformImpl
{
public:
	CTorus(
		double radius = 1,
		double smallRadius = 0.5,
		CMatrix4d const& transform = CMatrix4d());

	virtual bool Hit(CRay const& ray, CIntersection& intersection) const;

private:
	typedef std::complex<double> complex;

	std::vector<double> GetIntersections(const CVector3d& direction, const CVector3d& vantage) const;
	CVector3d CalculateNormal(CVector3d point) const;

private:
	double m_radius = 1.0;
	double m_smallRadius = 0.5;
};