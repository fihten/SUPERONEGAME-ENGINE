#include "JacobianDeterminantTest.h"

#define _USE_MATH_DEFINES // for C++
#include <cmath>

float function(float x0[], int nx, float y0[], int ny, float x, float y)
{
	float z = 1;
	for (int i = 0; i < nx; i++)
		z *= (x - x0[i]);
	for (int i = 0; i < ny; i++)
		z *= (y - y0[i]);
	return z;
}

void fillOutTexture(
	float r[], float g[], float b[], 
	int width, int height, int n,
	float m00, float m01,
	float m10, float m11
	)
{
	float hx = 1.0f / width;
	float hy = 1.0f / height;

	float dx = hx / n;
	float dy = hy / n;
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

	int width = 640;
	int height = 480;

	float* textureAr = new float[width * height];
	float* textureAg = new float[width * height];
	float* textureAb = new float[width * height];

	float* textureBr = new float[width * height];
	float* textureBg = new float[width * height];
	float* textureBb = new float[width * height];

	float angle0 = 20 * M_PI / 180;
	float scale0 = 0.8;

	float angle1 = -10 * M_PI / 180;
	float scale1 = 1.2;

	float m00 = scale0 * cos(angle0); float m01 = scale0 * sin(angle0);
	float m10 = -scale1 * sin(angle1); float m11 = scale1 * cos(angle1);

	float det = m00 * m11 - m01 * m10;
	float mInv00 = m11 / det; float mInv01 = -m01 / det;
	float mInv10 = -m10 / det; float mInv11 = m00 / det;

	

	int r0 = 10;
	int r1 = 20;

}