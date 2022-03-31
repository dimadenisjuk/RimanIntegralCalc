#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include<cstdlib>
#include <iostream>
#include<iomanip>
#include<ccomplex>
#include <cstring>
#include <fstream>
//#include<complex.h>
using namespace std;

#ifndef _countof
#define _countof(x) sizeof(x)/sizeof(x[0])
#endif

#define MAX_INPUT_LENGTH 100
typedef complex<double> T;
//typedef double T;

class Integ {
	T _a;
	T _b;
	char * _func;
	T i;
	T E;
public:
	Integ();
	Integ(T, T);
	~Integ();

	void Seta(T);
	void Setb(T);

	T Geta();
	T Getb();

	friend ostream& operator <<(ostream& stream, const Integ& ob1);
	friend istream& operator >>(istream& stream, Integ& ob2);

	T ResultOld();
	T ParseExprOld(const char*);

	T ResultNew();
};
/*
class MyException {
	T num1;
	T num2;
public:
	MyException();
	MyException(Integ ob1);
	void printMessage();
};*/

T fun(Integ& ob1);
