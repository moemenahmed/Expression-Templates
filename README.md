# Matrix Library implemented with expression templates
- **The Library deals with large size matrices of all numerical types (int,short,double,complex,...)**

- **using expression templates increases performance of operating on multiple matrices at the same time , and also makes the program
  consumes less memory**
- **The library uses lambdas to avoid writing same code with little differences only**
## suppoted operators
- The + for elementwise matrix addition
- The - for elementwise matrix subtraction
- The += and -= operators
- The * for scalar multiplication and matrix multiplication
## Example on how to use
### declare a new matrix of int type
```
// 1000x1000 matrix with all elements equal 4
Matrix<int> A(1000, 1000, 4);
```
### make operations ,then print the matrix
```
Matrix<int> B(1000, 1000, 2);
A+=B;
A.print_matrix();
```
