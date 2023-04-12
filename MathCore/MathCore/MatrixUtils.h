#pragma once
#include <limits>

int* nextTransposition(int* currentTransposition, int count, int& even);

template<class T, int rows = 4>
T det(T const* matrix, int i, int j)
{
	if (i < 0 || rows - 1 < i)
		return DBL_MAX;
	if (j < 0 || rows - 1 < j)
		return DBL_MAX;
	if (rows < 2)
		return 1.0;

	int transposition[rows - 1];
	int c = 0;
	for (int column = 0; column < rows; ++column)
	{
		if (column == j)
			continue;

		transposition[c++] = column;
	}

	T ret = 0;
	int even = 1;
	do
	{
		T add = even;
		c = 0;
		for (int row = 0; row < rows; ++row)
		{
			if (row == i)
				continue;

			add *= matrix[row * rows + transposition[c++]];
		}
		ret += add;
	} while (nextTransposition(transposition, rows - 1, even));

	return ret;
}

template<class T, int rows = 4>
T det(T const* matrix)
{
	int transposition[rows];
	for (int i = 0; i < rows; ++i)
		transposition[i] = i;

	T ret = 0;
	int even = 1;
	do
	{
		T add = even;
		for (int i = 0; i < rows; ++i)
			add *= matrix[i * rows + transposition[i]];
		ret += add;
	} while (nextTransposition(transposition, rows, even));

	return ret;
}