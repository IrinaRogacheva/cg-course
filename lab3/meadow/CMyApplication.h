#pragma once
#include "CGLApplication.h"
#include "IFlower.h"
#include "IGrass.h"
#include "IButterfly.h"
#include "CPositionGenerator.h"
#include "Point.h"
#include <vector>
#include <memory>
#include <array>

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char* title, int width = 0, int height = 0, bool needDepth = true);
	~CMyApplication(void);
protected:
	// Перегружаем необходимые виртуальные методы родительского класса
	virtual void OnInit();
	virtual void OnDisplay(void);
	virtual void OnReshape(int width, int height);
private:
	void CreateFlowers();
	void CreateGrass();
	void CreateButterflies();

	void DrawField() const;
	void DrawSky() const;
	void DrawSkyBackground() const;
	void DrawSun() const;
	void DrawCloud(int x, int y, int width, int height) const;

	
	static void AnimateClouds(int value);
	void DecrementCloudXPosAtIndex(size_t i, int x);
	void SetCloudXPosAtIndex(size_t i, int x);
	int GetCloudPosXAtIndex(size_t i) const;
	int GetWindowWidth() const;
	size_t GetCountOfClouds() const;

	int m_width;
	int m_height;
	int m_sunCenterX;
	int m_sunCenterY;
	std::array<Point, 3> m_cloudPositions {};

	const int HEIGHT_START_OF_FIELD;
	const int LILY_WIDTH;
	const int LILY_HEIGHT;

	static CMyApplication* m_pApp;
	std::vector<std::unique_ptr<IFlower>> m_flowers;
	std::vector<std::unique_ptr<IGrass>> m_grass;
	std::vector<std::unique_ptr<IButterfly>> m_butterflies;
	CPositionGenerator m_positionGenerator;
};
