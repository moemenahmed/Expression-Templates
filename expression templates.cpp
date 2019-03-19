
#include <iostream>
#include <complex>
#include "matrix_library.hpp"

int main() {
	size_t a = 5;
	size_t b = 5;
	std::complex c(1, 1);
	ExpMatrix<std::complex<int>> A(a,b,c);
	ExpMatrix<std::complex<int>> B(a, b,c);
	ExpMatrix<std::complex<int>> C(a, b,3);
	ExpMatrix<std::complex<int>> D(a, b);
	//Matrix<double>N(3, 3, 2.5);
	//Matrix<int> A = M;
	//std::cout << A.get_cols();
	//std::vector<int>v= M[0];
	//double x = N(2,0);
	//D = A - B + C;
	//D = A*c;
	B -= A;
	std::complex y = D(0,0);
	
	return 0;
}


