#include "CMyApplication.h"
#include <time.h>
#include "EFlowerType.h"
#include "EButterflyType.h"
#include "CRoundPetalFlower.h"
#include "COblongPetalFlower.h"
#include "CLilyFlower.h"
#include "CGrassStems.h"
#include "CGrassFern.h"
#include "CButterfly.h"
#include "FlowerColorsMap.h"
#include "Utils.h"

CMyApplication* CMyApplication::m_pApp = NULL;

CMyApplication::CMyApplication(const char* title, int width, int height, bool needDepth)
	: CGLApplication(title, width, height, needDepth)
	, HEIGHT_START_OF_FIELD(height / 2.5)
	, m_width(width)
	, m_height(height)
	, LILY_WIDTH(m_width * 0.02)
	, LILY_HEIGHT(m_height * 0.08)
	, m_sunCenterX(m_width / 2)
	, m_sunCenterY(m_height / 12.5)
{
	m_pApp = this;
	m_cloudPositions[0] = { (int)round(m_width + m_width / 3), m_height / 5 };
	m_cloudPositions[1] = { (int)round(m_width / 4), (int)round(m_height / 12.5) };
	m_cloudPositions[2] = { m_width / 12, m_height / 7 };
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	// Задаем цвет очистки буфера кадра
	glClearColor(1, 1, 1, 1);

	CreateFlowers();
	CreateGrass();
	CreateButterflies();

	SetTimer(100, AnimateClouds, 0);
}

