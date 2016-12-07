#pragma once
#include <iostream>
#include <cassert>
#include <algorithm>
#include <numeric>

template<typename T, size_t entries>
class Vector{
	
public:
	Vector();
	Vector(int size);
	Vector(int size, T* data);
	Vector(int size, T init);
	Vector(const Vector & orig);
    Vector(int size, std::function<T(int i)> init);
	~Vector(){delete[] data_;//std::cout << "Destruktor" << std::endl;
	}

	int size() const {return size_;}
	int readFile();
    
    T l2Norm() const;
	
	Vector operator+ (const Vector & o) const;
	Vector operator- (const Vector & o) const;
	T operator* (const Vector & o) const;
	T & operator() (int x);
	T operator() (int x) const;
	Vector & operator= (const Vector & rhs);
	Vector & operator+= (const Vector & rhs);
	Vector & operator-= (const Vector & rhs);
	T & operator*= (const Vector & rhs);
	bool operator== (const Vector & rhs) const;
	bool operator!= (const Vector & rhs) const;

private:	
	int size_;
	T * data_;
	
};

template<typename T, size_t entries>
std::ostream & operator<< (std::ostream & os, const Vector<T, entries> & m);






//****************** IMPLEMENTATION ****************
template<typename T, size_t entries>
Vector<T, entries>::Vector()
	: size_(0),
	data_(nullptr)
	
{
	//std::cout << "Constructor stand" << std::endl;
}

template<typename T, size_t entries>
Vector<T, entries>::Vector(int size)
	: size_(size),
	data_(new T[size]())
{
	assert( size > 0 );
	//std::cout << "Constructor Y,X" << std::endl;
}

template<typename T, size_t entries>
Vector<T, entries>::Vector(int size, T* data)
	: size_(size),
	data_(data)
{
	assert( size > 0);
	//std::cout << "Constructor Y,X,*" << std::endl;
}

template<typename T, size_t entries>
Vector<T, entries>::Vector(int size, T init)
	: size_(size)
{	
	assert( size > 0 );
	data_ = new T[size];
	std::fill(data_, data_+(size_), init);
	//std::cout << "Constructor Y,X,init" << std::endl;
}

template<typename T, size_t entries>
Vector<T, entries>::Vector(int size, std::function<T(int i)> init)
    : size_(size),
    data_(new T[size]())
{
    assert (size > 0);
    for(int i = 0; i < size; ++i){
        data_[i] = init(i);
    }
}

template<typename T, size_t entries>
Vector<T, entries>::Vector(const Vector & orig)
	: size_(orig.size_)
{
	data_ = new T[orig.size_];
	std::copy(orig.data_, orig.data_+size_, data_);
	//std::cout << "Constructor Vector" << std::endl;
}

template<typename T, size_t entries>
T Vector<T, entries>::l2Norm()const{
    T init = 0.;
    return sqrt(std::accumulate(data_, data_+size_, init, [](T x, T y){ return x+y*y;}  ));
}

template<typename T, size_t entries>
Vector<T, entries> Vector<T, entries>::operator+ (const Vector & o) const{
	assert( size_ == o.size_);
	Vector result (size_);
	std::transform(data_, data_+(size_), o.data_, result.data_, std::plus<T>() );
	return result;
}

template<typename T, size_t entries>
Vector<T, entries> Vector<T, entries>::operator- (const Vector & o) const{
	assert( size_ == o.size_);
	Vector result (size_);
	std::transform(data_, data_+size_, o.data_, result.data_, std::minus<T>() );
	return result;
}

template<typename T, size_t entries>
T Vector<T, entries>::operator* (const Vector & o) const{
	assert( size_ == o.size_);
    T init = 0.0;
    Vector result (size_);
    std::transform(data_, data_+size_, o.data_, result.data_, std::multiplies<T>() );
    return std::accumulate(result.data_, result.data_+result.size_, init);
}

template<typename T, size_t entries>
T & Vector<T, entries>::operator() (int x){
	assert( x >= 0);
    assert ( x < size_);
	return data_[x];
}

template<typename T, size_t entries>
T  Vector<T, entries>::operator() (int x) const {
	assert( x >= 0 && x < size_);
	return data_[x];
}

template<typename T, size_t entries>
Vector<T, entries> & Vector<T, entries>::operator= (const Vector & rhs){
	//std::cout << "before rhs: " << std::endl << rhs << std::endl;
	Vector tmp (rhs);
	//std::cout << "after tmp: " << std::endl << tmp << std::endl;
	std::swap(size_, tmp.size_);
	std::swap(data_, tmp.data_);
	return *this;
}

template<typename T, size_t entries>
Vector<T, entries> & Vector<T, entries>::operator+= (const Vector & rhs){
	assert( rhs.size_ == size_);
	return (*this) = (*this) + rhs;
}

template<typename T, size_t entries>
Vector<T, entries> & Vector<T, entries>::operator-= (const Vector & rhs){
	assert( rhs.size_ == size_);
	return (*this) = (*this) - rhs;
}

template<typename T, size_t entries>
T & Vector<T, entries>::operator*= (const Vector & rhs){
	assert( size_ == rhs.size_);
	return (*this) * rhs;
}

template<typename T, size_t entries>
bool Vector<T, entries>::operator== (const Vector & rhs) const{
	if(rhs.size_ != size_)
		return false;
    
	for(int x = 0; x < size_; ++x){
        if(rhs[x] != (*this)[x])
            return false;
	}
	return true;
}

template<typename T, size_t entries>
bool Vector<T, entries>::operator!= (const Vector & rhs) const{
	return !((*this) == rhs);
}

//END MEMBER FCTS

//BEGIN NON-MEMBER FCTS

template<typename T, size_t entries>
std::ostream & operator<< (std::ostream & os, const Vector<T, entries> & m){
	for(int y = 0; y < m.getSize(); ++y){
			os << m(y) << "\t";
	}
	return os;
}

