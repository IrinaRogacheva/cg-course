#pragma once
#include <gl/glut.h>
#include "IFlower.h"

class CRoundPetalFlower : public IFlower
{
public:
	CRoundPetalFlower(int xCenter, int yCenter, int width, int height, int numberOfPetalColor);
	void Draw() const override;
private:
	int m_xCenter;
	int m_yCenter;
	int m_width;
	int m_height;
	int m_numberOfPetalColor;
};