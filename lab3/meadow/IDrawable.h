#pragma once

class IDrawable
{
public:
	virtual void Draw() const = 0;

	virtual ~IDrawable() = default;
};