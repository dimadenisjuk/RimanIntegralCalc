#include "Integral.h"

int main() {
	setlocale(LC_ALL, "Rus");
	Integ ob1;
	cout << "# Введите интегрируемую функцию, начало и конец отрезка  " << endl;
	cin >> ob1;
	T res = fun(ob1);
	cout << "# Значение интеграла (площадь под фигурой) : " << res << endl;
	system("gnuplot graphP");
	return 0;
}
