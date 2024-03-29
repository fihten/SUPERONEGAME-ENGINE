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
	float f = 10.0f / M_PI;
	for (int i = 0; i < nx; i++)
		z += 0.25 * (1 + cos(f * (x - x0[i])));
	for (int i = 0; i < ny; i++)
		z += 0.25 * (1 + cos(f * (y - y0[i])));
	return z /= nx + ny;
}

float functionIntegral(
	float x0[], int nx, 
	float y0[], int ny, 
	float x, float hx,
	float y, float hy,
	float m00, float m01,
	float m10, float m11
	)
{
	float I = 0;
	float f = 100.0f / M_PI;
	for (int i = 0; i < nx; i++)
	{
		float I0 = 0;
		I0 += hx * hy;
		if (m00 != 0 && m10 != 0)
		{
			I0 += (-1.0f / (f * f * m00 * m10)) * cos(f * ((x + hx) * m00 + (y + hy) * m10 - x0[i]));
			I0 += (1.0f / (f * f * m00 * m10)) * cos(f * ((x + hx) * m00 + y * m10 - x0[i]));
			I0 += (1.0f / (f * f * m00 * m10)) * cos(f * (x * m00 + (y + hy) * m10 - x0[i]));
			I0 += (-1.0f / (f * f * m00 * m10)) * cos(f * (x * m00 + y * m10 - x0[i]));
		}
		else if (m00 != 0)
		{
			I0 += (hy / (f * m00)) * (sin(f * ((x + hx) * m00 - x0[i])) - sin(f * (x * m00 - x0[i])));
		}
		else if (m10 != 0)
		{
			I0 += (hx / (f * m10)) * (sin(f * ((y + hy) * m10 - x0[i])) - sin(f * (y * m10 - x0[i])));
		}
		I += 0.25 * I0;
	}
	for (int i = 0; i < ny; i++)
	{
		float I0 = 0;
		I0 += hx * hy;
		if (m01 != 0 && m11 != 0)
		{
			I0 += (-1.0f / (f * f * m01 * m11)) * cos(f * ((x + hx) * m01 + (y + hy) * m11 - y0[i]));
			I0 += (1.0f / (f * f * m01 * m11)) * cos(f * ((x + hx) * m01 + y * m11 - y0[i]));
			I0 += (1.0f / (f * f * m01 * m11)) * cos(f * (x * m01 + (y + hy) * m11 - y0[i]));
			I0 += (-1.0f / (f * f * m01 * m11)) * cos(f * (x * m01 + y * m11 - y0[i]));
		}
		else if (m01 != 0)
		{
			I0 += (hy / (f * m01)) * (sin(f * ((x + hx) * m01 - y0[i])) - sin(f * (x * m01 - y0[i])));
		}
		else if (m11 != 0)
		{
			I0 += (hx / (f * m11)) * (sin(f * ((y + hy) * m11 - y0[i])) - sin(f * (y * m11 - y0[i])));
		}
		I += 0.25 * I0;
	}
	I /= nx + ny;
	return I;
}

