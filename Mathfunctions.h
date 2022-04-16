#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#include "Definitions.h"
#include <math.h>

inline static T myLog(const T x) {
	return clog(x);
}

inline static T myLog10(const T x) {
	return clog(x)/clog(10);
}

inline static T myCot(const T x) {
	return (1+0*I) / ctan(x);
}

inline static T myAcot(const T x) {
	return (1+0*I) / catan(x);
}

inline static T mySin(const T x) {
	return csin(x);
}

inline static T myCos(const T x) {
	return ccos(x);
}

inline static T myTan(const T x) {
	return ctan(x);
}

inline static T myAcos(const T x) {
	return cacos(x);
}

inline static T myAsin(const T x) {
	return casin(x);
}

inline static T myAtan(const T x) {
	return catan(x);
}

inline static T myExp(const T x) {
	return cexp(x);
}

inline static T myConst(const T x) {
	return x;
}

inline static T myTanh(const T x) {
	return ctanh(x);
}

inline static T myAdd(const T num, const T x) {
	return num + x;
}

inline static T mySub(const T num, const T x) {
	return num - x;
}

inline static T myMul(const T num, const T x) {
	return num * x;
}

inline static T myDiv(const T num, const T x) {
	return num / x;
}

inline static T myPow(const T x, const T num) {
	if((int)(creal(num)) == creal(num))
		return pow(creal(x), creal(num)); // pow(double, double)
	return cpow(x, creal(num)); // pow(complex<double>, double)
}

#endif //MATHFUNCTIONS_H
