
#include <iostream>
#include <complex>
#include "matrix_library.hpp"

int main() {
	size_t a = 10;
	size_t b = 10;
	std::complex c(1, 1);
	//ExpMatrix<std::complex<int>> A(a,b,c);
	//ExpMatrix<std::complex<int>> B(a, b,c);
	//ExpMatrix<std::complex<int>> C(a, b,3);
	//ExpMatrix<std::complex<int>> D(a, b);
	ExpMatrix<int> A(a, b,4);
	ExpMatrix<int> B(a, b,1);
	ExpMatrix<int> C(a, b);
	for (size_t i = 0; i < a; i++)
	{
		for (size_t j = 0; j < b; j++)
		{
			C(i, j) += 5;

		}
	}
	ExpMatrix<int> D(a, b);
	//Matrix<double>N(3, 3, 2.5);
	//Matrix<int> A = M;
	//std::cout << A.get_cols();
	//std::vector<int>v= M[0];
	//double x = N(2,0);
	//D = A - B + C;
	//D = A*c;
	//B -= A;
	D = A * B;
	//std::complex y = D(0,0);
	
	return 0;
}


