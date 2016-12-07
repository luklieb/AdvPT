#pragma once

#include <iostream>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <functional>
#include <array>
#include "MatrixLike.h"



template<typename T, size_t rows, size_t cols>
class Matrix : public MatrixLike<T, Matrix<T, rows, cols>, rows, cols> {
	
public:
	Matrix();
	Matrix(T init);
	Matrix(const Matrix & orig);
	Matrix(std::function<T(int i)> init); //nur fuer Vector
    //~Matrix();//std::cout << "Destruktor" << std::endl;
	

	int getX() const {return sizeX_;}
	int getY() const {return sizeY_;}
	int size() const {return sizeY_;} //fuer Vectoren
	int readFile();	
	
	Matrix inverseDiagonal() const override;
	T l2Norm() const;
    
	Matrix operator+ (const Matrix & o) const;
	Matrix operator- (const Matrix & o) const;
    
	//template<size_t N>
	Matrix<T,rows,1> operator* (const Matrix<T, cols, 1> & o) const override;
    
    
	T & operator() (int y, int  x);
	const T & operator() (int y, int  x) const;
	T & operator() (int y);
	const T operator() (int y) const;

	Matrix & operator= (const Matrix & rhs);
	Matrix & operator+= (const Matrix & rhs);
	Matrix & operator-= (const Matrix & rhs);
    template<size_t N>
	Matrix<T, rows, N> & operator*= (const Matrix<T, cols, N> & rhs);
	bool operator== (const Matrix & rhs) const;
	bool operator!= (const Matrix & rhs) const;

private:	
	int sizeX_;
	int sizeY_;
    std::array<T, rows*cols> data_;
	
};
template<typename T, size_t rows, size_t cols>
std::ostream & operator<< (std::ostream & os, const Matrix<T, rows, cols> & m);







//****************** IMPLEMENTATION ****************
template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix()
	: sizeX_(cols),
	sizeY_(rows),
    data_(std::array<T, rows*cols>())
	
{
    static_assert(rows > 0 && cols > 0, "Groesse = 0\n");
    //std::cout << "Constructor stand" << std::endl;
}


template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(T init)
	: sizeX_(cols),
	sizeY_(rows),
    data_(std::array<T, rows*cols>())
{	
    static_assert(rows > 0 && cols > 0, "Groesse = 0\n");
    std::fill(data_.begin(), data_.end(), init);
	//std::cout << "Constructor Y,X,init" << std::endl;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(const Matrix<T, rows, cols> & orig)
	: sizeX_(orig.sizeX_),
	sizeY_(orig.sizeY_),
    data_(std::array<T, rows*cols>())
{
    static_assert(rows > 0 && cols > 0, "Groesse = 0\n");
	std::copy(orig.data_.begin(), orig.data_.end(), data_.begin());
	//std::cout << "Constructor Matrix" << std::endl;
}


template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols>::Matrix(std::function<T(int i)> init)
    : sizeX_(1),
    sizeY_(rows),
    data_(std::array<T, rows*cols>())
{
    static_assert(rows > 0 && cols > 0, "Groesse = 0\n");
    static_assert(cols == 1, "Kein Vector\n");
    for(int x = 0; x < sizeY_; ++x){
        data_[x] = init(x);
    }
}



template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::inverseDiagonal()const{
    assert (rows == cols);
    Matrix <T, rows, cols> result;
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
    //std::cout << "Add" << std::endl;

	Matrix <T, rows, cols> result;
	std::transform(data_.begin(), data_.end(), o.data_.begin(), result.data_.begin(), std::plus<T>() );
	return result;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator- (const Matrix<T, rows, cols> & o) const{
    //std::cout << "Minus" << std::endl;
	Matrix <T, rows, cols> result;
	std::transform(data_.begin(), data_.end(), o.data_.begin(), result.data_.begin(), std::minus<T>() );
	return result;
}


template<typename T, size_t rows, size_t cols>
//template<size_t N>
Matrix<T, rows, 1>  Matrix<T, rows, cols>::operator* (const Matrix<T, cols, 1> & o) const{

	Matrix<T, rows, 1> result (0.);
	
	for(int y = 0; y < sizeY_; ++y){
		for(int x = 0; x < o.getX(); ++x){
			for(int inner = 0; inner < sizeX_; ++inner){
				result(y,x) += (*this)(y, inner) * o(inner, x);
			}
		} 

	}
	return result;
}



template<typename T, size_t rows, size_t cols>
T Matrix<T, rows, cols>::l2Norm() const{
    //std::cout << "L2 Norm" << std::endl;
    static_assert(cols == 1, "l2Norm kein Vector \n");
    T init = 0.;
    return sqrt(std::accumulate(data_.begin(), data_.end(), init, [](T x, T y){ return x+y*y;}  ));
    
}



template<typename T, size_t rows, size_t cols>
T & Matrix<T, rows, cols>::operator() (int y){
    static_assert(cols == 1, "(int) kein Vector\n");
    assert( y >= 0 && y < rows);
    return data_[y];
}


template<typename T, size_t rows, size_t cols>
const T Matrix<T, rows, cols>::operator() (int y) const{
    static_assert(cols == 1, "(int) kein Vector\n");
    assert(y >= 0 && y < rows) ;
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
	Matrix <T, rows, cols> tmp (rhs);
	//std::cout << "after tmp: " << std::endl << tmp << std::endl;
	std::swap(sizeX_, tmp.sizeX_);
	std::swap(sizeY_, tmp.sizeY_);
	std::swap(data_, tmp.data_);
	return *this;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> & Matrix<T, rows, cols>::operator+= (const Matrix<T, rows, cols> & rhs){

	return (*this) = (*this) + rhs;
}

template<typename T, size_t rows, size_t cols>
Matrix<T, rows, cols> & Matrix<T, rows, cols>::operator-= (const Matrix<T, rows, cols> & rhs){
	return (*this) = (*this) - rhs;
}

template<typename T, size_t rows, size_t cols>
template<size_t N>
Matrix<T, rows, N> & Matrix<T, rows, cols>::operator*= (const Matrix<T, cols, N> & rhs){
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

