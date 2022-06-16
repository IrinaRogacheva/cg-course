#include "stdafx.h"
#include "Solve4.h"

EquationRoot4 Solve2(double a, double b, double c)
{
	double x1 = 0, x2 = 0;
	double discrim = pow(b, 2) - 4 * a * c;
	if (discrim == 0)
	{
		x2 = (-b) / (2 * a);
		return { 1, { x1, 0, 0, 0 } };
	}
	if (discrim > 0)
	{
		x1 = (-b + sqrt(discrim)) / (2 * a);
		x2 = (-b - sqrt(discrim)) / (2 * a);
		return { 2, { x1, x2, 0, 0 } };
	}
	return { 0, { 0, 0, 0, 0 } };
}

EquationRoot4 Solve3(double a, double b, double c, double d)
{
	double x1 = 0, x2 = 0, x3 = 0;
	b /= a;
	c /= a;
	d /= a;
	double p = (pow(b, 2) - 3 * c) / 9;
	double q = (2 * pow(b, 3) - 9 * b * c + 27 * d) / 54;
	double s = (pow(p, 3)) - (pow(q, 2));
	double phi;

	if (s > 0)
	{
		phi = acos(q / sqrt(pow(p, 3))) / 3;
		x1 = (-2) * sqrt(p) * cos(phi) - b / 3;
		x2 = (-2) * sqrt(p) * cos(phi + M_2PI / 3) - b / 3;
		x3 = (-2) * sqrt(p) * cos(phi - M_2PI / 3) - b / 3;
		return { 3, { x2, x1, x3, 0 } };
	}

	if (s < 0)
	{
		int k = 1;
		if (q < 0.)
		{
			q = -q;
			k = -1;
		}
		if (p > 0)
		{
			phi = 1. / 3. * acosh(q / sqrt(pow(p, 3)));
			x1 = -2 * k * sqrt(p) * cosh(phi) - b / 3;
		}
		else
		{
			phi = 1. / 3. * asinh(q / sqrt(pow(abs(p), 3)));
			x1 = -2 * k * sqrt(abs(p)) * sinh(phi) - b / 3;
		}
		return { 1, { x1, 0, 0, 0 } };
	}

	x1 = (-2) * pow(q, 1. / 3.) - b / 3;
	x2 = pow(q, 1. / 3.) - b / 3;
	return { 2, { x1, x2, 0, 0 } };
}

EquationRoot4 CreateAndSolveEquation3(double a4, double b4, double c4, double d4)
{
	double b = -b4;
	double c = a4 * c4 - 4 * d4;
	double d = -pow(a4, 2) * d4 + 4 * b4 * d4 - pow(c4, 2);
	return Solve3(1, b, c, d);
}

EquationRoot4 CreateAndSolveEquation2(double a4, double b4, double c4, double d4, double y, double sign)
{
	double k = 1;
	double b, c;
	if (a4 / 2 * y - c4 < 0)
	{
		k = -1;
	}
	b = a4 / 2 + sign * sqrt(pow(a4, 2) / 4 - b4 + y);
	c = y / 2 + sign * k * sqrt(pow(y, 2) / 4 - d4);
	return Solve2(1, b, c);
}

EquationRoot4 Solve4(double a, double b, double c, double d, double e)
{
	if (a == 0)
	{
		//throw std::invalid_argument("The coefficient at x^4 cannot be equal to 0");
	}
	b /= a;
	c /= a;
	d /= a;
	e /= a;
	double root3 = CreateAndSolveEquation3(b, c, d, e).roots[0];
	EquationRoot4 equationRoot = CreateAndSolveEquation2(b, c, d, e, root3, 1);
	EquationRoot4 equationRoot2 = CreateAndSolveEquation2(b, c, d, e, root3, -1);
	int index = (int)equationRoot.numRoots;
	for (size_t i = 0; i < equationRoot2.numRoots; i++)
	{
		equationRoot.roots[index + i] = equationRoot2.roots[i];
	}
	equationRoot.numRoots += equationRoot2.numRoots;
	if (equationRoot.numRoots == 0)
	{
		//throw std::domain_error("no valid roots");
	}
	return equationRoot;
}