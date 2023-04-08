#include "pch.h"
#include "Matrix.h"

#include <cmath>
#include <float.h>

int* nextTransposition(int* currentTransposition, int count, int& even);

template<class T>
Matrix<T>::Matrix()
{

}

template<class T>
Matrix<T>::Matrix(int rows, int columns) :rows(rows), columns(columns)
{
	if (rows > 0 && columns > 0)
		matrix = new T[rows * columns];
}

template<class T>
Matrix<T>::Matrix(const T& a00)
{
	matrix = new T[1];
	matrix[0] = a00;
}

template<class T>
Matrix<T>::Matrix(
	const T& a00, const T& a01,
	const T& a10, const T& a11
)
{
	matrix = new T[4];
	matrix[0] = a00; matrix[1] = a01;
	matrix[2] = a10; matrix[3] = a11;
}

template<class T>
Matrix<T>::Matrix(
	const T& a00, const T& a01, const T& a02,
	const T& a10, const T& a11, const T& a12,
	const T& a20, const T& a21, const T& a22
)
{
	matrix = new T[9];
	matrix[0] = a00; matrix[1] = a01; matrix[2] = a02;
	matrix[3] = a10; matrix[4] = a11; matrix[5] = a12;
	matrix[6] = a20; matrix[7] = a21; matrix[8] = a22;
}

template<class T>
Matrix<T>::Matrix(
	const T& a00, const T& a01, const T& a02, const T& a03,
	const T& a10, const T& a11, const T& a12, const T& a13,
	const T& a20, const T& a21, const T& a22, const T& a23,
	const T& a30, const T& a31, const T& a32, const T& a33
)
{
	matrix = new T[16];
	matrix[0] = a00; matrix[1] = a01; matrix[2] = a02; matrix[3] = a03;
	matrix[4] = a10; matrix[5] = a11; matrix[6] = a12; matrix[7] = a13;
	matrix[8] = a20; matrix[9] = a21; matrix[10] = a22; matrix[11] = a23;
	matrix[12] = a30; matrix[13] = a31; matrix[14] = a32; matrix[15] = a33;
}

template<class T>
Matrix<T>::~Matrix()
{
	if (matrix)
		delete []matrix;
}

template<class T>
bool Matrix<T>::isValid() const
{
	return matrix;
}

template<class T>
T& Matrix<T>::operator()(int row, int column) const
{
	int index = row * columns + column;
	return matrix[index];
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& m) const
{
	if (!isValid())
		return Matrix<T>();
	if (!m.isValid())
		return Matrix<T>();
	if (rows != m.rows)
		return Matrix<T>();
	if (columns != m.columns)
		return Matrix<T>();

	Matrix<T> ret(rows, columns);
	for (int row = 0; row < rows; ++row)
	{
		for (int column = 0; column < columns; ++column)
		{
			ret(row, column) = operator()(row, column) + m(row, column);
		}
	}

	return ret;
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& m) const
{
	if (!isValid())
		return Matrix<T>();
	if (!m.isValid())
		return Matrix<T>();
	if (rows != m.rows)
		return Matrix<T>();
	if (columns != m.columns)
		return Matrix<T>();

	Matrix<T> ret(rows, columns);
	for (int row = 0; row < rows; ++row)
	{
		for (int column = 0; column < columns; ++column)
		{
			ret(row, column) = operator()(row, column) - m(row, column);
		}
	}

	return ret;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& m) const
{
	if (!isValid())
		return Matrix<T>();
	if (!m.isValid)
		return Matrix<T>();
	if (columns != m.rows)
		return Matrix<T>();

	Matrix<T> ret(rows, m.columns);
	for (int row = 0; row < rows; ++row)
	{
		for (int column = 0; column < m.columns; ++column)
		{
			ret(row, column) = 0;
			for (int i = 0; i < columns; ++i)
			{
				ret(row, column) += operator()(row, i) * m(i, column);
			}
		}
	}

	return ret;
}

template<class T>
T Matrix<T>::det() const
{
	if (!isValid())
		return DBL_MAX;
	if (rows != columns)
		return DBL_MAX;

	int* transposition = new int[rows];
	for (int i = 0; i < rows; ++i)
		transposition[i] = i;

	T ret = 0;
	int even = 1;
	do
	{
		T add = even;
		for (int i = 0; i < rows; ++i)
			add *= operator()(i, transposition[i]);
		ret += add;
	}
	while (transposition = nextTransposition(transposition, rows, even));

	return ret;
}

template<class T>
T Matrix<T>::det(int i, int j) const
{
	if (!isValid())
		return DBL_MAX;
	if (rows != columns)
		return DBL_MAX;
	if (i < 0 || rows - 1 < i)
		return DBL_MAX;
	if (j < 0 || columns - 1 < j)
		return DBL_MAX;
	if (rows < 2)
		return 1.0;

	int* transposition = new int[rows - 1];
	int c = 0;
	for (int column = 0; column < columns; ++column)
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

			add *= operator()(row, transposition[c++]);
		}
		ret += add;
	} while (transposition = nextTransposition(transposition, columns - 1, even));

	return ret;
}

template<class T>
Matrix<T> Matrix<T>::invert() const
{
	if (!isValid())
		return Matrix<T>();
	if (rows != columns)
		return Matrix<T>();

	Matrix<T> m(rows, columns);
	
	T d = det();
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < columns; ++c)
		{
			m(r, c) = det(c, r) / d;
		}
	}

	return m;
}

template<class T>
Matrix<T> Matrix<T>::transpose() const
{
	if (!isValid())
		return Matrix<T>();
	if (rows != columns)
		return Matrix<T>();

	Matrix<T> m(rows, columns);
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < columns; ++c)
		{
			m(r, c) = operator()(c, r);
		}
	}

	return m;
}

template<class T>
Matrix<T> Matrix<T>::makePerspectiveProjection(T aspectRatio, T fovy, T near, T far)
{
	fovy = fovy * M_PI / 180;
	T tinv = 1 / tan(0.5 * fovy);
	T m22 = far / (far - near);

	return Matrix<T>(
		tinv / aspectRatio, 0, 0, 0,
		0, tinv, 0, 0,
		0, 0, m22, 0,
		0, 0, -near * m22, 0);
	 
}

template<class T>
Matrix<T> Matrix<T>::makeOrthographicProjection(T left, T right, T bottom, T top, T near, T far)
{
	return Matrix<T>(
		2.0 / (right - left), 0, 0, 0,
		0, 2.0 / (top - bottom), 0, 0,
		0, 0, 1.0 / (far - near), 0,
		-(right + left) / (right - left), -(top + bottom) / (top - bottom), -near / (far - near), 1
		);
}
