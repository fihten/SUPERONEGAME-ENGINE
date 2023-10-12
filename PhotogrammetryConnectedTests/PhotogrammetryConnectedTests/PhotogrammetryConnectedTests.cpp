#include <iostream>

#define _USE_MATH_DEFINES 
#include <math.h>

void findFourierCoefficients(
	float y[], int N, float step, 
	float fourierCoefficientsA[],
	float fourierCoefficientsB[],
	int M
)
{
	float L = N * step;
	for (int ci = 0; ci < M; ci++)
	{
		fourierCoefficientsA[ci] = 0;
		fourierCoefficientsB[ci] = 0;
		for (int ei = 0; ei < N; ei++)
		{
			float xi = ((float)ei + 0.5f) * step;

			fourierCoefficientsA[ci] += y[ei] * cos((2 * ci * M_PI * xi) / L) * step;
			
			if (ci == 0)
				continue;

			fourierCoefficientsB[ci] += y[ei] * cos((2 * ci * M_PI * xi) / L) * step;
		}
		fourierCoefficientsA[ci] *= (2.0f / L);
		fourierCoefficientsB[ci] *= (2.0f / L);
	}
}

template<class Function>
void discretizeFunction(Function function, float a, float b, float y[], int N)
{
	float step = (b - a) / N;
	for (int ei = 0; ei < N; ei++)
	{
		float xi = a + ((float)ei + 0.5f) * step;
		y[ei] = function(xi);
	}
}

float function(float x)
{
	return sin(x - 1);
}

float DfunctionDx(float x, int n)
{
	return sin(x - 1 + n * M_PI_2);
}

int main()
{
	float y[100];
	int N = sizeof y / sizeof * y;
	float a = -1.0f;
	float b = 1.0f;
	discretizeFunction(function, a, b, y, N);

	float step = (b - a) / N;
	int n = 21;
	int m = 10;

	float fourierCoefficientsA[100];
	float fourierCoefficientsB[100];

	findFourierCoefficients(
		y, n, step,
		fourierCoefficientsA,
		fourierCoefficientsB,
		m
	);

	float x10 = a + 10.5f * step;

	int maxOrderOfDerivative = 10;
	for (int order = 0; order <= maxOrderOfDerivative; order++)
	{
		float theoreticalDerivative = DfunctionDx(x10, order);
		float calculatedDerivative = order == 0 ? fourierCoefficientsA[0] / 2 : 0;
		for (int ci = 1; ci < m; ci++)
		{
			float L = b - a;
			float f = (2 * ci * M_PI) / L;
			calculatedDerivative += pow(f, order) * fourierCoefficientsA[ci] * cos(f * x10 + order * M_PI_2);
			calculatedDerivative += pow(f, order) * fourierCoefficientsB[ci] * sin(f * x10 + order * M_PI_2);
		}
	}
}
