/*
source code for using and testing the matrix library .
*/

#include <iostream>
#include <complex>
#include <chrono>
#include "matrix_library.hpp"

void example1()
{
	// example 1 : add 4 matrices 1000x1000 of int type
	std::cout << "example 1: " << std::endl;
	size_t rows = 1000;
	size_t cols = 1000;
	ExpMatrix<int> A(rows, cols, 4);// matrix with all elements equal 4
	ExpMatrix<int> B(rows, cols, 1);// matrix with all elements equal 1
	ExpMatrix<int> C(rows, cols, 3);// matrix with all elements equal 3
	ExpMatrix<int> D(rows, cols, 5);// matrix with all elements equal 5
	ExpMatrix<int> sum(rows, cols);// matrix with all elements equal 0

	auto start = std::chrono::high_resolution_clock::now();
	sum = A + B + C + D;
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "it takes " << duration.count() << " ms to add 4 matrices of type int each 1000 x 1000" << std::endl;
	std::cout << "one sample entry of the output matrix : " << sum(0, 0) << std::endl;
	std::cout << std::endl;
}
void example2()
{
	// example 2 : add 3 matrices 500x500 of complex type
	std::cout << "example 2: " << std::endl;
	size_t rows = 500;
	size_t cols = 500;
	std::complex<int>c1(5, 6);
	std::complex<int>c2(1, 0);
	std::complex<int>c3(0, 5);
	ExpMatrix<std::complex<int>> A(rows, cols, c1);// matrix with all elements equal 5+i*6
	ExpMatrix<std::complex<int>> B(rows, cols, c2);// matrix with all elements equal 1
	ExpMatrix<std::complex<int>> C(rows, cols, c3);// matrix with all elements equal i*5
	ExpMatrix<std::complex<int>> sum(rows, cols);// matrix with all elements equal 0

	auto start = std::chrono::high_resolution_clock::now();
	sum = A + B + C ;
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "it takes " << duration.count() << " ms to add 3 matrices of type complex each 500 x 500" << std::endl;
	std::cout << "one sample entry of the output matrix : " << sum(0, 0) << std::endl;
	std::cout << std::endl;
}
void example3()
{
	// example 3 : operate on 4 matrices 500 of double type
	std::cout << "example 3: " << std::endl;
	size_t rows = 500;
	size_t cols = 500;
	ExpMatrix<double> A(rows, cols, 4.1);// matrix with all elements equal 4.1
	ExpMatrix<double> B(rows, cols, 1.2);// matrix with all elements equal 1.2
	ExpMatrix<double> C(rows, cols, 3.6);// matrix with all elements equal 3.6
	ExpMatrix<double> D(rows, cols, 5.3);// matrix with all elements equal 5.3
	ExpMatrix<double> sum(rows, cols);// matrix with all elements equal 0

	auto start = std::chrono::high_resolution_clock::now();
	sum = 5.1*A + B - C + 2.1*D;
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "it takes " << duration.count() << " ms to operate on 4 matrices of type double each 500 x 500 with different operations " << std::endl;
	std::cout << "one sample entry of the output matrix : " << sum(0, 0) << std::endl;
	std::cout << std::endl;
}
void example4()
{
	//example 4: initializing matrices with different values using indexing , test +=  operator, print matrix
	std::cout << "example 4: " << std::endl;
	size_t rows = 3;
	size_t cols = 3;
	ExpMatrix<short> A(rows, cols);
	ExpMatrix<short> B(rows, cols,40);// matrix with all elements equal 40
	//filling matrix A manually by index operator ()
	A(0, 0) = 1; A(0, 1) = 2; A(0, 2) = 3;
	A(1, 0) = 20; A(1, 1) = 5; A(1, 2) = 3;
	A(2, 0) = 3; A(2, 1) = 30; A(2, 2) = 1;
	//print A before executing A+=B
	std::cout << "A= " << std::endl;
	A.print_matrix();
	// print B matrix
	std::cout << "B= " << std::endl;
	B.print_matrix();

	//test += operator
	A += B;
	std::cout << "after executing A+=B , A equals" << std::endl;
	// print matrix A using print_matrix() function
	A.print_matrix();
	std::cout << std::endl;
}
void example5()
{
	//example 5: matrix multiplication followed by addition with another matrix
	std::cout << "example 5: " << std::endl;

	ExpMatrix<double> A(100, 500, 4.1);// matrix with all elements equal 4.1
	ExpMatrix<double> B(500, 100, 1.2);// matrix with all elements equal 1.2
	ExpMatrix<double> C(100, 100, 3.6);// matrix with all elements equal 3.6
	ExpMatrix<double> sum(100, 100);// matrix with all elements equal 0

	auto start = std::chrono::high_resolution_clock::now();
	sum = A * B + C;
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "it takes " << duration.count() << " ms to multiply 2 matrices of type double each one 100x500 and the other 500x100 followed by elementwise addition with another matrix " << std::endl;
	std::cout << "one sample entry of the output matrix : " << sum(0, 0) << std::endl;
	std::cout << std::endl;
}
int main() 
{
	example1();
	example2();
	example3();
	example4();
	example5();
	return 0;
}


