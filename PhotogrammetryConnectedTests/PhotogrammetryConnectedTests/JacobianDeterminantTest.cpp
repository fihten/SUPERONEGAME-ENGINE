#include "JacobianDeterminantTest.h"

float function(float x0[], int nx, float y0[], int ny, float x, float y)
{
	float z = 1;
	for (int i = 0; i < nx; i++)
		z *= (x - x0[i]);
	for (int i = 0; i < ny; i++)
		z *= (y - y0[i]);
	return z;
}

void JacobianDeterminantTest()
{
	float x0_r[] = {
		0.121,0.324,0.76,0.436,0.21,0.976,0.32
	};
	int nx0_r = sizeof(x0_r) / sizeof(*x0_r);

	float x0_g[] = {
		0.97,0.56,0.12,0.25,0.76,0.68
	};
	int nx0_g = sizeof(x0_g) / sizeof(*x0_g);

	float x0_b[] = {
		0.56,0.03,0.35,0.87,0.99,0.34,0.54
	};
	int nx0_b = sizeof(x0_b) / sizeof(*x0_b);

	float y0_r[] = {
		0.43,0.32,0.9,0.7,0.8,0.13,0.54,0.32,0.432,0.865
	};
	int ny0_r = sizeof(y0_r) / sizeof(*y0_r);

	float y0_g[] = {
		0.56,0.23,0.745,0.84,0.354,0.645,0.123,0.243
	};
	int ny0_g = sizeof(y0_g) / sizeof(*y0_g);

	float y0_b[] = {
		0.3,0.65,0.23,0.96,0.54,0.453,0.876,0.925,0.165,0.472
	};
	int ny0_b = sizeof(y0_b) / sizeof(*y0_b);
}