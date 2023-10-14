#include <iostream>

#define _USE_MATH_DEFINES 
#include <math.h>
#include <algorithm>

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

// (mxx*x+myx*y)^k*(mxy*x+myy*y)^l
double findCoefficientsNearXofSpecifiedDegree(
	double mxx, double myx,
	double mxy, double myy,
	int k, int l,
	int degree
)
{
	int degree_factorial = 1;
	int i = 1;
	for (; i <= degree; i++)
		degree_factorial *= i;

	double coefficient = 0;

	// i <= k && i >= 0 && degree - i <= l && degree - i >= 0
	// ->
	// max(0, degree - l) <= i <= min(k, degree)
	int i0 = std::max<int>(0, degree - l);
	int i1 = std::min<int>(k, degree);

	int k_factorial = 1;
	double pow_myx = 1;
	i = 1;
	for (; i < k - i0 + 1; i++)
		pow_myx *= myx;
	for (; i <= k; i++)
		k_factorial *= i;

	int l_factorial = 1;
	double pow_myy = 1;
	i = 1;
	for (; i < l - degree + i0 + 1; i++)
		pow_myy *= myy;
	double pow_mxy = 1;
	for (; i <= l; i++)
	{
		pow_mxy *= mxy;
		l_factorial *= i;
	}

	int b = 1;
	double pow_mxx = 1;
	i = 0;
	for (; i < i0; i++)
	{
		pow_mxx *= mxx;
		b *= degree - i;
		b /= i + 1;
	}

	for (; i <= i1; i++)
	{
		int j = degree - i;

		if (k - i == 0)
			pow_myx = 1;
		if (j == 0)
			pow_mxy = 1;

		double fc = pow_myx * pow_myy * pow_mxx * pow_mxy;
		int ic = (b * k_factorial * l_factorial) / degree_factorial;
		coefficient += ic * fc;

		b *= j;
		b /= i + 1;

		k_factorial *= k - i;
		l_factorial /= l - j + 1;

		pow_myx = myx != 0 ? (float)(pow_myx) / (float)(myx) : pow_myx;
		pow_myy *= myy;
		pow_mxx *= mxx;
		pow_mxy = mxy != 0 ? (float)(pow_mxy) / (float)(mxy) : pow_mxy;
	}
	return coefficient;
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
	std::cout << "----------------------" << std::endl;
	int degree = 3;
	for (int i = 0; i <= degree; i++)
	{
		// (mxx*x+myx*y)^k*(mxy*x+myy*y)^l
		double c = findCoefficientsNearXofSpecifiedDegree(
			2, -1,
			3, 1,
			0, 3,
			i
		);
		std::cout << c << " * " << "x^" << i << (i < degree ? " + " : "\n");
	}
}
