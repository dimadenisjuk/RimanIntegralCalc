#include "Integral.h"

#ifdef windows
#include <Windows.h>
#define GNUPLOT ".\\gnuplot\\bin\\gnuplot.exe graphP -"
#else
#define GNUPLOT "gnuplot graphP -"
#endif

int main() {
#ifdef windows
	SetConsoleOutputCP(65001);
#endif
	Integ ob1;
	cout << "# Введите интегрируемую функцию, начало и конец отрезка  " << endl;
	cin >> ob1;
	T res = fun(ob1);
	cout << "# Значение интеграла (площадь под фигурой): " << res << endl;
	cout << "# Введите \"quit\" для выхода из GNUPlot" << endl;
	system(GNUPLOT);
	return 0;
}