void fillOutTexture(
	float chanel[], 
	int width, int height,
	float m00, float m01,
	float m10, float m11,
	float x0[], int nx, float y0[], int ny
	)
{
	float hx = 1.0f / width;
	float hy = 1.0f / height;

	// fill out cells
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float x = i * hx;
			float y = j * hy;

			float I = functionIntegral(
				x0, nx,
				y0, ny,
				x, hx,
				y, hy,
				m00, m01,
				m10, m11
			);
			I /= hx * hy;
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
			float x_ = i * mInv00 + j * mInv10;
			float y_ = i * mInv01 + j * mInv11;

/*			if (x < x_corners[0])
				continue;
			if (x > x_corners[1])
				continue;
			if (y < y_corners[0])
				continue;
			if (y > y_corners[2])
				continue;*/
			float distSqr = (x - x_) * (x - x_) + (y - y_) * (y - y_);
			if (distSqr > radius * radius)
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

	int width = 1000;
	int height = 1000;

	float* textureAr = new float[width * height];
	float* textureAg = new float[width * height];
	float* textureAb = new float[width * height];

	float* textureBr = new float[width * height];
	float* textureBg = new float[width * height];
	float* textureBb = new float[width * height];

	float angle0 = 30 * M_PI / 180;
	float scale0 = 1.2;

	float angle1 = 40 * M_PI / 180;
	float scale1 = 0.8;

	float m00 = scale0 * cos(angle0); float m01 = scale0 * sin(angle0);
	float m10 = -scale1 * sin(angle1); float m11 = scale1 * cos(angle1);

	float det = m00 * m11 - m01 * m10;
	float mInv00 = m11 / det; float mInv01 = -m01 / det;
	float mInv10 = -m10 / det; float mInv11 = m00 / det;

	fillOutTexture(
		textureAr,
		width, height,
		1, 0,
		0, 1,
		x0_r, nx0_r, y0_r, ny0_r
	);
	fillOutTexture(
		textureAg,
		width, height,
		1, 0,
		0, 1,
		x0_g, nx0_g, y0_g, ny0_g
	);
	fillOutTexture(
		textureAb,
		width, height,
		1, 0,
		0, 1,
		x0_b, nx0_b, y0_b, ny0_b
	);

	fillOutTexture(
		textureBr,
		width, height,
		mInv00, mInv01,
		mInv10, mInv11,
		x0_r, nx0_r, y0_r, ny0_r
	);
	fillOutTexture(
		textureBg,
		width, height,
		mInv00, mInv01,
		mInv10, mInv11,
		x0_g, nx0_g, y0_g, ny0_g
	);
	fillOutTexture(
		textureBb,
		width, height,
		mInv00, mInv01,
		mInv10, mInv11,
		x0_b, nx0_b, y0_b, ny0_b
	);

	int r0 = 1;
	int r1 = 100;

	int xa = 500;
	int ya = 500;

	int xb = 600;
	int yb = 500;

	angle0 = 5 * M_PI / 180;
	scale0 = 1.2;

	angle1 = 40 * M_PI / 180;
	scale1 = 0.8;

	m00 = scale0 * cos(angle0); m01 = scale0 * sin(angle0);
	m10 = -scale1 * sin(angle1); m11 = scale1 * cos(angle1);

	det = m00 * m11 - m01 * m10;
	mInv00 = m11 / det; mInv01 = -m01 / det;
	mInv10 = -m10 / det; mInv11 = m00 / det;

	m01 *= (float)(height) / (float)(width);
	m10 *= (float)(width) / (float)(height);

	mInv01 *= (float)(height) / (float)(width);
	mInv10 *= (float)(width) / (float)(height);

	float x0 = 10;
	float y0 = 0;
	float x1 = 0;
	float y1 = 10;

	float x0_ = std::round(x0 * m00 + y0 * m10);
	float y0_ = std::round(x0 * m01 + y0 * m11);

	float x1_ = std::round(x1 * m00 + y1 * m10);
	float y1_ = std::round(x1 * m01 + y1 * m11);

	angle0 = std::atan2(y0_, x0_);
	scale0 = std::sqrt(x0_ * x0_ + y0_ * y0_) / 10;

	angle1 = -std::atan2(x1_, y1_);
	scale1 = std::sqrt(x1_ * x1_ + y1_ * y1_) / 10;

	m00 = scale0 * cos(angle0); m01 = scale0 * sin(angle0);
	m10 = -scale1 * sin(angle1); m11 = scale1 * cos(angle1);

	det = m00 * m11 - m01 * m10;
	mInv00 = m11 / det; mInv01 = -m01 / det;
	mInv10 = -m10 / det; mInv11 = m00 / det;

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
			m00, m01,
			m10, m11,
			mInv00, mInv01,
			mInv10, mInv11,
			xb, yb, r
		);
		IB[3 * i + 1] = calculateIntegral(
			textureBg,
			width, height,
			m00, m01,
			m10, m11,
			mInv00, mInv01,
			mInv10, mInv11,
			xb, yb, r
		);
		IB[3 * i + 2] = calculateIntegral(
			textureBb,
			width, height,
			m00, m01,
			m10, m11,
			mInv00, mInv01,
			mInv10, mInv11,
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

	float xa_tmp = xa * m00 + ya * m10;
	float ya_tmp = xa * m01 + ya * m11;

	float xb_tmp = xb * m00 + yb * m10;
	float yb_tmp = xb * m01 + yb * m11;

	xa = xa_tmp; ya = ya_tmp;
	xb = xb_tmp; yb = yb_tmp;

	for (int r = r0; r <= r1; r++)
	{
		int i = r - r0;
		IA[3 * i + 0] = calculateIntegral(
			textureAr,
			width, height,
			mInv00, mInv01,
			mInv10, mInv11,
			m00, m01,
			m10, m11,
			xa, ya, r
		);
		IA[3 * i + 1] = calculateIntegral(
			textureAg,
			width, height,
			mInv00, mInv01,
			mInv10, mInv11,
			m00, m01,
			m10, m11,
			xa, ya, r
		);
		IA[3 * i + 2] = calculateIntegral(
			textureAb,
			width, height,
			mInv00, mInv01,
			mInv10, mInv11,
			m00, m01,
			m10, m11,
			xa, ya, r
		);

		IB[3 * i + 0] = calculateIntegral(
			textureBr,
			width, height,
			1, 0,
			0, 1,
			1, 0,
			0, 1,
			xb, yb, r
		);
		IB[3 * i + 1] = calculateIntegral(
			textureBg,
			width, height,
			1, 0,
			0, 1,
			1, 0,
			0, 1,
			xb, yb, r
		);
		IB[3 * i + 2] = calculateIntegral(
			textureBb,
			width, height,
			1, 0,
			0, 1,
			1, 0,
			0, 1,
			xb, yb, r
		);
	}
	
	for (int r = r0; r <= r1; r++)
	{
		int i = r - r0;
//		fileOfX << IA[3 * i + 0] << std::endl << IA[3 * i + 1] << std::endl << IA[3 * i + 2] << std::endl;
//		fileOfY << IB[3 * i + 0] << std::endl << IB[3 * i + 1] << std::endl << IB[3 * i + 2] << std::endl;
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