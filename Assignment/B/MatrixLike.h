#pragma once

// forward declarations
template<typename T, size_t rows, size_t cols>
class Matrix;


template<typename T, class Derived, size_t rows, size_t cols>
class MatrixLike {
public:
	/// c'tor/ d'tor
	virtual ~MatrixLike ( ) noexcept = 0; // pure virtual destructor

	/// virtual operators
	virtual Matrix<T, rows, 1> operator* (const Matrix<T, cols, 1> & o) const = 0;
	// feel free to extend as required

	const T & operator() (int r, int c) const {return (static_cast<Derived*>(this))(r,c);}
	T & operator() (int r, int c) {return (static_cast<Derived*>(this))(r,c);}

	/// other functions
	virtual Derived inverseDiagonal( ) const = 0;
	// feel free to extend as required

protected:
};

// MatrixLike d'tor implementation
template<typename T, class Derived, size_t rows, size_t cols>
inline MatrixLike<T, Derived, rows, cols>::~MatrixLike ( ) noexcept { }
