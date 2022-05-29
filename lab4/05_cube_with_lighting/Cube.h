#pragma once

enum class CubeSide
{
	REAR_UPPER_LEFT_TRIANGLE,
	REAR_LOWER_LEFT_TRIANGLE,
	REAR_UPPER_RIGHT_TRIANGLE,
	REAR_LOWER_RIGHT_TRIANGLE,
	FRONT_UPPER_LEFT_TRIANGLE,
	FRONT_LOWER_LEFT_TRIANGLE,
	FRONT_UPPER_RIGHT_TRIANGLE,
	FRONT_LOWER_RIGHT_TRIANGLE,
	BACK_SQUARE,
	LEFT_SQUARE,
	RIGHT_SQUARE,
	FRONT_SQUARE,
	UPPER_SQUARE,
	BOTTOM_SQUARE,
};

class Cube
{
public:
	explicit Cube(float size = 1);
	void Draw() const;

	void SetSideColor(CubeSide side, const glm::vec4& color);
	void SetSpecularColor(glm::vec4 color);
	void SetShininess(float shininess);
	void SetSize(float size);

private:
	static const int TRIANGULAR_FACE_COUNT = 8;
	static const int SQUARE_FACE_COUNT = 6;

	float m_size;
	glm::vec4 m_sideColors[TRIANGULAR_FACE_COUNT + SQUARE_FACE_COUNT];
	glm::vec4 m_specularColor = { 0, 0, 0, 1 };
	float m_shininess = 1;

	static constexpr float vertices[12][3] = {
		{ -1, -1, 0 }, // 0
		{ -1, 0, +1 }, // 1
		{ -1, +1, 0 }, // 2
		{ -1, 0, -1 }, // 3
		{ 0, +1, +1 }, // 4
		{ 0, -1, +1 }, // 5
		{ +1, 0, +1 }, // 6
		{ +1, -1, 0 }, // 7
		{ +1, +1, 0 }, // 8
		{ +1, 0, -1 }, // 9
		{ 0, -1, -1 }, // 10
		{ 0, +1, -1 }, // 11
	};

	void DrawFace(size_t colorIndex, const unsigned char* facePoints, bool isSquare = false) const;
	void DrawEdges(const unsigned char* facePoints) const;
};
