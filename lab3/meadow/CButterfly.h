#pragma once
#include <gl/glut.h>
#include "IButterfly.h"
#include "EButterflyType.h"
#include "Color.h"

class CButterfly : public IButterfly
{
public:
	CButterfly(ButterflyType type, int xPos, int yPos);
	void Draw() const override;
private:
	int m_xPos;
	int m_yPos;
	ButterflyType m_type;

	Color m_firstWingColor;
	Color m_secondWingColor;

	void DrawFirst() const;
	void DrawSecond() const;
	void DrawThird() const;
};