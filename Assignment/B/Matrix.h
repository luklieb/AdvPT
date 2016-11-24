#pragma once
#include <iostream>
#include <cassert>
#include <algorithm>
#include "MatrixLike.h"

template<typename T>
class Vector;


template<typename T>
class Matrix : public MatrixLike<T,Matrix<T>>{
	
public:
	Matrix();
	Matrix(int sizeY, int sizeX);
	Matrix(int sizeY, int sizeX, T* data);
	Matrix(int sizeY, int sizeX, T init);
	Matrix(const Matrix & orig);
	~Matrix(){delete[] data_;//std::cout << "Destruktor" << std::endl;
	}

	int getX() const {return sizeX_;}
	int getY() const {return sizeY_;}
	int readFile();	
	
    Matrix inverseDiagonal() const;
    
	Matrix operator+ (const Matrix & o) const;
	Matrix operator- (const Matrix & o) const;
	Matrix operator* (const Matrix & o) const;
	Vector<T> operator* (const Vector<T> & o) const;
	T & operator() (int y, int x);
	const T & operator() (int y, int x) const;
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
template<typename T>
std::ostream & operator<< (std::ostream & os, const Matrix<T> & m);






//****************** IMPLEMENTATION ****************
template<typename T>
Matrix<T>::Matrix()
	: sizeX_(0),
	sizeY_(0),
	data_(nullptr)
	
{
	//std::cout << "Constructor stand" << std::endl;
}

template<typename T>
Matrix<T>::Matrix(int sizeY, int sizeX)
	: sizeX_(sizeX),
	sizeY_(sizeY),
	data_(new T[sizeX*sizeY]())
{
	assert( sizeX > 0 );
	assert( sizeY > 0 );
	//std::cout << "Constructor Y,X" << std::endl;
}

template<typename T>
Matrix<T>::Matrix(int sizeY, int sizeX, T* data)
	: sizeX_(sizeX), 
	sizeY_(sizeY), 
	data_(data)
{
	assert( sizeX > 0);
	assert (sizeY > 0);
	//std::cout << "Constructor Y,X,*" << std::endl;
}

template<typename T>
Matrix<T>::Matrix(int sizeY, int sizeX, T init)
	: sizeX_(sizeX), 
	sizeY_(sizeY)
{	
	assert( sizeX > 0 );
	assert( sizeY > 0 );
	data_ = new T[sizeX*sizeY];
	std::fill(data_, data_+(sizeX_*sizeY_), init);
	//std::cout << "Constructor Y,X,init" << std::endl;
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> & orig)
	: sizeX_(orig.sizeX_),
	sizeY_(orig.sizeY_)
{
	data_ = new T[orig.sizeY_*orig.sizeX_];
	std::copy(orig.data_, orig.data_+(sizeX_*sizeY_), data_);
	//std::cout << "Constructor Matrix" << std::endl;
}

template<typename T>
Matrix<T> Matrix<T>::inverseDiagonal()const{
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


template<typename T>
Matrix<T> Matrix<T>::operator+ (const Matrix<T> & o) const{
	assert( sizeX_ == o.sizeX_);
	assert( sizeY_ == o.sizeY_);
	Matrix result (sizeY_, sizeX_);
	std::transform(data_, data_+(sizeX_*sizeY_), o.data_, result.data_, std::plus<T>() );
	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator- (const Matrix<T> & o) const{
	assert( sizeX_ == o.sizeX_);
	assert( sizeY_ == o.sizeY_);
	Matrix result (sizeY_, sizeX_);
	std::transform(data_, data_+(sizeX_*sizeY_), o.data_, result.data_, std::minus<T>() );
	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator* (const Matrix<T> & o) const{
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

template<typename T>
Vector<T> Matrix<T>::operator* (const Vector<T> & o) const{
	assert( sizeX_ == o.size() );
	Vector<T> result (sizeY_, (T)0.0);
	for(int y = 0; y < sizeY_ ; ++y){
		for(int x = 0; x < sizeX_; ++x){
			result(y) += (*this)(y,x) * o(x);
		}
	}
	
	return result;
}


template<typename T>
T & Matrix<T>::operator() (int y, int x){
	assert( x >= 0 && x < sizeX_);
	assert( y >= 0 && y < sizeY_);
	return data_[y*sizeX_ + x];
}

template<typename T>
const T &  Matrix<T>::operator() (int y, int x) const {
	assert( x >= 0 && x < sizeX_);
	assert( y >= 0 && y < sizeY_);
	return data_[y*sizeX_ + x];
}

template<typename T>
Matrix<T> & Matrix<T>::operator= (const Matrix<T> & rhs){
	//std::cout << "before rhs: " << std::endl << rhs << std::endl;
	Matrix tmp (rhs);
	//std::cout << "after tmp: " << std::endl << tmp << std::endl;
	std::swap(sizeX_, tmp.sizeX_);
	std::swap(sizeY_, tmp.sizeY_);
	std::swap(data_, tmp.data_);
	return *this;
}

template<typename T>
Matrix<T> & Matrix<T>::operator+= (const Matrix<T> & rhs){
	assert( rhs.sizeX_ == sizeX_);
	assert (rhs.sizeY_ == sizeY_);
	return (*this) = (*this) + rhs;
}

template<typename T>
Matrix<T> & Matrix<T>::operator-= (const Matrix<T> & rhs){
	assert( rhs.sizeX_ == sizeX_);
	assert( rhs.sizeY_ == sizeY_);
	return (*this) = (*this) - rhs;
}

template<typename T>
Matrix<T> & Matrix<T>::operator*= (const Matrix<T> & rhs){
	assert( sizeX_ == rhs.sizeY_);
	//Matrix result (o.sizeX_, sizeY_);
	return (*this) = (*this) * rhs;
}

template<typename T>
bool Matrix<T>::operator== (const Matrix<T> & rhs) const{
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

template<typename T>
bool Matrix<T>::operator!= (const Matrix<T> & rhs) const{
	return !((*this) == rhs);
}

//END MEMBER FCTS

//BEGIN NON-MEMBER FCTS

template<typename T>
std::ostream & operator<< (std::ostream & os, const Matrix<T> & m){
	for(int y = 0; y < m.getY(); ++y){
		for(int x = 0; x < m.getX(); ++x){
			os << m(y,x) << "\t";
		}
		os << std::endl;
	}
	return os;
}

