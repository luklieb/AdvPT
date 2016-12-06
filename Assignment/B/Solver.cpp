#include <iostream>
#include <cmath>
#include "Timer.h"
#include "Matrix.h"
#include "Stencil.h"

#define PI 3.141592653589793

template<typename T, size_t rows>
using Vector = Matrix<T, rows, 1>;

template<typename T, class Derived, size_t numPoints>
void solve (const MatrixLike<T, Derived, numPoints, numPoints>& A, const Vector<T, numPoints>& b, Vector<T, numPoints>& u) {
	const size_t numGridPoints = u.size( );

	double initRes = (b - A * u).l2Norm( ); // determine the initial residual
	double curRes = initRes;
	std::cout << "Initial residual:\t\t" << initRes << std::endl;

	unsigned int curIt = 0; // store the current iteration index

	while (curRes > 1.e-5 * initRes) { // solve until the residual is reduced by a certain amount
		++curIt;

		u += A.inverseDiagonal( ) * (b - A * u); // Jacobi step

		curRes = (b - A * u).l2Norm( ); // update the residual

		if (0 == curIt % 500) // print some info every few steps
			std::cout << "Residual after iteration " << curIt << ":\t" << curRes << std::endl;
	}

	std::cout << "Residual after iteration " << curIt << ":\t" << curRes  << std::endl; // print the final number of iterations and the final residual
}

template<size_t numPoints>
void testFullMatrix (const int numGridPoints) {
	const double hx = 1. / (numPoints - 1);
	const double hxSq = hx * hx;

	std::cout << "Starting full matrix solver for " << numPoints << " grid points" << std::endl;

	Matrix<double, numPoints, numPoints> A(numPoints, numPoints, 0.);
	Vector<double, numPoints> u(numPoints, 0.);
	Vector<double, numPoints> b(numPoints, 0.);

	A(0, 0) = 1.;
	for (int x = 1; x < numPoints - 1; ++x) {
		A(x, x - 1) = 1. / hxSq;
		A(x, x) = -2. / hxSq;
		A(x, x + 1) = 1. / hxSq;
	}
	A(numPoints - 1, numPoints - 1) = 1.;

	for (int x = 0; x < numPoints; ++x) {
		b(x) = sin(2. * PI * (x / (double)(numPoints - 1)));
	}

	std::cout << "Initialization Matrix complete\n";

	siwir::Timer time;
	time.reset();
	
	solve(A, b, u);
	
	std::cout << "Time elapsed Matrix: " << time.elapsed() << std::endl << std::endl;
}

template<size_t numPoints>
void testStencil (const int numGridPoints) {

    std::cout << "Starting full stencil solver for " << numGridPoints << " grid points" << std::endl;
    const double hx = 1. / (numGridPoints - 1);
    const double hxSq = hx * hx;
    
    Vector<double, numPoints> u(numGridPoints, 0.);
    Vector<double, numPoints> b(numGridPoints, [&numGridPoints](int x) ->double {return sin(2. * PI * (x / (double)(numGridPoints - 1)));});
    Stencil<double, numPoints, numPoints> Asten({ { 0, 1. } }, { { -1, 1. / hxSq },{ 0, -2. / hxSq },{ 1, 1. / hxSq } });

    std::cout << "Initialization Stencil complete\n";
    
    siwir::Timer time;
    time.reset();
    
    solve(Asten, b, u);
    
    std::cout << "Time elapsed Stencil: " << time.elapsed() << std::endl << std::endl;
    
}

int main(int argc, char** argv) {
    const int a = 17;
    
	testFullMatrix<a>(a);
    /*
	testFullMatrix<b>(b);
	testFullMatrix<c>(c);
	testFullMatrix<d>(d);

    testStencil<a>(a);
    testStencil<b>(b);
    testStencil<c>(c);
    testStencil<d>(d);
     */
    
}
