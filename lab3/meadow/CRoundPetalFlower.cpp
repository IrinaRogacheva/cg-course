#include "CRoundPetalFlower.h"
#include "FlowerColorsMap.h"
#include "Utils.h"

CRoundPetalFlower::CRoundPetalFlower(int xCenter, int yCenter, int width, int height, int numberOfPetalColor)
	: m_xCenter(xCenter)
	, m_yCenter(yCenter)
	, m_width(width)
	, m_height(height)
	, m_numberOfPetalColor(numberOfPetalColor)
{
}

void CRoundPetalFlower::Draw() const
{
	Color petalColor = PETAL_COLORS[m_numberOfPetalColor];
	glColor3ub(petalColor.r, petalColor.g, petalColor.b);

	int height = m_height;
	FillEllipse(m_xCenter, m_yCenter - height / 5, m_width / 5, height / 5); //верхний лепесток
	FillEllipse(m_xCenter + m_width / 6, m_yCenter - height / 10, m_width / 5, height / 5); //верхний правый лепесток
	FillEllipse(m_xCenter - m_width / 6, m_yCenter - height / 10, m_width / 5, height / 5); //верхний левый лепесток
	FillEllipse(m_xCenter + m_width / 7, m_yCenter + height / 6, m_width / 5, height / 5); //правый нижний лепесток
	FillEllipse(m_xCenter - m_width / 7, m_yCenter + height / 6, m_width / 5, height / 5); //левый нижний лепесток

	Color centerColor = PETAL_COLOR_TO_CENTER_COLOR.at(petalColor);
	glColor3ub(centerColor.r, centerColor.g, centerColor.b);
	FillEllipse(m_xCenter, m_yCenter, 4, 4);
}
