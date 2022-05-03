#include "CChargedParticle.h"
//#define _USE_MATH_DEFINES
//#include <cmath>
#include <gl/glut.h>

CChargedParticle::CChargedParticle(Point center, ChargeSign sign)
	: m_center(center)
	, m_sign(sign)
{
}

void CChargedParticle::Draw(int points) const
{
	const float step = 2 * M_PI / points;
	if (m_sign == ChargeSign::PLUS)
	{
		glColor3f(1, 0, 0);
	}
	else
	{
		glColor3f(0, 0, 1);
	}
	// Эллипс представлет в виде "веера" из треугольников
	glBegin(GL_TRIANGLE_FAN);
	// Начальная точка веера располагается в центре эллипса
	glVertex2i(m_center.x, m_center.y);
	// Остальные точки - равномерно по его границе
	for (float angle = 0; angle <= 2 * M_PI; angle += step)
	{
		float a = (fabsf(angle - 2 * M_PI) < 1e-5) ? 0 : angle;
		const float dx = m_radius * cosf(a);
		const float dy = m_radius * sinf(a);
		glVertex2f(dx + m_center.x, dy + m_center.y);
	}
	glEnd();
}

bool CChargedParticle::IsPointBelongsToParticle(Point point) const
{
	return (pow(point.x - m_center.x, 2) + pow(point.y - m_center.y, 2)) <= pow(m_radius, 2);
}

float CChargedParticle::CalculateDistanceToParticle(Point otherParticleCenter, ChargeSign sign) const
{
	return pow(pow(otherParticleCenter.x - m_center.x, 2) + pow(otherParticleCenter.y - m_center.y, 2), 1 / 2);
}

Point CChargedParticle::GetCenter() const
{
	return m_center;
}

ChargeSign CChargedParticle::GetSign() const
{
	return m_sign;
}

void CChargedParticle::SetCenter(Point point)
{
	m_center = point;
}

Point CChargedParticle::GetForce() const
{
	return m_force;
}

void CChargedParticle::SetForce(Point force)
{
	m_force = force;
}

float CChargedParticle::GetMass() const
{
	return m_mass;
}

bool CChargedParticle::GetIsFromBorder() const
{
	return m_isFromBorder;
}

void CChargedParticle::SetIsFromBorder(bool isFromBorder)
{
	m_isFromBorder = isFromBorder;
}
