#include "pch.h"
#include "Cube.h"

Cube::Cube(float size)
	: m_size(size)
{
	constexpr glm::vec4 defaultColor{ 1, 1, 1, 1 };
	SetSideColor(CubeSide::REAR_UPPER_LEFT_TRIANGLE, defaultColor);
	SetSideColor(CubeSide::REAR_LOWER_LEFT_TRIANGLE, defaultColor);
	SetSideColor(CubeSide::REAR_UPPER_RIGHT_TRIANGLE, defaultColor);
	SetSideColor(CubeSide::REAR_LOWER_RIGHT_TRIANGLE, defaultColor);
	SetSideColor(CubeSide::FRONT_UPPER_LEFT_TRIANGLE, defaultColor);
	SetSideColor(CubeSide::FRONT_LOWER_LEFT_TRIANGLE, defaultColor);
	SetSideColor(CubeSide::FRONT_UPPER_RIGHT_TRIANGLE, defaultColor);
	SetSideColor(CubeSide::FRONT_LOWER_RIGHT_TRIANGLE, defaultColor);
	SetSideColor(CubeSide::BACK_SQUARE, defaultColor);
	SetSideColor(CubeSide::LEFT_SQUARE, defaultColor);
	SetSideColor(CubeSide::RIGHT_SQUARE, defaultColor);
	SetSideColor(CubeSide::FRONT_SQUARE, defaultColor);
	SetSideColor(CubeSide::UPPER_SQUARE, defaultColor);
	SetSideColor(CubeSide::BOTTOM_SQUARE, defaultColor);
}

void Cube::Draw() const
{
	static constexpr unsigned char triangularFaces[TRIANGULAR_FACE_COUNT][3] = {
		{ 5, 1, 0 }, // задний верхний левый треугольник
		{ 10, 0, 3 }, // задний нижний левый треугольник
		{ 1, 4, 2 }, // задний верхний правый треугольник
		{ 3, 2, 11 }, // задний нижний правый треугольник
		{ 7, 6, 5 }, // передний верхний левый треугольник
		{ 7, 10, 9 }, // передний нижний левый треугольник
		{ 6, 8, 4 }, // передний верхний правый треугольник
		{ 8, 9, 11 }, // передний нижний правый треугольник
	};
	static constexpr unsigned char squareFaces[SQUARE_FACE_COUNT][4] = {
		{ 0, 1, 2, 3 }, // задний квадрат
		{ 7, 5, 0, 10 }, // левый квадрат
		{ 8, 11, 2, 4 }, // правый квадрат
		{ 7, 9, 8, 6 }, // передний квадрат
		{ 5, 6, 4, 1 }, // верхний квадрат
		{ 10, 3, 11, 9 }, // нижний квадрат
	};

	glEnable(GL_COLOR_MATERIAL);
	// Цвет вершины будет управлять диффузным и фоновым цветом материала
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(m_specularColor));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
	glBegin(GL_TRIANGLES);
	{
		for (size_t face = 0; face < TRIANGULAR_FACE_COUNT; ++face)
		{
			const unsigned char* facePoints = triangularFaces[face];
			DrawFace(face, facePoints);
		}
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		for (size_t face = 0; face < SQUARE_FACE_COUNT; ++face)
		{
			const unsigned char* facePoints = squareFaces[face];
			const size_t OFFSET = TRIANGULAR_FACE_COUNT - 1;
			DrawFace(face + OFFSET, facePoints, true);
		}
	}
	glEnd();

	for (size_t face = 0; face < TRIANGULAR_FACE_COUNT; ++face)
	{
		const unsigned char* facePoints = triangularFaces[face];
		DrawEdges(facePoints);
	}
}

void Cube::SetSideColor(CubeSide side, const glm::vec4& color)
{
	unsigned index = static_cast<unsigned>(side);
	m_sideColors[index] = color;
}

void Cube::SetSpecularColor(glm::vec4 color)
{
	m_specularColor = color;
}

void Cube::SetShininess(float shininess)
{
	m_shininess = shininess;
}

void Cube::SetSize(float size)
{
	m_size = size;
}

void Cube::DrawFace(size_t colorIndex, const unsigned char* facePoints, bool isSquare) const
{
	// устанавливаем диффузный и фоновый цвет грани
	glColor4fv(glm::value_ptr(m_sideColors[colorIndex]));

	// получаем вершины грани
	auto p0 = glm::make_vec3(vertices[facePoints[0]]);
	auto p1 = glm::make_vec3(vertices[facePoints[1]]);
	auto p2 = glm::make_vec3(vertices[facePoints[2]]);
	auto p3 = glm::make_vec3(vertices[facePoints[3]]);
	// Вычисляем координаты вершин с учетом его размера
	p0 *= m_size * 0.5f;
	p1 *= m_size * 0.5f;
	p2 *= m_size * 0.5f;
	p3 *= m_size * 0.5f;

	// Вычисляем нормаль к грани через
	// векторное произведение его смежных сторон
	auto v01 = p1 - p0;
	auto v02 = p2 - p0;
	auto normal = glm::normalize(glm::cross(v01, v02));

	glNormal3fv(glm::value_ptr(normal));

	glVertex3fv(glm::value_ptr(p0));
	glVertex3fv(glm::value_ptr(p1));
	glVertex3fv(glm::value_ptr(p2));
	if (isSquare)
	{
		glVertex3fv(glm::value_ptr(p3));
	}
}

void Cube::DrawEdges(const unsigned char* facePoints) const
{
	glColor4f(0, 0, 0, 1);
	glLineWidth(2);
	
	glBegin(GL_LINE_LOOP);
	glm::vec3 p[3];
	for (auto i = 0; i < 3; i++)
	{
		p[i] = glm::make_vec3(vertices[facePoints[i]]) * m_size * 0.5f;
	}

	for (auto i = 0; i < 3; i++)
	{
		glVertex3fv(glm::value_ptr(p[i]));
	}
	glEnd();
}
