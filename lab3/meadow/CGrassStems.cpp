# include "CGrassStems.h"

CGrassStems::CGrassStems(int xPos, int yPos)
	: m_xPos(xPos)
	, m_yPos(yPos)
{
}

void CGrassStems::Draw() const
{
	glLineWidth(3);
	glColor3ub(41, 148, 56);
	glBegin(GL_LINES);
	glVertex2i(m_xPos - 17, m_yPos - 30);
	glVertex2i(m_xPos, m_yPos);
	glVertex2i(m_xPos - 6, m_yPos - 36);
	glVertex2i(m_xPos, m_yPos);
	glVertex2i(m_xPos, m_yPos - 42);
	glVertex2i(m_xPos, m_yPos);
	glVertex2i(m_xPos + 11, m_yPos - 36);
	glVertex2i(m_xPos, m_yPos);
	glVertex2i(m_xPos + 23, m_yPos - 18);
	glVertex2i(m_xPos, m_yPos);
	glVertex2i(m_xPos + 23, m_yPos - 18);
	glVertex2i(m_xPos + 25, m_yPos - 6);
	glEnd();
}
