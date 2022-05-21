/*
#define _USE_MATH_DEFINES
#include <cmath>

#include "CFlower.h"
#include "Utils.h"
#include <stdexcept>

const float CFlower::MIN_RADIUS = 8;

CFlower::CFlower(FlowerType type, int petalCount, int xCenter, int yCenter, int width, int numberOfCenterColor, int numberOfPetalColor, float angle)
	: m_type(type)
	, m_petalCount(petalCount)
	, m_angle(angle)
	, m_xCenter(xCenter)
	, m_yCenter(yCenter)
	, m_width(width)
	, m_numberOfCenterColor(numberOfCenterColor)
	, m_numberOfPetalColor(numberOfPetalColor)
{
}

void CFlower::Draw() const
{
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(m_xCenter, m_yCenter, 0);
	glRotatef(m_angle, 0.0, 0.0, 1.0);
	glTranslatef(-m_xCenter, -m_yCenter, 0);
	
	if (m_type == FlowerType::THREE_LEAF)
	{
		DrawThreeLeafFlower();
	}
	else if (m_type == FlowerType::FOUR_LEAF)
	{

		
		// Сохраняем старую матрицу в стек матриц драйвера
		glPushMatrix();
		glTranslatef(m_xCenter, m_yCenter, 0);
		// Способ рисования не изменился
		DrawLongLeafsFlower();
		// Извлекаем старую матрицу из стека матриц драйвера
		glPopMatrix();
		
		
	}
	else
	{
		glColor3ub(247, 232, 99);
		FillEllipse(m_xCenter, m_yCenter, 4, 4);
		throw std::invalid_argument("error");
	}

	glPopMatrix();
}

void CFlower::DrawThreeLeafFlower() const
{
	glColor3ub(PETAL_COLORS[m_numberOfPetalColor].r, PETAL_COLORS[m_numberOfPetalColor].g, PETAL_COLORS[m_numberOfPetalColor].b);
	
	int height = m_width * 0.9;
	FillEllipse(m_xCenter, m_yCenter - height / 5, m_width / 5, height / 5); //верхний лепесток
	FillEllipse(m_xCenter + m_width / 6, m_yCenter - height / 10, m_width / 5, height / 5); //верхний правый лепесток
	FillEllipse(m_xCenter - m_width / 6, m_yCenter - height / 10, m_width / 5, height / 5); //верхний левый лепесток
	FillEllipse(m_xCenter + m_width / 7, m_yCenter + height / 6, m_width / 5, height / 5); //правый нижний лепесток
	FillEllipse(m_xCenter - m_width / 7, m_yCenter + height / 6, m_width / 5, height / 5); //левый нижний лепесток
	
	glColor3ub(CENTER_COLORS[m_numberOfCenterColor].r, CENTER_COLORS[m_numberOfCenterColor].g, CENTER_COLORS[m_numberOfCenterColor].b);
	FillEllipse(m_xCenter, m_yCenter, 4, 4);
}

void CFlower::DrawLongLeafsFlower() const
{
	const float step = float(2 * M_PI) / float(m_petalCount * 20);

	glBegin(GL_TRIANGLE_FAN);

	glColor3ub(CENTER_COLORS[m_numberOfCenterColor].r, CENTER_COLORS[m_numberOfCenterColor].g, CENTER_COLORS[m_numberOfCenterColor].b);
	glVertex2f(0, 0);

	glColor3ub(PETAL_COLORS[m_numberOfPetalColor].r, PETAL_COLORS[m_numberOfPetalColor].g, PETAL_COLORS[m_numberOfPetalColor].b);
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

float CFlower::GetRadiusByAngle(float angle) const
{
	return 0.5f * (m_width / 2 - MIN_RADIUS) * cosf(angle * m_petalCount) + MIN_RADIUS;
}
*/