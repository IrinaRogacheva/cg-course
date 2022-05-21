#pragma once
#include <gl/glut.h>
#include "IFlower.h"

class CLilyFlower : public IFlower
{
public:
	CLilyFlower(int xCenter, int yCenter, int width, int height);
	void Draw() const override;
private:
	void DrawAtTheOrigin() const;

	int m_xCenter;
	int m_yCenter;
	int m_width;
	int m_height;
};