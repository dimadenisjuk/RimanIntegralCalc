#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef _WIN32
#define MS_WINDOWS
#endif
#ifdef _WIN64
#define MS_WINDOWS
#endif

#ifdef MS_WINDOWS
#include <Windows.h>
#define GNUPLOT ".\\gnuplot\\bin\\gnuplot.exe graphP -"
#else
#define GNUPLOT "gnuplot graphP -"
#endif //windows

#endif //DEFINITIONS_H