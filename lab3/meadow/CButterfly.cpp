#include "CButterfly.h"
#include "Utils.h"

CButterfly::CButterfly(ButterflyType type, int xPos, int yPos)
	: m_type(type)
	, m_xPos(xPos)
	, m_yPos(yPos)
{
    m_firstWingColor = GenerateRandomColor();
    m_secondWingColor = GenerateRandomColor();
}

void CButterfly::Draw() const
{
	if (m_type == ButterflyType::FIRST)
	{
		DrawFirst();
	}
	else if (m_type == ButterflyType::SECOND)
	{
		DrawSecond();
	}
	else
	{
		DrawThird();
	}
}

void CButterfly::DrawFirst() const
{
    glLineWidth(2);
    glBegin(GL_TRIANGLES);
    glColor3ub(m_firstWingColor.r, m_firstWingColor.g, m_firstWingColor.b);
    glVertex2i(m_xPos, m_yPos);
    glVertex2i(m_xPos + 20, m_yPos - 6);
    glVertex2i(m_xPos - 11, m_yPos - 36);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3ub(m_secondWingColor.r, m_secondWingColor.g, m_secondWingColor.b);
    glVertex2i(m_xPos, m_yPos);
    glVertex2i(m_xPos + 22.5, m_yPos + 12);
    glVertex2i(m_xPos + 17, m_yPos);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(0, 0, 0);
    glVertex2i(m_xPos - 11, m_yPos - 6);
    glVertex2i(m_xPos + 11, m_yPos + 12);
    glEnd();
}

void CButterfly::DrawSecond() const
{
    glLineWidth(3);
    glColor3ub(m_firstWingColor.r, m_firstWingColor.g, m_firstWingColor.b);
    FillEllipse(m_xPos, m_yPos + 24, 11, 9);
    glColor3ub(m_secondWingColor.r, m_secondWingColor.g, m_secondWingColor.b);
    FillEllipse(m_xPos + 11, m_yPos + 9, 11, 18);
    glColor3ub(255, 255, 255);
    FillEllipse(m_xPos + 11, m_yPos + 6, 5.5, 6);
    glColor3ub(255, 0, 100);
    FillEllipse(m_xPos + 11, m_yPos + 6, 4, 4.2);
    glBegin(GL_LINES);
    glColor3ub(0, 0, 0);
    glVertex2f(m_xPos + 3, m_yPos + 36);
    glVertex2f(m_xPos + 25, m_yPos + 18);
    glEnd();
}

void CButterfly::DrawThird() const
{
    glLineWidth(3);
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(m_firstWingColor.r, m_firstWingColor.g, m_firstWingColor.b);
    glVertex2f(m_xPos, m_yPos);
    glVertex2f(m_xPos, m_yPos - 18);
    glVertex2f(m_xPos - 17, m_yPos - 36);
    glVertex2f(m_xPos - 17, m_yPos - 18);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(m_secondWingColor.r, m_secondWingColor.g, m_secondWingColor.b);
    glVertex2f(m_xPos, m_yPos);
    glVertex2f(m_xPos - 11, m_yPos - 3);
    glVertex2f(m_xPos - 22.5, m_yPos + 3);
    glVertex2f(m_xPos - 11, m_yPos + 12);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(0, 0, 0);
    glVertex2f(m_xPos - 3, m_yPos + 13);
    glVertex2f(m_xPos + 3, m_yPos - 13);
    glEnd();
}
