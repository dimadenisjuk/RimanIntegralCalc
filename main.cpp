#include "Integral.h"
#define USE_QT6

#ifdef USE_QT6
#include "GraphicWin.h"
#endif

#ifdef windows
#include <Windows.h>
#define GNUPLOT ".\\gnuplot\\bin\\gnuplot.exe graphP -"
#else
#define GNUPLOT "gnuplot graphP -"
#endif

int main(int argc, char* argv[]) {
#ifdef USE_QT6
	QApplication app(argc, argv); // analog of main() function in QT
	Window window;
		
	QProgressBar* progressBar = new QProgressBar(&window);
	progressBar->setRange(0, 100);
	progressBar->setValue(20);
	progressBar->setGeometry(10, 180, 180, 10);

	window.show();
	return app.exec();
#else
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
#endif
}
