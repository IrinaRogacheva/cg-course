#include "CMyApplication.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

CMyApplication* CMyApplication::m_pApp = NULL;

CMyApplication::CMyApplication(const char* title, int width, int height, bool needDepth)
	:CGLApplication(title, width, height, needDepth)
{
	m_pApp = this;
}

CMyApplication::~CMyApplication(void)
{
}

size_t CMyApplication::GetCountOfParticles() const
{
	return m_chargedParticles.size();
}
/*
const CChargedParticle& CMyApplication::GetParticleAtIndex(size_t index) const
{
	return m_chargedParticles[index];
}
*/
CChargedParticle& CMyApplication::GetParticleAtIndex(size_t index)
{
	return m_chargedParticles[index];
}

void CMyApplication::OnInit()
{
	// Задаем цвет очистки буфера кадра
	glClearColor(0, 0, 0, 1);
	SetTimer(1000, Callback, 1);
}

void CMyApplication::Callback(int value)
{
	m_pApp->PostRedisplay();
	m_pApp->SetTimer(1000/60, Callback, 1);

	const int CHARGE = 50;
	Point nextPos;
	bool isFromBorder = false;
	
	if (m_pApp->GetCountOfParticles() > 1)
	{
		for (size_t i = 0; i < m_pApp->GetCountOfParticles() - 1; ++i)
		{
			for (size_t j = i + 1; j < m_pApp->GetCountOfParticles(); ++j)
			{
				Point diff = m_pApp->GetParticleAtIndex(j).GetCenter() - m_pApp->GetParticleAtIndex(i).GetCenter();
				double distSqrd = diff.x * diff.x + diff.y * diff.y;
				Point normal = diff / sqrt(distSqrd);
				Point force = normal * CHARGE * CHARGE / distSqrd;
				if ((m_pApp->GetParticleAtIndex(j).GetForce() - m_pApp->GetParticleAtIndex(j).GetCenter()) < (force - m_pApp->GetParticleAtIndex(j).GetCenter()) && m_pApp->GetParticleAtIndex(j).GetIsFromBorder())
				{
					m_pApp->GetParticleAtIndex(j).SetIsFromBorder(false);
				}
				if ((m_pApp->GetParticleAtIndex(i).GetForce() - m_pApp->GetParticleAtIndex(j).GetCenter()) < (force - m_pApp->GetParticleAtIndex(j).GetCenter()) && m_pApp->GetParticleAtIndex(i).GetIsFromBorder())
				{
					m_pApp->GetParticleAtIndex(i).SetIsFromBorder(false);
				}
				if (m_pApp->GetParticleAtIndex(j).GetForce().x > 0 || m_pApp->GetParticleAtIndex(i).GetForce().x)
				{
					Point forceerer = m_pApp->GetParticleAtIndex(j).GetForce();
					Point forceereri = m_pApp->GetParticleAtIndex(i).GetForce();
				}
				if (m_pApp->GetParticleAtIndex(j).GetSign() == m_pApp->GetParticleAtIndex(i).GetSign())
				{
					m_pApp->GetParticleAtIndex(j).SetForce(m_pApp->GetParticleAtIndex(j).GetForce() + force);
					m_pApp->GetParticleAtIndex(i).SetForce(m_pApp->GetParticleAtIndex(i).GetForce() - force);
				}
				else
				{
					m_pApp->GetParticleAtIndex(j).SetForce(m_pApp->GetParticleAtIndex(j).GetForce() - force);
					m_pApp->GetParticleAtIndex(i).SetForce(m_pApp->GetParticleAtIndex(i).GetForce() + force);
				}
			}
		}

		const int RADIUS = 5;
		const int WINDOW_WIDTH = 700;
		const int WINDOW_HEIGHT = 500;

		for (size_t i = 0; i < m_pApp->GetCountOfParticles(); ++i)
		{
			nextPos = m_pApp->GetParticleAtIndex(i).GetCenter() + m_pApp->GetParticleAtIndex(i).GetForce() / m_pApp->GetParticleAtIndex(i).GetMass();
			
			if (nextPos.x <= 0 + RADIUS || nextPos.y <= 0 + RADIUS || nextPos.x >= WINDOW_WIDTH - RADIUS || nextPos.y >= WINDOW_HEIGHT - RADIUS)
			{
				m_pApp->GetParticleAtIndex(i).SetIsFromBorder(true);
			}
			if ((nextPos.x <= 0 + RADIUS || nextPos.x >= WINDOW_WIDTH - RADIUS) && m_pApp->GetParticleAtIndex(i).GetIsFromBorder())
			{
				m_pApp->GetParticleAtIndex(i).SetForce({ m_pApp->GetParticleAtIndex(i).GetForce().x * (-1),  m_pApp->GetParticleAtIndex(i).GetForce().y });
				nextPos = m_pApp->GetParticleAtIndex(i).GetCenter() + m_pApp->GetParticleAtIndex(i).GetForce() / m_pApp->GetParticleAtIndex(i).GetMass();
				if (nextPos <= Point{ WINDOW_WIDTH - RADIUS, WINDOW_HEIGHT - RADIUS } && nextPos >= Point{ 0 + RADIUS, 0 + RADIUS })
				{
					m_pApp->GetParticleAtIndex(i).SetCenter(nextPos);
				}
				else
				{
					//m_pApp->GetParticleAtIndex(i).SetIsFromBorder(false);
				}
			}
			else if ((nextPos.y <= 0 + RADIUS || nextPos.y >= WINDOW_HEIGHT - RADIUS) && m_pApp->GetParticleAtIndex(i).GetIsFromBorder())
			{
				m_pApp->GetParticleAtIndex(i).SetForce({ m_pApp->GetParticleAtIndex(i).GetForce().x,  m_pApp->GetParticleAtIndex(i).GetForce().y * (-1) });
				nextPos = m_pApp->GetParticleAtIndex(i).GetCenter() + m_pApp->GetParticleAtIndex(i).GetForce() / m_pApp->GetParticleAtIndex(i).GetMass();
				if (nextPos <= Point{ WINDOW_WIDTH - RADIUS, WINDOW_HEIGHT - RADIUS } && nextPos >= Point{ 0 + RADIUS, 0 + RADIUS })
				{
					m_pApp->GetParticleAtIndex(i).SetCenter(nextPos);
				}
				else
				{
					//m_pApp->GetParticleAtIndex(i).SetIsFromBorder(false);
				}
			}
			else if (!m_pApp->GetParticleAtIndex(i).GetIsFromBorder())
			{
				if (nextPos <= Point{ WINDOW_WIDTH - RADIUS, WINDOW_HEIGHT - RADIUS } && nextPos >= Point{ 0 + RADIUS, 0 + RADIUS })
				{
					m_pApp->GetParticleAtIndex(i).SetCenter(nextPos);
				}
				m_pApp->GetParticleAtIndex(i).SetForce({ 0, 0 });
			}
			/*
			if (nextPos <= Point{ WINDOW_WIDTH - RADIUS, WINDOW_HEIGHT - RADIUS } && nextPos >= Point{ 0 + RADIUS, 0 + RADIUS })
			{
				m_pApp->GetParticleAtIndex(i).SetCenter(nextPos);
			}
			*/
			//m_pApp->GetParticleAtIndex(i).Draw();
			//m_pApp->GetParticleAtIndex(i).SetForce({ 0, 0 });
		}
	}
	
}

