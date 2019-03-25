/* Matrix Library : a library that deals with matrices by expression templates , performs
operations like elementwise addition, subtraction, scalar multiplication and
matrix multipication operations using operators .
it also supports different numerical tybes of a matrix like (int, long, float, double, complex,..)

*/

#include <cassert>
#include <vector>
#include <functional>

/*
Scalar class :  class for objects that represent scalars of any type (int , double , complex ,...)
*/
template <typename T>
class Scalar
{
private:
	T const& s; // value of the scalar
public:
	// constructor initializes value 
	Scalar(T const& v) : s(v)
	{
	}
	// for index operations the scalar is the value of each element 
	T operator() (size_t, size_t) const
	{
		return s;
	}
	// assuming scalars have zero as 0 rows ,0 cols 
	size_t get_rows() const
	{
		return 0;
	}
	size_t get_cols() const
	{
		return 0;
	}

};


/* helper Traits class to select how to refer to an expression template node
  - in general : by reference
  - for scalars : by value
 */
template <typename T>
class Traits
{
public:
	typedef T const& ExprRef; // type to refer to is constant reference
};
// partial specialization for scalars 
template <typename T>
class Traits<Scalar<T> >
{
public:
	typedef Scalar<T> ExprRef; // type to refer to is ordinary value 
};



/*****************************expression class template***************************************/

/**************************************Expression class***************************************/
/*
Expression : is the base class (CRTP) , it can be a simple matrix or an operation that contains another
matrices or another operations.
*/
template <typename Type,typename T> // Type is either Matrix or Operation
class Expression
{
public:

	// size of represented data 
	size_t get_rows() const
	{
		return static_cast<Type const&>(*this).get_rows();
	}
	size_t get_cols() const
	{
		return static_cast<Type const&>(*this).get_cols();
	}


	// index element operator for constants 
	T operator() (const size_t r, const size_t c) const
	{
		return static_cast<Type const&>(*this)(r, c);
	}
	// index element operator for variables
	T& operator() (const size_t r, const size_t c)
	{
		return static_cast<Type const&>(*this)(r, c);
	}


};

/*
Operation : expression template class for objects that represent the operation
of two operands.
The operands can be matrix or scalar or Operation again (nested expressions).
The Operation class is used for both addition and multiplication using lambdas.
*/
template <typename T, typename OP1, typename OP2>
class Operation:public Expression<Operation<T,OP1,OP2>,T>
{
private:
	typename Traits<OP1>::ExprRef op1; // first operand 
	typename Traits<OP2>::ExprRef op2; // second operand
	std::function <T(T, T)>operation;
public:
	// constructor initializes references to operands
	Operation(OP1 const& a, OP2 const& b, const std::function<T(T, T)>& _operation) : op1(a), op2(b)
	{
		operation = _operation;
	}
	// compute sum when value requested 
	T operator() (const size_t r, const size_t c) const
	{
		return operation(op1(r, c), op2(r, c));
	}
	// size is maximum size 
	size_t get_rows() const
	{
		//size 0 is for Scalar
		assert(op1.get_rows() == 0 || op2.get_rows() == 0 || op1.get_rows() == op2.get_rows());
		return op1.get_rows() != 0 ? op1.get_rows() : op2.get_rows();
	}
	size_t get_cols() const
	{
		//size 0 is for Scalar
		assert(op1.get_cols() == 0 || op2.get_cols() == 0 || op1.get_cols() == op2.get_cols());
		return op1.get_cols() != 0 ? op1.get_cols() : op2.get_cols();
	}

};

/*******************************************Matrix class***************************************/
/*
Matrix : a simple matrix class without the use of expression templates.
It is used for the next main class .
*/
template <typename T>
class Matrix:public Expression<Matrix<T>,T>
{
private:
	std::vector<std::vector<T> > matrix;
	size_t rows;
	size_t cols;
public:
	// create matrix with initial size
	Matrix(const size_t r, const size_t c)
	{
		matrix.resize(r);
		for (size_t i = 0; i < matrix.size(); i++) {
			matrix[i].resize(c);
		}
		rows = r;
		cols = c;
	}
	// create matrix with initial size and initial value
	Matrix(const size_t r, const size_t c, const T initial_value)
	{
		matrix.resize(r);
		for (size_t i = 0; i < matrix.size(); i++)
		{
			matrix[i].resize(c, initial_value);
		}
		rows = r;
		cols = c;
	}

