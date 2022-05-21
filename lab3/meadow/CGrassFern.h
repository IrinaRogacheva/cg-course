#pragma once
#include <gl/glut.h>
#include "IGrass.h"

class CGrassFern : public IGrass
{
public:
	CGrassFern(int xPos, int yPos);
	void Draw() const override;
private:
	int m_xPos;
	int m_yPos;
};