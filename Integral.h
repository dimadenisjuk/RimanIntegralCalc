#pragma once
#include "Definitions.h"
#include<stdlib.h>
#include <string.h>
#include "ParserNew.h"

//#include<complex.h>

#ifndef _countof
#define _countof(x) sizeof(x)/sizeof(x[0])
#endif

#define MAX_INPUT_LENGTH 100

typedef struct {
	T _a;
	T _b;
	char * _func;
	T i;
	T E;
} Integ;

IntegCreate(Integ* integ);
IntegCreate(Integ* integ, T, T);
IntegDelete(Integ* integ);


T IntegResultOld(Integ* integ, int* progress, long long prestion); // аргумент -- для обратной связи (возвращает прогресс вычислений в процентах)
T IntegParseExprOld(Integ* integ, const char*);

T IntegResultNew(Integ* integ, int* progress, long long presition);

T fun(Integ* ob1, int* progress, int parserVersion=1, long long presition=PRESITION_M);
