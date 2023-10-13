#include <iostream>

#define _USE_MATH_DEFINES 
#include <math.h>

void findFourierCoefficientsOfDerivative(
	double y[], int N, double step, 
	double fourierCoefficientsNearCos[],
	int M
)
{
	double L = N * step;
	fourierCoefficientsNearCos[0] = 2.0f * (y[N - 1] - y[0]) / L;
	double s = -1;
	for (int ci = 1; ci < M; ci++)
	{
		fourierCoefficientsNearCos[ci] = (y[N - 1] - y[0]) * s;
		s *= -1;
		double f = (ci * M_PI) / L;
		for (int ei = 1; ei < N; ei++)
		{
			double xi = ((double)ei + 0.5f) * step;
			fourierCoefficientsNearCos[ci] += f * (y[ei] - y[0]) * sin(f * xi) * step;
		}
		fourierCoefficientsNearCos[ci] *= (2.0f / L);
	}
}

template<class Function>
void discretizeFunction(Function function, double a, double b, double y[], int N)
{
	double step = (b - a) / N;
	for (int ei = 0; ei < N; ei++)
	{
		double xi = a + ((double)ei + 0.5f) * step;
		y[ei] = function(xi);
	}
}

double function(double x)
{
	return sin(x - 1);
}

double DfunctionDx(double x, int n)
{
	return sin(x - 1 + n * M_PI_2);
}

int main()
{
	double y[1000];
	int N = sizeof y / sizeof * y;
	double a = -1.0f;
	double b = 1.0f;
	discretizeFunction(function, a, b, y, N);

	double step = (b - a) / N;
	int n = 201;
	int m = 100;

	double fourierCoefficientsNearCos[400];
	findFourierCoefficientsOfDerivative(
		y, n, step,
		fourierCoefficientsNearCos,
		m
	);

	double x = a;

	double theoreticalDerivative = DfunctionDx(x, 1);
	double calculatedDerivative = fourierCoefficientsNearCos[0] / 2;
	for (int ci = 1; ci < m; ci++)
	{
		double L = step * (float)(n);
		double f = (ci * M_PI) / L;
		calculatedDerivative += fourierCoefficientsNearCos[ci] * cos(f *x);
	}
	std::cout << "----------------------" << std::endl;
	std::cout << "theoretical derivative " << theoreticalDerivative << std::endl;
	std::cout << "calculated derivative " << calculatedDerivative << std::endl;
	std::cout << "error " << abs(theoreticalDerivative - calculatedDerivative) << std::endl;
}
