#pragma once
#include <random>

class CPositionGenerator
{
public:
    CPositionGenerator();

    void SetStartXRange(int startXRange);
    void SetEndXRange(int endXRange);

    void SetStartYRange(int startYRange);
    void SetEndYRange(int startYRange);

    int GenerateXPosition();
    int GenerateYPosition();
private:
    std::random_device m_rand_dev;
    std::mt19937 m_generator;

    int m_startRangeX = 0;
    int m_endRangeX = 0;

    int m_startRangeY = 0;
    int m_endRangeY = 0;
};