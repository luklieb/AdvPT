#pragma once
#include <iostream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <functional>
#include "MatrixLike.h"



template<typename T, size_t rows, size_t cols>
class Matrix : public MatrixLike<T, Matrix<T, rows, cols>, rows, cols> {
	
public:
	Matrix();
	Matrix(int sizeY, int sizeX);
	Matrix(int sizeY, int sizeX, T* data);
	Matrix(int sizeY, int sizeX, T init);
	Matrix(const Matrix & orig);
	Matrix(int size, std::function<T(int i)> init); //Vector
	Matrix(int size); //Vector
	~Matrix(){delete[] data_;//std::cout << "Destruktor" << std::endl;
	}

	int getX() const {return sizeX_;}
	int getY() const {return sizeY_;}
	int size() const {return sizeY_;} //fuer Vectoren
	int readFile();	
	
	Matrix inverseDiagonal() const override;
	T l2Norm() const;
    
	Matrix operator+ (const Matrix & o) const;
	Matrix operator- (const Matrix & o) const;
    
	//template<size_t N>
	Matrix<T,rows,1> operator* (const Matrix<T, cols, 1> & o) const;
    
    
	T & operator() (int y, int  x);
	const T & operator() (int y, int  x) const;
	T & operator() (int y);
	const T operator() (int y) const;

	Matrix & operator= (const Matrix & rhs);
	Matrix & operator+= (const Matrix & rhs);
	Matrix & operator-= (const Matrix & rhs);
	Matrix & operator*= (const Matrix & rhs);
	bool operator== (const Matrix & rhs) const;
	bool operator!= (const Matrix & rhs) const;

private:	
	int sizeX_;
	int sizeY_;
	T * data_;
	
};
template<typename T, size_t rows, size_t cols>
std::ostream & operator<< (std::ostream & os, const Matrix<T, rows, cols> & m);







//****************** IMPLEMENTATION ****************
template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix()
	: sizeX_(0),
	sizeY_(0),
	data_(nullptr)
	
