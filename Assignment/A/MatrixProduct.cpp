#include "Matrix.h"



void Matrix::allocate(int sizeY, int sizeX){
	data_ = new double [sizeX * sizeY];
}

Matrix::Matrix(int sizeY, int sizeX)
	: sizeX_(sizeX),
	sizeY_(sizeY)
{
	assert( sizeX >= 0 );
	assert( sizeY >= 0 );
	allocate(sizeY, sizeX);
}

Matrix::Matrix(int sizeY, int sizeX, double* data)
	: sizeX_(sizeX), 
	sizeY_(sizeY), 
	data_(data)
	{}

Matrix::Matrix(int sizeY, int sizeX, double init)
	: sizeX_(sizeX), 
	sizeY_(sizeY)
{	
	assert( sizeX >= 0 );
	assert( sizeY >= 0 );
	allocate(sizeY, sizeX);
	std::fill(data_, data_+(sizeX_*sizeY_), init);
}

Matrix::Matrix(const Matrix & orig)
	: sizeX_(orig.sizeX_),
	sizeY_(orig.sizeY_)
{
	allocate(sizeY_, sizeX_);
	std::copy(orig.data_, orig.data_+(sizeX_*sizeY_), data_);
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
	assert( sizeY_ == o.sizeX_);
	Matrix result (o.sizeX_, sizeY_);
	for(int x = 0; x < result.getX(); ++x){
		for(int y = 0; y < result.getY(); ++y){
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
	Matrix tmp (rhs);
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

