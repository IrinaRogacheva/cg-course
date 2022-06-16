#pragma once

#include <iostream>
#include <math.h>
#define M_PI (3.141592653589793)
#define M_2PI (2. * M_PI)

struct EquationRoot4
{
	double numRoots;
	double roots[4];
};

EquationRoot4 Solve2(double a, double b, double c);
EquationRoot4 Solve3(double a, double b, double c, double d);
EquationRoot4 CreateAndSolveEquation3(double a4, double b4, double c4, double d4);
EquationRoot4 CreateAndSolveEquation2(double a4, double b4, double c4, double d4, double y, double sign);
EquationRoot4 Solve4(double a, double b, double c, double d, double e);