#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

struct Point
{
	int x = 0;
	int y = 0;

	friend Point operator+(const Point& a, const Point& b);
	friend Point operator-(const Point& a, const Point& b);
	friend Point operator/(const Point& a, double d);
	friend Point operator*(const Point& a, const int i);
	friend bool operator>=(const Point& a, const Point& b);
	friend bool operator<=(const Point& a, const Point& b);
	friend bool operator<(const Point& a, const Point& b);
};
