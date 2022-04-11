#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef _WIN32
#define MS_WINDOWS
#endif
#ifdef _WIN64
#define MS_WINDOWS
#endif

#ifdef MS_WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include <Windows.h>
#define GNUPLOT ".\\gnuplot\\bin\\gnuplot.exe graphP -"
#else
#define GNUPLOT "gnuplot graphP -"
#endif //MS_WINDOWS

// Presition:
#define PRESITION_LL	100000
#define PRESITION_L	1000000
#define PRESITION_M	10000000
#define PRESITION_H	100000000
#define PRESITION_HH	1000000000
#define PRESITION_FINE	10000000000
// Parser versions:
#define PARSER_OLD 0
#define PARSER_NEW 1

#include<ccomplex>
using namespace std;
typedef complex<double> T;
//typedef double T;

#endif //DEFINITIONS_H
