#pragma once

struct Color
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	bool operator <(const Color& color) const
	{
		return r < color.r && g < color.g && b < color.b;
	}
};
