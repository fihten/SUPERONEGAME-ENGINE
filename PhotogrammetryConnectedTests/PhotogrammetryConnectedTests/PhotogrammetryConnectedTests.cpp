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

int main()
{
    std::cout << "Hello World!\n";
}
