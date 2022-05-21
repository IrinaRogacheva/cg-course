#pragma once
#include <gl/glut.h>
#include "IFlower.h"

class COblongPetalFlower : public IFlower
{
public:
	COblongPetalFlower(int petalCount, int xCenter, int yCenter, int minRadius, int maxRadius, int numberOfPetalColor);
	void Draw() const override;
private:
	void DrawAtTheOrigin() const;
	float GetRadiusByAngle(float angle) const;

	int m_xCenter;
	int m_yCenter;
	int m_minRadius;
	int m_maxRadius;
	int m_numberOfPetalColor;
	int m_petalCount;
};