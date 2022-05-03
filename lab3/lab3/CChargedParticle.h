#pragma once
#include "ChargeSign.h"
#include "Point.h"

class CChargedParticle
{
public:
	CChargedParticle(Point center, ChargeSign sign);
	void Draw(int points = 360) const;
	bool IsPointBelongsToParticle(Point point) const;
	float CalculateDistanceToParticle(Point otherParticleCenter, ChargeSign sign) const;
	Point GetCenter() const;
	ChargeSign GetSign() const;
	void SetCenter(Point point);
	Point GetForce() const;
	void SetForce(Point force);
	float GetMass() const;
	bool GetIsFromBorder() const;
	void SetIsFromBorder(bool isFromBorder);
private:
	Point m_center;
	int m_radius = 5;
	ChargeSign m_sign = ChargeSign::PLUS;
	float m_mass = 1;
	float m_charge = 10;
	Point m_force = {0, 0};
	bool m_isFromBorder = false;
};