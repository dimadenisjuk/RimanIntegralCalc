#include "math.h"

const static T tmpEd = 1;

inline T myLog(const T x) {
	return log(x);
}

inline T myLog10(const T x) {
	return log10(x);
}

inline T myCot(const T x) {
	return tmpEd / tan(x);
}

inline T myAcot(const T x) {
	return tmpEd / atan(x);
}

inline T mySin(const T x) {
	return sin(x);
}

inline T myCos(const T x) {
	return cos(x);
}

inline T myTan(const T x) {
	return tan(x);
}

inline T myAcos(const T x) {
	return acos(x);
}

inline T myAsin(const T x) {
	return asin(x);
}

inline T myAtan(const T x) {
	return atan(x);
}

inline T myExp(const T x) {
	return exp(x);
}

inline T myConst(const T x) {
	return x;
}

inline T myTanh(const T x) {
	return tanh(x);
}

inline T myAdd(const T num, const T x) {
	return num + x;
}

inline T mySub(const T num, const T x) {
	return num - x;
}

inline T myMul(const T num, const T x) {
	return num * x;
}

inline T myDiv(const T num, const T x) {
	return num / x;
}

inline T myPow(const T x, const T num) {
//	if((int)(num.real()) == num.real())
//		return pow(x.real(), num.real()); // pow(double, double)
	return pow(x, num.real()); // pow(complex<double>, double)
}
