#define _USE_MATH_DEFINES
#include <cmath>

#include "COblongPetalFlower.h"
#include "FlowerColorsMap.h"

COblongPetalFlower::COblongPetalFlower(int petalCount, int xCenter, int yCenter, int minRadius, int maxRadius, int numberOfPetalColor)
	: m_petalCount(petalCount)
	, m_xCenter(xCenter)
	, m_yCenter(yCenter)
	, m_minRadius(minRadius)
	, m_maxRadius(maxRadius)
	, m_numberOfPetalColor(numberOfPetalColor)
{
}

void COblongPetalFlower::Draw() const
{
	glPushMatrix();
	glTranslatef(m_xCenter, m_yCenter, 0);
	DrawAtTheOrigin();
	glPopMatrix();
}

void COblongPetalFlower::DrawAtTheOrigin() const
{
	const float step = float(2 * M_PI) / float(m_petalCount * 20);

	glBegin(GL_TRIANGLE_FAN);

	Color petalColor = PETAL_COLORS[m_numberOfPetalColor];
	Color centerColor = PETAL_COLOR_TO_CENTER_COLOR.at(petalColor);

	glColor3ub(centerColor.r, centerColor.g, centerColor.b);
	glVertex2f(0, 0);

	glColor3ub(petalColor.r, petalColor.g, petalColor.b);
	for (float angle = 0; angle <= float(2 * M_PI) + 0.5f * step; angle += step)
	{
		float fixedAngle = (fabsf(angle - float(2 * M_PI)) < 1e-4f) ? 0 : angle;

		float radius = GetRadiusByAngle(fixedAngle);
		float x = radius * cosf(fixedAngle);
		float y = radius * sinf(fixedAngle);

		glVertex2f(x, y);
	}

	glEnd();
}

float COblongPetalFlower::GetRadiusByAngle(float angle) const
{
	return 0.5f * (m_maxRadius - m_minRadius) * cosf(angle * m_petalCount) + m_minRadius;
}
