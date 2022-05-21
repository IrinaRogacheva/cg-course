#include "CGrassFern.h"
#include "Utils.h"

CGrassFern::CGrassFern(int xPos, int yPos)
	: m_xPos(xPos)
	, m_yPos(yPos)
{
}

void CGrassFern::Draw() const
{
    glColor3ub(67, 124, 23);
    glBegin(GL_LINES);
    glVertex2f(m_xPos, m_yPos);
    glVertex2f(m_xPos, m_yPos - 60);
    glEnd();
    FillEllipse(m_xPos - 17, m_yPos - 6, 17, 6);
    FillEllipse(m_xPos + 17, m_yPos - 6, 17, 6);
    FillEllipse(m_xPos - 14, m_yPos - 21, 14, 4.8);
    FillEllipse(m_xPos + 14, m_yPos - 21, 14, 4.8);
    FillEllipse(m_xPos - 11, m_yPos - 33, 11, 4.8);
    FillEllipse(m_xPos + 11, m_yPos - 33, 11, 4.8);
    FillEllipse(m_xPos - 8, m_yPos - 45, 8, 3.6);
    FillEllipse(m_xPos + 8, m_yPos - 45, 8, 3.6);
    FillEllipse(m_xPos, m_yPos - 60, 4.5, 12);
}
