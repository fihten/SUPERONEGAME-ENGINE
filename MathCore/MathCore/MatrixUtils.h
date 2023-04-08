#pragma once

int* nextTransposition(int* currentTransposition, int count, int& even);

template<class T, int rows = 4>
T det(T matrix[], int i, int j);

template<class T, int rows = 4>
T det(T matrix[]);