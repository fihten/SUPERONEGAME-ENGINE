#include "JacobianDeterminantTest.h"

#define _USE_MATH_DEFINES // for C++
#include <cmath>

#include <algorithm>
#include <string>
#include <fstream>
#include <locale>

float function(float x0[], int nx, float y0[], int ny, float x, float y)
{
	float z = 1;
	for (int i = 0; i < nx; i++)
		z += 0.25 * (1 + cos(5 * (x - x0[i]) / M_PI));
	for (int i = 0; i < ny; i++)
		z += 0.25 * (1 + cos(5 * (y - y0[i]) / M_PI));
	return z /= nx + ny;
}

void fillOutTexture(
	float chanel[], 
	int width, int height, int n,
	float m00, float m01,
	float m10, float m11,
	float x0[], int nx, float y0[], int ny
	)
{
	float hx = 1.0f / width;
	float hy = 1.0f / height;

	float dx = hx / n;
	float dy = hy / n;

	// fill out cells
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float x = i * hx;
			float y = j * hy;

			float I = 0;
			// integrate within cell
			for (int k = 0; k < n; k++)
			{
				for (int l = 0; l < n; l++)
				{
					float x_ = x + k * dx;
					float y_ = y + l * dy;

					float xOfFunctionSpace = x_ * m00 + y_ * m10;
					float yOfFunctionSpace = x_ * m01 + y_ * m11;

					float z = function(x0, nx, y0, ny, xOfFunctionSpace, yOfFunctionSpace);
					I += z;
				}
			}
			I /= n * n;
			chanel[j * width + i] = I;
		}
	}
}

float calculateIntegral(
	float chanel[],
	int width, int height,
	float m00, float m01,
	float m10, float m11,
	float mInv00, float mInv01,
	float mInv10, float mInv11,
	int x, int y, int radius
)
{
	int x_corners[4] = { x - radius, x + radius, x - radius, x + radius };
	int y_corners[4] = { y - radius, y - radius, y + radius, y + radius };

	int xl = INT_MAX;
	int xr = -INT_MAX;

	int yb = INT_MAX;
	int yt = -INT_MAX;

	for (int i = 0; i < 4; i++)
	{
		float x = x_corners[i] * m00 + y_corners[i] * m10;
		float y = x_corners[i] * m01 + y_corners[i] * m11;

		xl = std::min<int>(xl, std::floor(x));
		xr = std::max<int>(xr, std::ceil(x));

		yb = std::min<int>(yb, std::floor(y));
		yt = std::max<int>(yt, std::ceil(y));
	}

	if (xl < 0)
		return 0;
	if (xr > width - 1)
		return 0;
	if (yb < 0)
		return 0;
	if (yt > height - 1)
		return 0;

	float I = 0;
	for (int i = xl; i <= xr; i++)
	{
		for (int j = yb; j <= yt; j++)
		{
			float x = i * mInv00 + j * mInv10;
			float y = i * mInv01 + j * mInv11;

			if (x < x_corners[0])
				continue;
			if (x > x_corners[1])
				continue;
			if (y < y_corners[0])
				continue;
			if (y > y_corners[2])
				continue;

			float z = chanel[j * width + i];
			I += z;
		}
	}

	return I;
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

	int width = 1280;
	int height = 960;

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

	int n = 10;

	fillOutTexture(
		textureAr,
		width, height, n,
		1, 0,
		0, 1,
		x0_r, nx0_r, y0_r, ny0_r
	);
	fillOutTexture(
		textureAg,
		width, height, n,
		1, 0,
		0, 1,
		x0_g, nx0_g, y0_g, ny0_g
	);
	fillOutTexture(
		textureAb,
		width, height, n,
		1, 0,
		0, 1,
		x0_b, nx0_b, y0_b, ny0_b
	);

	fillOutTexture(
		textureBr,
		width, height, n,
		m00, m01,
		m10, m11,
		x0_r, nx0_r, y0_r, ny0_r
	);
	fillOutTexture(
		textureBg,
		width, height, n,
		m00, m01,
		m10, m11,
		x0_g, nx0_g, y0_g, ny0_g
	);
	fillOutTexture(
		textureBb,
		width, height, n,
		m00, m01,
		m10, m11,
		x0_b, nx0_b, y0_b, ny0_b
	);

	int r0 = 1;
	int r1 = 100;

	int xa = 400;
	int ya = 480;

	int xb = 400;
	int yb = 480;

	m01 *= (float)(height) / (float)(width);
	m10 *= (float)(width) / (float)(height);

	mInv01 *= (float)(height) / (float)(width);
	mInv10 *= (float)(width) / (float)(height);

	float* IA = new float[3 * (r1 - r0 + 1)];
	float* IB = new float[3 * (r1 - r0 + 1)];
	for (int r = r0; r <= r1; r++)
	{
		int i = r - r0;
		IA[3 * i + 0] = calculateIntegral(
			textureAr,
			width, height,
			1, 0,
			0, 1,
			1, 0,
			0, 1,
			xa, ya, r
		);
		IA[3 * i + 1] = calculateIntegral(
			textureAg,
			width, height,
			1, 0,
			0, 1,
			1, 0,
			0, 1,
			xa, ya, r
		);
		IA[3 * i + 2] = calculateIntegral(
			textureAb,
			width, height,
			1, 0,
			0, 1,
			1, 0,
			0, 1,
			xa, ya, r
		);

		IB[3 * i + 0] = calculateIntegral(
			textureBr,
			width, height,
			mInv00, mInv01,
			mInv10, mInv11,
			m00, m01,
			m10, m11,
			xb, yb, r
		);
		IB[3 * i + 1] = calculateIntegral(
			textureBg,
			width, height,
			mInv00, mInv01,
			mInv10, mInv11,
			m00, m01,
			m10, m11,
			xb, yb, r
		);
		IB[3 * i + 2] = calculateIntegral(
			textureBb,
			width, height,
			mInv00, mInv01,
			mInv10, mInv11,
			m00, m01,
			m10, m11,
			xb, yb, r
		);
	}

	struct Comma final : std::numpunct<char>
	{
		char do_decimal_point() const override { return ','; }
	};
	std::ofstream fileOfX("x.txt");
	std::ofstream fileOfY("y.txt");
	fileOfX.imbue(std::locale(std::locale::classic(), new Comma));
	fileOfY.imbue(std::locale(std::locale::classic(), new Comma));
	
	for (int r = r0; r <= r1; r++)
	{
		int i = r - r0;
		fileOfX << IA[3 * i + 0] << std::endl << IA[3 * i + 1] << std::endl << IA[3 * i + 2] << std::endl;
		fileOfY << IB[3 * i + 0] << std::endl << IB[3 * i + 1] << std::endl << IB[3 * i + 2] << std::endl;
	}

	delete[]IA;
	delete[]IB;

	delete[]textureAr;
	delete[]textureAg;
	delete[]textureAb;
	
	delete[]textureBr;
	delete[]textureBg;
	delete[]textureBb;
}