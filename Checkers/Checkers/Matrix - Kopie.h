#pragma once
#include <iostream>
#include <cstdlib>

template<int rows, int columns>
class Matrix {
	
public:

	float val[rows][columns];
	
	Matrix() {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < columns; ++j) {
				val[i][j] = 0;
			}
		}
	}

	void print() {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < columns; ++j) {
				std::cout << val[i][j];
				if (j == columns - 1) {
					std::cout << "\n";
					break;
				}
				std::cout << ", ";
			}
		}
	}

	//Hadamard Product
	Matrix<rows, columns> hadamard(const Matrix<rows, columns>& m) const {
		Matrix<rows, columns> result = Matrix<rows, columns>();
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < columns; ++j) {
				result.val[i][j] = val[i][j] * m.val[i][j];
			}
		}
		return result;
	}

	//apply function to elements
	void apply(void (*func)(float& val, bool der), bool derivative = false) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < columns; ++j) {
				func(val[i][j], derivative);
			}
		}
	}

	Matrix<columns, rows> transpose() const {
		Matrix<columns, rows> result = Matrix<columns, rows>();
		for (int i = 0; i < columns; ++i) {
			for (int j = 0; j < rows; ++j) {
				result.val[i][j] = val[j][i];
			}
		}
		return result;
	}

	//sets every entry to zero
	void clear() {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < columns; ++j) {
				val[i][j] = 0;
			}
		}
	}

	static Matrix<rows, columns> random() {
		Matrix<rows, columns> result = Matrix<rows, columns>();
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < columns; ++j) {
				result.val[i][j] = ((float)rand() / RAND_MAX * 2.0) - 1.0;
			}
		}
		return result;
	}
};

//Multiplication
template<int r1, int c1, int r2, int c2>
Matrix<r1, c2> operator*(const Matrix<r1, c1>& m1, const Matrix<r2, c2>& m2) {

	static_assert(c1 == r2);
	Matrix<r1, c2> result = Matrix<r1, c2>();

	for (int i = 0; i < r1; ++i) {
		for (int k = 0; k < c2; ++k) {
			for (int j = 0; j < c1; ++j) {
				result.val[i][k] += m1.val[i][j] * m2.val[j][k];
			}
		}
	}

	return result;

}

template<int r, int c>
Matrix<r, c> operator*(const Matrix<r, c>& m, const float& a) {
	Matrix<r, c> result = Matrix<r, c>();
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			result.val[i][j] = m.val[i][j] * a;
		}
	}
	return result;
}

//Addition
template<int r, int c>
Matrix<r, c> operator+(const Matrix<r, c>& m, const Matrix<r, c>& n) {
	Matrix<r, c> result = Matrix<r, c>();
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			result.val[i][j] = m.val[i][j] + n.val[i][j];
		}
	}
	return result;
}

template<int r, int c>
Matrix<r, c>& operator+=(Matrix<r, c>& m, const Matrix<r, c>& n) {

	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			m.val[i][j] += n.val[i][j];
		}
	}
	return m;
}


//Subtraction
template<int r, int c>
Matrix<r, c> operator-(const Matrix<r, c>& m, const Matrix<r, c>& n) {
	Matrix<r, c> result = Matrix<r, c>();
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			result.val[i][j] = m.val[i][j] - n.val[i][j];
		}
	}
	return result;
}

template<int r, int c>
Matrix<r, c>& operator-=(Matrix<r, c>& m, const Matrix<r, c>& n) {

	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			m.val[i][j] -= n.val[i][j];
		}
	}
	return m;
}