void CMyApplication::OnDisplay(void)
{
	// Очищаем буфер цвета и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawSky();
	DrawField();

	for (const auto& grass : m_grass)
	{
		grass->Draw();
	}

	for (const auto& flower : m_flowers)
	{
		flower->Draw();
	}

	for (const auto& butterfly : m_butterflies)
	{
		butterfly->Draw();
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

void CMyApplication::CreateFlowers()
{
	const int FLOWER_TYPES_COUNT = 2;
	const int PETAL_COLOR_COUNT = PETAL_COLORS.size();

	const int MIN_PETAL_COUNT = 5;
	const int MAX_PETAL_COUNT = 7;
	const int PETAL_COUNT_IN_RAND = MAX_PETAL_COUNT - MIN_PETAL_COUNT;
	const int MIN_MAX_RADIUS = m_width / 38;
	const int MAX_MAX_RADIUS = m_width / 28;
	const int COUNT_OF_MAX_RADIUS_IN_RAND = MAX_MAX_RADIUS - MIN_MAX_RADIUS;

	const float PROPORTION_OF_WIDTH_AT_HEIGHT = 0.9;
	const int MIN_WIDTH = m_width / 28;
	const int MAX_WIDTH = m_width / 20;
	const int COUNT_OF_WIDTH_IN_RAND = MAX_WIDTH - MIN_WIDTH;

	FlowerType type;
	int width, height, numberOfPetalColor, petalCount, minRadius, maxRadius, minMinRadius, countOfMinRadiusInRand;

	srand(time(0));
	for (size_t i = 0; i <= 150; i++)
	{
		type = static_cast<FlowerType>(rand() % FLOWER_TYPES_COUNT);
		numberOfPetalColor = rand() % PETAL_COLOR_COUNT;
		
		if (type == FlowerType::ROUND_PETALS)
		{
			width = rand() % COUNT_OF_WIDTH_IN_RAND + MIN_WIDTH;
			height = width * PROPORTION_OF_WIDTH_AT_HEIGHT;

			m_positionGenerator.SetStartXRange(width / 2);
			m_positionGenerator.SetEndXRange(m_width - width / 2);

			m_positionGenerator.SetStartYRange(HEIGHT_START_OF_FIELD + height / 2);
			m_positionGenerator.SetEndYRange(m_height - LILY_HEIGHT - height / 2);

			m_flowers.push_back(std::make_unique<CRoundPetalFlower>(m_positionGenerator.GenerateXPosition(), m_positionGenerator.GenerateYPosition(), width, height, numberOfPetalColor));
		}
		else if (type == FlowerType::OBLONG_PETALS)
		{
			petalCount = rand() % PETAL_COUNT_IN_RAND + MIN_PETAL_COUNT;
			maxRadius = rand() % COUNT_OF_MAX_RADIUS_IN_RAND + MIN_MAX_RADIUS;

			minMinRadius = maxRadius * 0.4;
			countOfMinRadiusInRand = maxRadius * 0.5 - minMinRadius;
			minRadius = rand() % countOfMinRadiusInRand + minMinRadius;

			m_positionGenerator.SetStartXRange(maxRadius);
			m_positionGenerator.SetEndXRange(m_width - maxRadius);

			m_positionGenerator.SetStartYRange(HEIGHT_START_OF_FIELD + maxRadius);
			m_positionGenerator.SetEndYRange(m_height - LILY_HEIGHT - maxRadius);

			m_flowers.push_back(std::make_unique<COblongPetalFlower>(petalCount, m_positionGenerator.GenerateXPosition(), m_positionGenerator.GenerateYPosition(), minMinRadius, maxRadius, numberOfPetalColor));
		}
	}

	for (size_t i = 0; i <= 15; i++)
	{
		m_positionGenerator.SetStartXRange(LILY_WIDTH * 3);
		m_positionGenerator.SetEndXRange(m_width - LILY_WIDTH * 3);

		m_positionGenerator.SetStartYRange(m_height - LILY_HEIGHT * 2);
		m_positionGenerator.SetEndYRange(m_height - LILY_HEIGHT * 1.8);

		m_flowers.push_back(std::make_unique<CLilyFlower>(m_positionGenerator.GenerateXPosition(), m_positionGenerator.GenerateYPosition(), LILY_WIDTH, LILY_HEIGHT));
	}
}

void CMyApplication::CreateGrass()
{
	for (size_t i = 0; i <= 50; i++)
	{
		m_positionGenerator.SetStartXRange(0);
		m_positionGenerator.SetEndXRange(m_width);

		m_positionGenerator.SetStartYRange(HEIGHT_START_OF_FIELD + LILY_HEIGHT * 2);
		m_positionGenerator.SetEndYRange(m_height - LILY_HEIGHT * 1.8);

		m_grass.push_back(std::make_unique<CGrassStems>(m_positionGenerator.GenerateXPosition(), m_positionGenerator.GenerateYPosition()));
	}

	for (size_t i = 0; i <= 20; i++)
	{
		m_positionGenerator.SetStartXRange(0);
		m_positionGenerator.SetEndXRange(m_width);

		m_positionGenerator.SetStartYRange(HEIGHT_START_OF_FIELD + 20);
		m_positionGenerator.SetEndYRange(HEIGHT_START_OF_FIELD + 25);

		m_grass.push_back(std::make_unique<CGrassFern>(m_positionGenerator.GenerateXPosition(), m_positionGenerator.GenerateYPosition()));
	}
}

void CMyApplication::CreateButterflies()
{
	const int BUTTERFLIES_TYPES_COUNT = 3;
	ButterflyType type;
	
	srand(time(0));
	for (size_t i = 0; i <= 25; i++)
	{
		type = static_cast<ButterflyType>(rand() % BUTTERFLIES_TYPES_COUNT);

		m_positionGenerator.SetStartXRange(0);
		m_positionGenerator.SetEndXRange(m_width);

		m_positionGenerator.SetStartYRange(m_sunCenterY + 60);
		m_positionGenerator.SetEndYRange(m_height - 50);

		m_butterflies.push_back(std::make_unique<CButterfly>(type, m_positionGenerator.GenerateXPosition(), m_positionGenerator.GenerateYPosition()));
	}
}

void CMyApplication::DrawField() const
{
	glColor3ub(150, 201, 38);

	glBegin(GL_QUADS);
	glVertex2f(0, m_height / 2.5);
	glVertex2f(m_width, m_height / 2.5);
	glVertex2f(m_width, m_height);
	glVertex2f(0, m_height);
	glEnd();
}

void CMyApplication::DrawSky() const
{
	DrawSkyBackground();
	DrawSun();

	DrawCloud(m_cloudPositions[0].x, m_cloudPositions[0].y, m_width / 8.5, m_height / 20);
	DrawCloud(m_cloudPositions[1].x, m_cloudPositions[1].y, m_width / 8, m_height / 18.18);
	DrawCloud(m_cloudPositions[2].x, m_cloudPositions[2].y, m_width / 10.5, m_height / 24.37);
}

void CMyApplication::DrawSkyBackground() const
{
	glColor3ub(148, 208, 242);

	glBegin(GL_QUADS);

	glVertex2i(0, 0);
	glVertex2i(m_width, 0);
	glVertex2i(m_width, HEIGHT_START_OF_FIELD);
	glVertex2i(0, HEIGHT_START_OF_FIELD);

	glEnd();
}

void CMyApplication::DrawSun() const
{
	glColor3ub(247, 232, 99);
	FillEllipse(m_sunCenterX, m_sunCenterY, m_width / 25.7, m_height / 20);
}

void CMyApplication::DrawCloud(int x, int y, int width, int height) const
{
	glColor3ub(242, 245, 243);
	FillEllipse(x + width / 10.6, y + height / 3, width / 8.15, height / 2.82);
	FillEllipse(x + width / 3.75, y + height / 3.875, width / 5.88, height / 1.9375);
	FillEllipse(x + width / 1.77, y + height / 6.2, width / 4.08, height / 1.30);
	FillEllipse(x + width / 1.177, y + height / 3.875, width / 6.625, height / 2.21);
}

void CMyApplication::AnimateClouds(int value)
{
	const int SPEED = 10;
	const int RESERVE = 300;

	for (size_t i = 0; i <= m_pApp->GetCountOfClouds() - 1; i++)
	{
		m_pApp->DecrementCloudXPosAtIndex(i, SPEED);
		if (m_pApp->GetCloudPosXAtIndex(i) < -RESERVE)
		{
			m_pApp->SetCloudXPosAtIndex(i, m_pApp->GetWindowWidth() + RESERVE);
		}
	}
	m_pApp->PostRedisplay();
	m_pApp->SetTimer(100, AnimateClouds, 0);
}

void CMyApplication::DecrementCloudXPosAtIndex(size_t i, int x)
{
	m_cloudPositions[i].x -= x;
}

void CMyApplication::SetCloudXPosAtIndex(size_t i, int x)
{
	m_cloudPositions[i].x = x;
}

int CMyApplication::GetCloudPosXAtIndex(size_t i) const
{
	return m_cloudPositions[i].x;
}

int CMyApplication::GetWindowWidth() const
{
	return m_width;
}

size_t CMyApplication::GetCountOfClouds() const
{
	return m_cloudPositions.size();
}

