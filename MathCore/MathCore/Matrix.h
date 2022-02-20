#pragma once

template<class T>
class Matrix
{
public:
	Matrix();
	Matrix(int rows, int columns);
	Matrix(const T& a00);
	Matrix(
		const T& a00, const T& a01,
		const T& a10, const T& a11
	);
	Matrix(
		const T& a00, const T& a01, const T& a02,
		const T& a10, const T& a11, const T& a12,
		const T& a20, const T& a21, const T& a22
	);
	Matrix(
		const T& a00, const T& a01, const T& a02, const T& a03,
		const T& a10, const T& a11, const T& a12, const T& a13,
		const T& a20, const T& a21, const T& a22, const T& a23,
		const T& a30, const T& a31, const T& a32, const T& a33
	);
	
	~Matrix();

public:
	T& operator()(int row, int column) const;
	Matrix operator+(const Matrix& m) const;
	Matrix operator-(const Matrix& m) const;
	Matrix operator*(const Matrix& m) const;
	T det() const;
	T det(int i, int j) const;
	Matrix invert() const;
	Matrix transpose() const;
	bool isValid() const;

	static Matrix makePerspectiveProjection(T aspectRatio, T fovy, T near, T far);
	static Matrix makeOrthographicProjection(T left, T right, T bottom, T top, T near, T far);
private:
	T* matrix = nullptr;
	int rows = 0;
	int columns = 0;
};
