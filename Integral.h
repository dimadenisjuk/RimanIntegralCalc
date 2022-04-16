#ifndef INTEGRAL_H
#define INTEGRAL_H

#include "Definitions.h"
#include<stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
//#include "ParserNew.h"

//#include<complex.h>

#ifndef _countof
#define _countof(x) sizeof(x)/sizeof(x[0])
#endif //_countof

#define MAX_INPUT_LENGTH 100

typedef struct {
	T _a;
	T _b;
	char * _func;
	T i;
	T E;
} Integ;

void DefIntegCreate(Integ* integ, T, T);
void IndefIntegCreate(Integ* integ);
void IntegDelete(Integ* integ);


T IntegResultOld(Integ* integ, int* progress, long long prestion); // аргумент -- для обратной связи (возвращает прогресс вычислений в процентах)
T IntegParseExprOld(Integ* integ, const char*);

T IntegResultNew(Integ* integ, int* progress, long long presition);

T fun(Integ* ob1, int* progress, int parserVersion, long long presition);

#endif //INTEGRAL_H
