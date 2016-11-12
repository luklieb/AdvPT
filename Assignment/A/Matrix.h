#pragma once
#include <iostream>
#include <cassert>
#include <algorithm>


class Matrix{
	
public:
	Matrix();
	Matrix(int sizeY, int sizeX);
	Matrix(int sizeY, int sizeX, double* data);
	Matrix(int sizeY, int sizeX, double init);
	Matrix(const Matrix & orig);
	~Matrix(){delete[] data_;//std::cout << "Destruktor" << std::endl;
	}

	int getX() const {return sizeX_;}
	int getY() const {return sizeY_;}
	int readFile();	
	
	Matrix operator+ (const Matrix & o);
	Matrix operator- (const Matrix & o);
	Matrix operator* (const Matrix & o);
	double & operator() (int y, int x);
	double operator() (int y, int x) const;
	Matrix & operator= (const Matrix & rhs);
	Matrix & operator+= (const Matrix & rhs);
	Matrix & operator-= (const Matrix & rhs);
	Matrix & operator*= (const Matrix & rhs);
	bool operator== (const Matrix & rhs) const;
	bool operator!= (const Matrix & rhs) const;

private:	
	int sizeX_;
	int sizeY_;
	double * data_;
	
};

std::ostream & operator<< (std::ostream & os, const Matrix & m);






//****************** IMPLEMENTATION ****************
Matrix::Matrix()
	: sizeX_(0),
	sizeY_(0),
	data_(nullptr)
	
{
	//std::cout << "Constructor stand" << std::endl;
}

Matrix::Matrix(int sizeY, int sizeX)
	: sizeX_(sizeX),
	sizeY_(sizeY),
	data_(new double[sizeX*sizeY]())
{
	assert( sizeX >= 0 );
	assert( sizeY >= 0 );
	//std::cout << "Constructor Y,X" << std::endl;
}

Matrix::Matrix(int sizeY, int sizeX, double* data)
	: sizeX_(sizeX), 
	sizeY_(sizeY), 
	data_(data)
{
	//std::cout << "Constructor Y,X,*" << std::endl;
}

Matrix::Matrix(int sizeY, int sizeX, double init)
	: sizeX_(sizeX), 
	sizeY_(sizeY)
{	
	assert( sizeX >= 0 );
	assert( sizeY >= 0 );
	data_ = new double[sizeX*sizeY];
	std::fill(data_, data_+(sizeX_*sizeY_), init);
	//std::cout << "Constructor Y,X,init" << std::endl;
}

Matrix::Matrix(const Matrix & orig)
	: sizeX_(orig.sizeX_),
	sizeY_(orig.sizeY_)
{
	data_ = new double[orig.sizeY_*orig.sizeX_];
	std::copy(orig.data_, orig.data_+(sizeX_*sizeY_), data_);
	//std::cout << "Constructor Matrix" << std::endl;
}

Matrix Matrix::operator+ (const Matrix & o){
	assert( sizeX_ == o.sizeX_);
	assert( sizeY_ == o.sizeY_);
	Matrix result (sizeY_, sizeX_);
	std::transform(data_, data_+(sizeX_*sizeY_), o.data_, result.data_, std::plus<double>() );
	return result;
}

Matrix Matrix::operator- (const Matrix & o){
	assert( sizeX_ == o.sizeX_);
	assert( sizeY_ == o.sizeY_);
	Matrix result (sizeY_, sizeX_);
	std::transform(data_, data_+(sizeX_*sizeY_), o.data_, result.data_, std::minus<double>() );
	return result;
}

Matrix Matrix::operator* (const Matrix & o){
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

double & Matrix::operator() (int y, int x){
	assert( x >= 0 && x < sizeX_);
	assert( y >= 0 && y < sizeY_);
	return data_[y*sizeX_ + x];
}

double  Matrix::operator() (int y, int x) const {
	assert( x >= 0 && x < sizeX_);
	assert( y >= 0 && y < sizeY_);
	return data_[y*sizeX_ + x];
}

Matrix & Matrix::operator= (const Matrix & rhs){
	//std::cout << "before rhs: " << std::endl << rhs << std::endl;
	Matrix tmp (rhs);
	//std::cout << "after tmp: " << std::endl << tmp << std::endl;
	std::swap(sizeX_, tmp.sizeX_);
	std::swap(sizeY_, tmp.sizeY_);
	std::swap(data_, tmp.data_);
	return *this;
}

Matrix & Matrix::operator+= (const Matrix & rhs){
	assert( rhs.sizeX_ == sizeX_);
	assert (rhs.sizeY_ == sizeY_);
	return (*this) = (*this) + rhs;
}

Matrix & Matrix::operator-= (const Matrix & rhs){
	assert( rhs.sizeX_ == sizeX_);
	assert( rhs.sizeY_ == sizeY_);
	return (*this) = (*this) - rhs;
}

Matrix & Matrix::operator*= (const Matrix & rhs){
	assert( sizeX_ == rhs.sizeY_);
	assert( sizeY_ == rhs.sizeX_);
	//Matrix result (o.sizeX_, sizeY_);
	return (*this) = (*this) * rhs;
}

bool Matrix::operator== (const Matrix & rhs) const{
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

bool Matrix::operator!= (const Matrix & rhs) const{
	return !((*this) == rhs);
}

//END MEMBER FCTS

//BEGIN NON-MEMBER FCTS

std::ostream & operator<< (std::ostream & os, const Matrix & m){
	for(int y = 0; y < m.getY(); ++y){
		for(int x = 0; x < m.getX(); ++x){
			os << m(y,x) << "\t";
		}
		os << std::endl;
	}
	return os;
}