	// copy constructor
	Matrix(const Matrix<T>& M)
	{
		rows = M.rows;
		cols = M.cols;
		matrix = M.matrix;
	}
	// assignment operator
	Matrix<T>& operator= (Matrix<T> const& M) {
		if (&M == this)
		{
			return *this;
		}

		matrix.resize(M.rows);
		for (size_t i = 0; i < matrix.size(); i++) {
			matrix[i].resize(M.cols);
		}

		for (size_t i = 0; i < M.rows; i++) {
			for (size_t j = 0; j < M.cols; j++) {
				matrix[i][j] = M(i, j);
			}
		}
		rows = M.rows;
		cols = M.cols;

		return *this;
	}
	//assignment operator for Expression calculating
	template<typename Type>
	Matrix & operator=(Expression<Type,T> const& M)
	{
		size_t rows = M.get_rows();
		size_t cols = M.get_cols();
		assert(get_rows() == rows && get_cols() == cols);
		for (size_t r = 0; r < rows; r++)
		{
			for (size_t c = 0; c < cols; c++)
			{
				(*this)(r, c) = M(r, c);
			}

		}
		return *this;
	}
	//get number of rows
	size_t get_rows()const
	{
		return rows;
	}
	//get number of columns
	size_t get_cols()const
	{
		return cols;
	}

	//index vector operator for constants
	std::vector<T> operator[] (const size_t r) const
	{
		return matrix[r];
	}
	//index vector operator for variables
	std::vector<T>& operator[] (const size_t r)
	{
		return matrix[r];
	}
	//index element operator for constants
	T operator() (const size_t r, const size_t c) const
	{
		return matrix[r][c];
	}
	//index element operator for variables
	T& operator() (const size_t r, const size_t c)
	{
		return matrix[r][c];
	}
	// operator +=
	template <typename T>
	Matrix<T>& operator += (const Matrix<T> & a)
	{
		Matrix<T> output(get_rows(), get_cols());
		output = (*this) + a;
		(*this) = output;
		return (*this);
	}
	// operator -=
	template <typename T>
	Matrix<T>& operator -= (const Matrix<T> & a)
	{
		Matrix<T> output(get_rows(), get_cols());
		output = (*this) - a;
		(*this) = output;
		return (*this);
	}
	//print matrix to console
	void print_matrix()
	{
		std::cout << std::endl;
		for (size_t r = 0; r < (*this).get_rows(); r++)
		{
			if (r != 0)std::cout << std::endl;
			for (size_t c = 0; c < (*this).get_cols(); c++)
			{
				std::cout << (*this)(r, c);
				if (c != (*this).get_cols() - 1)std::cout << " , ";
			}

		}
		std::cout << std::endl;
	}
};



/******************************************operators********************************************/
// addition of two matrices
template <typename T, typename R1, typename R2>
auto  operator+ (Expression<R1,T> const& a, Expression<R2, T> const& b)
{
	auto f = [](T x, T y) {return x + y; };
	return Operation<T, Expression<R1, T>, Expression<R2, T>> (a, b, f);
}
// subtraction of two matrices
template <typename T, typename R1, typename R2>
auto  operator- (Expression<R1, T> const& a, Expression<R2, T> const& b)
{
	auto f = [](T x, T y) {return x - y; };
	return Operation<T, Expression<R1, T>, Expression<R2, T>>(a, b, f);
}

// scalar multiplication ( scalar * matrix)
template <typename T, typename R2>
auto operator* (const T& a, Expression<R2, T> const& b)
{
	auto f = [](T x, T y) {return x * y; };
	return Operation<T, Scalar<T>, Expression<R2, T>> (Scalar<T>(a), b, f);
}
// scalar multiplication ( matrix * scalar)
template <typename T, typename R2>
auto  operator* (Expression<R2, T> const& b, const T& a)
{
	auto f = [](T x, T y) {return x * y; };
	return Operation<T, Scalar<T>, Expression<R2, T>> (Scalar<T>(a), b, f);
}
// matrix multiplication
template <typename T>
Matrix<T> operator* (Matrix<T> const& a, Matrix<T> const& b)
{

	assert(a.get_cols() == b.get_rows());
	Matrix<T> output(a.get_rows(), b.get_cols(), 0);
	size_t rows = output.get_rows();
	size_t cols = output.get_cols();
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			for (size_t k = 0; k < rows; k++)
			{
				output(i, j) += a(i, k)*b(k, j);
			}

		}
	}
	return output;

}

