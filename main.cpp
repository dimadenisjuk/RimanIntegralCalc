//#define USE_QT6

#include "Definitions.h"
#include "Integral.h"

#ifdef USE_QT6
#include "GraphicWin.h"
#endif //USE_QT6


int main(int argc, char* argv[]) {
#ifdef USE_QT6
	QApplication app(argc, argv); // analog of main() function in QT
	Window window;
		

	window.show();
	return app.exec();
#else
#ifdef MS_WINDOWS
	SetConsoleOutputCP(65001);
#endif //windows
	Integ ob1;
	cout << "# Введите интегрируемую функцию, начало и конец отрезка  " << endl;
	cin >> ob1;
	int progress = 0;
	T res = fun(ob1, &progress, PARSER_NEW, PRESITION_M);
	cout << "# Значение интеграла (площадь под фигурой): " << res << endl;
	cout << "# Введите \"quit\" для выхода из GNUPlot" << endl;
	system(GNUPLOT);
	return 0;
#endif //USE_QT6
}
