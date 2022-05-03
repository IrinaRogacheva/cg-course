#include "Point.h"

Point operator+(const Point& a, const Point& b)
{
	return { a.x + b.x, a.y + b.y };
}

bool operator>=(const Point& a, const Point& b)
{
	return a.x >= b.x && a.y >= b.y;
}

bool operator<=(const Point& a, const Point& b) //не длина вектора
{
	return a.x <= b.x && a.y <= b.y;
}

bool operator<(const Point& a, const Point& b) //длина вектора
{
	return pow(pow(a.x + a.y, 2), 1 / 2) < pow(pow(b.x + b.y, 2), 1 / 2);
}

Point operator-(const Point& a, const Point& b)
{
	return { a.x - b.x, a.y - b.y };
}

Point operator/(const Point& a, double d)
{
	return { (int)std::round(a.x / d), (int)std::round(a.y / d) };
}

Point operator*(const Point& a, const int i)
{
	return { a.x * i, a.y * i };
}

