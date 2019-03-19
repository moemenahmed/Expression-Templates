#include <cassert>
#include <vector>
#include <functional>
// Scalar class :  class for objects that represent scalars
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
	T operator() (size_t,size_t) const
	{
		return s;
	}
	// scalars have zero as 0 rows ,0 cols 
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

// expression class templates

// expression template class for objects that represent the Operationition of two operands
template <typename T, typename OP1, typename OP2> 
class Operation
{
private:
	typename Traits<OP1>::ExprRef op1; // first operand 
	typename Traits<OP2>::ExprRef op2; // second operand
	std::function <T(T, T)>operation;
public:
	// constructor initializes references to operands
	Operation (OP1 const& a, OP2 const& b, const std::function<T(T, T)>& _operation) : op1(a), op2(b)
	{
		operation = _operation;
	}
	// compute sum when value requested 
	T operator() (const size_t r,const size_t c) const 
	{ 
		return operation(op1(r,c) , op2(r,c));
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

//Matrix class
template <typename T> 
class Matrix
{
private:
	std::vector<std::vector<T> > matrix;
	size_t rows;
	size_t cols;
public:
	// create matrix with initial size
	Matrix(const size_t r , const size_t c)
	{
		matrix.resize(r);
		for (size_t i = 0; i < matrix.size(); i++) {
			matrix[i].resize(c);
		}
		rows = r;
		cols = c;
	}
	// create matrix with initial size and initial value
	Matrix(const size_t r, const size_t c , const T initial_value)
	{
		matrix.resize(r);
		for (size_t i = 0; i < matrix.size(); i++) 
		{
			matrix[i].resize(c,initial_value);
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

};

//ExpMatrix class
template <typename T, typename obj = Matrix<T> > //obj type is either Matrix or Operation
class ExpMatrix
{
private:
	obj exp_obj; // access to the data of the matrix
public:
	// create matrix with initial size
	ExpMatrix(const size_t r, const size_t c):exp_obj(r,c)
	{
	}
	// create matrix with initial size and initial value
	ExpMatrix(const size_t r, const size_t c, const T initial_value):exp_obj(r,c, initial_value)
	{
	}
	// copy constructor
	ExpMatrix(const obj& M):exp_obj(M)
	{
	}
	// size of represented data 
	size_t get_rows() const
	{ 
		return exp_obj.get_rows();
	}
	size_t get_cols() const
	{
		return exp_obj.get_cols();
	}
	// assignment operator for same type 
	ExpMatrix& operator= (ExpMatrix const& M) 
	{ 
		assert(get_rows()== M.get_rows()  && get_cols() == M.get_cols());
		for (size_t r = 0; r < M.get_rows(); r++)
		{
			for (size_t c = 0; c < M.get_cols(); c++)
			{
				exp_obj(r, c) = M(r, c);
			}
			
		}
		return *this;
	}

	// assignment operator for different type 
	template<typename T2, typename obj2> 
	ExpMatrix& operator= (ExpMatrix<T2, obj2> const& M) 
	{ 
		assert(get_rows() == M.get_rows() && get_cols() == M.get_cols());
		for (size_t r = 0; r < M.get_rows(); r++)
		{
			for (size_t c = 0; c < M.get_cols(); c++)
			{
				exp_obj(r, c) = M(r, c);
			}

		}
		return *this;
	}

	// index element operator for constants 
	T operator() (const size_t r, const size_t c) const
	{ 
		assert(r<get_rows() || c<get_cols()); 
		return exp_obj(r,c);
	}
	// index element operator for variables
	T& operator() (const size_t r, const size_t c)
	{
		assert(r < get_rows() || c < get_cols());
		return exp_obj(r,c);
	}
	// return what the matrix currently represents 
	obj const& object() const 
	{ 
		return exp_obj;
	} 
	obj& object() 
	{ 
		return exp_obj; 
	}

	template <typename T>
	ExpMatrix<T>& operator += (const ExpMatrix<T> & a)
	{
		ExpMatrix<T> output (get_rows(), get_cols());
		output = (*this) + a;
		(*this) = output;
		return (*this);
	}
	template <typename T>
	ExpMatrix<T>& operator -= (const ExpMatrix<T> & a)
	{
		ExpMatrix<T> output(get_rows(), get_cols());
		output = (*this) - a;
		(*this) = output;
		return (*this);
	}

};

//operators
// addition of two matrix
template <typename T, typename R1, typename R2> 
ExpMatrix<T, Operation<T, R1, R2> > operator+ (ExpMatrix<T, R1> const& a, ExpMatrix<T, R2> const& b) 
{
	auto f = [](T x, T y) {return x + y; };
	return ExpMatrix<T, Operation<T, R1, R2> >(Operation<T, R1, R2>(a.object(), b.object(),f ));
}
// subtraction of two matrix
template <typename T, typename R1, typename R2>
ExpMatrix<T, Operation<T, R1, R2> > operator- (ExpMatrix<T, R1> const& a, ExpMatrix<T, R2> const& b)
{
	auto f = [](T x, T y) {return x - y; };
	return ExpMatrix<T, Operation<T, R1, R2> >(Operation<T, R1, R2>(a.object(), b.object(), f));
}
	


// scalar multiplication
template <typename T, typename R2>
ExpMatrix<T, Operation<T, Scalar<T>, R2> > operator* (const T& a, ExpMatrix<T, R2> const& b)
{
	auto f = [](T x, T y) {return x * y; };
	return ExpMatrix<T, Operation<T, Scalar<T>, R2> >(Operation<T, Scalar<T>, R2>(Scalar<T>(a), b.object(), f));
}
// scalar multiplication
template <typename T, typename R2>
ExpMatrix<T, Operation<T, Scalar<T>, R2> > operator* (ExpMatrix<T, R2> const& b, const T& a)
{
	auto f = [](T x, T y) {return x * y; };
	return ExpMatrix<T, Operation<T, Scalar<T>, R2> >(Operation<T, Scalar<T>, R2>(Scalar<T>(a), b.object(), f));
}
// matrix multiplication
template <typename T>
ExpMatrix<T> operator* (ExpMatrix<T> const& a, ExpMatrix<T> const& b)
{
	assert(a.get_cols()==b.get_rows());
	ExpMatrix<T> output(a.get_rows(), b.get_cols(),0);
	for (size_t i = 0; i < output.get_rows(); i++)
	{
		for (size_t j = 0; j < output.get_cols(); j++)
		{
			for (size_t k = 0; k < a.get_rows();k++)
			{
				output(i, j) += a(i, k)*b(k, j);
			}
			
		}
	}
	return output;
	
}