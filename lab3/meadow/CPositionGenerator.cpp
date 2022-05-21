#include "CPositionGenerator.h"

CPositionGenerator::CPositionGenerator()
	: m_rand_dev()
{
	std::mt19937 m_generator(m_rand_dev());
}

void CPositionGenerator::SetStartXRange(int startXRange)
{
	m_startRangeX = startXRange;
}

void CPositionGenerator::SetEndXRange(int endXRange)
{
	m_endRangeX = endXRange;
}

void CPositionGenerator::SetStartYRange(int startYRange)
{
	m_startRangeY = startYRange;
}

void CPositionGenerator::SetEndYRange(int startYRange)
{
	m_endRangeY = startYRange;
}

int CPositionGenerator::GenerateXPosition()
{
	std::uniform_int_distribution<int> distr(m_startRangeX, m_endRangeX);
	return distr(m_generator);
}

int CPositionGenerator::GenerateYPosition()
{
	std::uniform_int_distribution<int> distr(m_startRangeY, m_endRangeY);
	return distr(m_generator);
}
