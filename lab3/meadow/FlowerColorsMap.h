#pragma once
#include <map>
#include <array>
#include <algorithm>
#include "Color.h"

const std::array<Color, 4> PETAL_COLORS{
	Color{ 247, 183, 225 }, 
	Color{ 235, 29, 2 }, 
	Color{ 250, 197, 40 },
	Color{ 56, 118, 228 }
};

const std::map<Color, Color> PETAL_COLOR_TO_CENTER_COLOR {
	{ 
		Color{ 247, 183, 225 }, Color{ 255, 3, 3 } 
	},
	{
		Color{ 235, 29, 2 }, Color{ 255, 242, 10 }
	},
	{
		Color{ 250, 197, 40 }, Color{ 252, 23, 23 } 
	},
	{
		Color{ 56, 118, 228 }, Color{ 106, 204, 188 }
	}
};