{
	//std::cout << "Constructor stand" << std::endl;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(int sizeY, int sizeX)
	: sizeX_(sizeX),
	sizeY_(sizeY),
	data_(new T[sizeX*sizeY]())
{
	assert( sizeX > 0 );
	assert( sizeY > 0 );
	//std::cout << "Constructor Y,X" << std::endl;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(int sizeY, int sizeX, T* data)
	: sizeX_(sizeX), 
	sizeY_(sizeY), 
	data_(data)
{
	assert( sizeX > 0);
	assert (sizeY > 0);
	//std::cout << "Constructor Y,X,*" << std::endl;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(int sizeY, int sizeX, T init)
	: sizeX_(sizeX), 
	sizeY_(sizeY)
{	
	assert( sizeX > 0 );
	assert( sizeY > 0 );
	data_ = new T[sizeX*sizeY];
	std::fill(data_, data_+(sizeX_*sizeY_), init);
	//std::cout << "Constructor Y,X,init" << std::endl;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(const Matrix<T, rows, cols> & orig)
	: sizeX_(orig.sizeX_),
	sizeY_(orig.sizeY_)
{
	data_ = new T[orig.sizeY_*orig.sizeX_];
	std::copy(orig.data_, orig.data_+(sizeX_*sizeY_), data_);
	//std::cout << "Constructor Matrix" << std::endl;
}


template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(int size, std::function<T(int i)> init)
: sizeX_(1),
sizeY_(size)
{
    data_ = new T[sizeY_]();
    for(int x = 0; x < sizeY_; ++x){
        data_[x] = init(x);
    }
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(int size)
: sizeX_(1),
sizeY_(size)
{
    data_ = new T[sizeY_]();

}


template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::inverseDiagonal()const{
    assert (sizeY_ == sizeX_);
    Matrix result(sizeY_, sizeX_);
    T temp;
    for(int i = 0; i<sizeY_; ++i){
        temp = (*this)(i,i);
        assert ( temp != 0 );
        result(i,i) = ((T)1.0)/temp;
    }
    return result;
}


template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator+ (const Matrix<T, rows, cols> & o) const{
	assert( sizeX_ == o.sizeX_);
	assert( sizeY_ == o.sizeY_);
	Matrix result (sizeY_, sizeX_);
	std::transform(data_, data_+(sizeX_*sizeY_), o.data_, result.data_, std::plus<T>() );
	return result;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator- (const Matrix<T, rows, cols> & o) const{
	assert( sizeX_ == o.sizeX_);
	assert( sizeY_ == o.sizeY_);
	Matrix result (sizeY_, sizeX_);
	std::transform(data_, data_+(sizeX_*sizeY_), o.data_, result.data_, std::minus<T>() );
	return result;
}


template<typename T, size_t rows, size_t cols>
//template<size_t N>
Matrix<T, rows, 1>  Matrix<T, rows, cols>::operator* (const Matrix<T, cols, 1> & o) const{
	//assert( sizeX_ == o.sizeY_);
	//static_assert(rows == cols, "mult\n");
    
	Matrix<T, rows, 1> result (sizeY_, o.getX());
	
	for(int y = 0; y < sizeY_; ++y){
		for(int x = 0; x < o.getX(); ++x){
			for(int inner = 0; inner < sizeX_; ++inner){
				result(y,x) += (*this)(y, inner) * o(inner, x);
			}
		} 

	}
	return result;
}

/*
template<typename T, size_t rows, size_t cols>
Matrix<T, rows, 1>  Matrix<T, rows, cols>::operator* (const Matrix<T, rows, 1> & o) const{
    assert( sizeX_ == o.sizeY_);
    Matrix result (sizeY_, o.sizeX_);
    for(int y = 0; y < sizeY_; ++y){
        for(int x = 0; x < o.sizeX_; ++x){
            for(int inner = 0; inner < sizeX_; ++inner){
                result(y,x) += (*this)(y, inner) * o(inner, x);
            }
        } 
        
    }
    return result;
}

*/



template<typename T, size_t rows, size_t cols>
T Matrix<T, rows, cols>::l2Norm() const{
    static_assert( cols == 1, "l2Norm kein Vektor\n" );
    assert(rows == sizeY_);
    T init = 0.;
    return sqrt(std::accumulate(data_, data_+sizeY_, init, [](T x, T y){ return x+y*y;}  ));
    
}



template<typename T, size_t rows, size_t cols>
T & Matrix<T, rows, cols>::operator() (int y){
    static_assert( cols == 1, "Vector () Operator 1\n");
    assert( rows < y);
    return data_[y];
}


template<typename T, size_t rows, size_t cols>
const T Matrix<T, rows, cols>::operator() (int y) const{
    static_assert( cols == 1, "Vector () Operator 2\n");
    assert( rows < y);
    return data_[y];
}



template<typename T, size_t rows, size_t cols>
T & Matrix<T, rows, cols>::operator() (int y, int x){
	assert( x >= 0 && x < sizeX_);
	assert( y >= 0 && y < sizeY_);
	return data_[y*sizeX_ + x];
}

template<typename T, size_t rows, size_t cols>
const T &  Matrix<T, rows, cols>::operator() (int y, int x) const {
	assert( x >= 0 && x < sizeX_);
	assert( y >= 0 && y < sizeY_);
	return data_[y*sizeX_ + x];
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> & Matrix<T, rows, cols>::operator= (const Matrix<T, rows, cols> & rhs){
	//std::cout << "before rhs: " << std::endl << rhs << std::endl;
	Matrix tmp (rhs);
	//std::cout << "after tmp: " << std::endl << tmp << std::endl;
	std::swap(sizeX_, tmp.sizeX_);
	std::swap(sizeY_, tmp.sizeY_);
	std::swap(data_, tmp.data_);
	return *this;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> & Matrix<T, rows, cols>::operator+= (const Matrix<T, rows, cols> & rhs){
	assert( rhs.sizeX_ == sizeX_);
	assert (rhs.sizeY_ == sizeY_);
	return (*this) = (*this) + rhs;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> & Matrix<T, rows, cols>::operator-= (const Matrix<T, rows, cols> & rhs){
	assert( rhs.sizeX_ == sizeX_);
	assert( rhs.sizeY_ == sizeY_);
	return (*this) = (*this) - rhs;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> & Matrix<T, rows, cols>::operator*= (const Matrix<T, rows, cols> & rhs){
	assert( sizeX_ == rhs.sizeY_);
	//Matrix result (o.sizeX_, sizeY_);
	return (*this) = (*this) * rhs;
}

template<typename T, size_t rows, size_t cols>
bool Matrix<T, rows, cols>::operator== (const Matrix<T, rows, cols> & rhs) const{
	if(rhs.sizeX_ != sizeX_)
		return false;
	if(rhs.sizeY_ != sizeY_)
		return false;
	
	for(int y = 0; y < sizeY_; ++y){
		for(int x = 0; x < sizeX_; ++x){
			if(rhs(x,y) != (*this)(x,y))
				return false;
		}
	}
	return true;
}

template<typename T, size_t rows, size_t cols>
bool Matrix<T, rows, cols>::operator!= (const Matrix<T, rows, cols> & rhs) const{
	return !((*this) == rhs);
}

//END MEMBER FCTS

//BEGIN NON-MEMBER FCTS

template<typename T, size_t rows, size_t cols>
std::ostream & operator<< (std::ostream & os, const Matrix<T, rows, cols> & m){
	for(int y = 0; y < m.getY(); ++y){
		for(int x = 0; x < m.getX(); ++x){
			os << m(y,x) << "\t";
		}
		os << std::endl;
	}
	return os;
}

