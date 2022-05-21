#pragma once
#include "EFlowerType.h"
#include "Color.h"
#include <gl/glut.h>
#include <array>

class CFlower
{
public:
	CFlower(FlowerType type, int petalCount, int xCenter, int yCenter, int width, int numberOfCenterColor, int numberOfPetalColor, float angle);
	void Draw() const;
private:
	void DrawThreeLeafFlower() const;
	void DrawLongLeafsFlower() const;

	float GetRadiusByAngle(float angle) const;

	static constexpr const std::array<Color, 3> CENTER_COLORS {
		Color{255, 3, 3}, Color{255, 242, 10}, Color{252, 23, 23}
	};
	static constexpr const std::array<Color, 3> PETAL_COLORS{
		Color{247, 183, 225}, Color{235, 29, 2}, Color{250, 197, 40}
	};

	static const float MIN_RADIUS;

	int m_xCenter;
	int m_yCenter;
	int m_width;
	int m_numberOfCenterColor;
	int m_numberOfPetalColor;
	float m_angle;
	int m_petalCount;

	FlowerType m_type;
};