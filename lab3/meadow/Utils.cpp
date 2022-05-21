#include "Utils.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <gl/glut.h>
#include <random>

void FillEllipse(float xCenter, float yCenter, float rx, float ry, int points)
{
	const float step = 2 * M_PI / points;

	// ������ ����������� � ���� "�����" �� �������������
	glBegin(GL_TRIANGLE_FAN);
	// ��������� ����� ����� ������������� � ������ �������
	glVertex2d(xCenter, yCenter);
	// ��������� ����� - ���������� �� ��� �������
	for (float angle = 0; angle <= 2 * M_PI; angle += step)
	{
		float a = (fabsf(angle - 2 * M_PI) < 1e-5) ? 0 : angle;
		const float dx = rx * cosf(a);
		const float dy = ry * sinf(a);
		glVertex2f(dx + xCenter, dy + yCenter);
	}
	glEnd();
}

Color GenerateRandomColor()
{
	return { (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256) };
}