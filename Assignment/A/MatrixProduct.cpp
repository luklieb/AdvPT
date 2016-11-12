#include "Matrix.h"
#include <iostream>


void validInput(int & var);
void validInput(double & var);
//int validInputInt();

using std::cout;
using std::endl;
using std::cin;

int main (int argv, const char** args){

	int s1 = 0;
	int s2 = 0;
	int s3 = 0;

	cout << "Enter sizes s1, s2, s3: " << endl;
	//cin >> s1 >> s2 >> s3;
	validInput(s1);
	validInput(s2);
	validInput(s3);
	cout << "s1: " << s1 << ", s2: " << s2 << ", s3: " << s3 << endl;
	if( (s1 <= 0) || (s2 <= 0) || (s3<= 0) ){
		std::cerr << "Wrong sizes... abort programm" << endl;
		return EXIT_FAILURE;
	} 
	Matrix m1(s1,s2);
	Matrix m2(s2,s3);
	Matrix m3(s1,s3);

	cout << "m1 row by row, " << s1 << "x" << s2 << endl;
	for(int y = 0; y<s1; ++y){
		for(int x = 0; x<s2; ++x){
			validInput(m1(y,x));
		}
	}
	cout << "m1: "<< endl << m1 << endl;
	cout << "m2 row by row, " << s2 << "x" << s3 << endl;
	for(int y = 0; y<s2; ++y){
		for(int x = 0; x<s3; ++x){
			validInput(m2(y,x));
		}
	}

	cout << "m2: " << endl << m2 << endl;
	
	cout << "m1 y: " << m1.getY() << ", m1 x: " << m1.getX() << endl; 	
	cout << "m2 y: " << m2.getY() << ", m2 x: " << m2.getX() << endl;
	cout << "Computing m1*m2=m3" << endl;
	m3 = m1*m2;
	cout << m3 << endl;



	return EXIT_SUCCESS;
}



void validInput(double &  var){
	std::remove_reference<decltype(var)>::type n;
	std::cin >> n;
	while(std::cin.fail()){
		// not a number
		std::cin.clear(); // reset failbit
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip input too big
		cout << "Wrong input. Enter number again..." << endl;
		// next, request user reinput
		std::cin >> n;
	}
	var = n;
}
void validInput(int &  var){
	std::remove_reference<decltype(var)>::type n;
	std::cin >> n;
	while(std::cin.fail()){
		// not a number
		std::cin.clear(); // reset failbit
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip input too big
		cout << "Wrong input. Enter number again..." << endl;
		// next, request user reinput
		std::cin >> n;
	}
	var = n;
}

/*
int validInputInt(){
	int n;
	std::cin >> n;
	while(std::cin.fail()){
		// not a number
		std::cin.clear(); // reset failbit
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip input too big
		cout << "Wrong input. Enter number again..." << endl;
		// next, request user reinput
		std::cin >> n;
	}
	return n;
}
*/



