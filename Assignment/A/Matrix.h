#pragma once
#include <iostream>
#include <cassert>



class Matrix{
	
public:
	Matrix();
	Matrix(int sizeY, int sizeX);
	Matrix(int sizeY, int sizeX, double* data);
	Matrix(int sizeY, int sizeX, double init);
	Matrix(const Matrix & orig);
	~Matrix(){delete[] data_;std::cout << "Destruktor" << std::endl;}

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
	void allocate(int sizeY, int sizeX);
	
};

std::ostream & operator<< (std::ostream & os, const Matrix & m);
