#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include<cstdlib>
#include <iostream>
#include<iomanip>
#include <cstring>
#include <fstream>
#include "Definitions.h"
#include "ParserNew.h"

//#include<complex.h>
using namespace std;

#ifndef _countof
#define _countof(x) sizeof(x)/sizeof(x[0])
#endif

#define MAX_INPUT_LENGTH 100

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
	void SetFunc(const char*);

	T Geta();
	T Getb();

	friend ostream& operator <<(ostream& stream, const Integ& ob1);
	friend istream& operator >>(istream& stream, Integ& ob2);

	T ResultOld(int* progress, long long prestion); // аргумент -- для обратной связи (возвращает прогресс вычислений в процентах)
	T ParseExprOld(const char*);

	T ResultNew(int* progress, long long presition);
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

T fun(Integ& ob1, int* progress, int parserVersion=1, long long presition=PRESITION_M);