void CMyApplication::OnDisplay(void)
{
	// Очищаем буфер цвета и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& chargedParticle : m_chargedParticles)
	{
		chargedParticle.Draw();
	}
	
}

void CMyApplication::OnReshape(int width, int height)
{
	// Настраиваем порт просмотра
	glViewport(0, 0, width, height);

	// Делаем текущей матрицу проецирования и настраиваем ее параметры
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);

	// Делаем текущей матрицей матрицу моделирования-вида
	glMatrixMode(GL_MODELVIEW);
}

void CMyApplication::OnMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (FindParticleAtPoint({ x, y }))
		{
			m_chargedParticles.erase(m_chargedParticles.begin() + *FindParticleAtPoint({ x, y }));
			PostRedisplay();
			return;
		}

		if (button == GLUT_LEFT_BUTTON)
		{
			m_chargedParticles.push_back(CChargedParticle({ x, y }, ChargeSign::PLUS));
			PostRedisplay();
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			m_chargedParticles.push_back(CChargedParticle({ x, y }, ChargeSign::MINUS));
			PostRedisplay();
		}
	}
}

void CMyApplication::OnKeyboard(unsigned char key, int x, int y)
{
	if (key == 127) 
	{
		m_chargedParticles.clear();
		PostRedisplay();
	}
}

std::optional<size_t> CMyApplication::FindParticleAtPoint(Point point)
{
	for (size_t i = 0; i < m_chargedParticles.size(); i++)
	{
		if (m_chargedParticles[i].IsPointBelongsToParticle(point))
		{
			return i;
		}
	}
	return std::nullopt;
}

std::optional<size_t> CMyApplication::FindClosestParticle(size_t index)
{
	if (m_chargedParticles.size() <= 1)
	{
		return std::nullopt;
	}
	std::vector<float> distances;
	for (size_t i = 0; i < m_chargedParticles.size(); i++)
	{
		if (i != index)
		{
			distances.push_back(m_chargedParticles[i].CalculateDistanceToParticle(m_chargedParticles[i].GetCenter(), m_chargedParticles[i].GetSign()));
		}
		else
		{
			distances.push_back(std::numeric_limits<float>::max());
		}
	}
	return min_element(distances.begin(), distances.end()) - distances.begin();
}

