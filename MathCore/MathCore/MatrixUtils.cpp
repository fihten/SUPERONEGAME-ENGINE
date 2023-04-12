#include "pch.h"
#include "MatrixUtils.h"

int* nextTransposition(int* currentTransposition, int count, int& even)
{
	if (count < 2)
		return nullptr;

	int j = count - 2;
	for (; j >= 0; --j)
	{
		if (currentTransposition[j] < currentTransposition[j + 1])
			break;
	}

	if (j < 0)
		return nullptr;

	int l = count - 1;
	for (; currentTransposition[j] > currentTransposition[l]; --l) {}

	int tmp = currentTransposition[j];
	currentTransposition[j] = currentTransposition[l];
	currentTransposition[l] = tmp;

	int n = (count - 1 - j) / 2;
	for (int k = 0; k < n; ++k)
	{
		tmp = currentTransposition[j + 1 + k];
		currentTransposition[j + 1 + k] = currentTransposition[count - 1 - k];
		currentTransposition[count - 1 - k] = tmp;
	}

	if (n % 2 == 0)
		even *= -1;

	return currentTransposition;
}
