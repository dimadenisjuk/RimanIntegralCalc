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
       	IndefIntegCreate(&ob1);
	printf("%s\n", Strings[IStrMainTip].M_LANG);
	scanf("%s%lf%lf", ob1._func, &ob1._a, &ob1._b);
	char* strPresition = malloc(100*sizeof(char));
	scanf("%s", strPresition);
	printf("dsldfj");
	long long presition;
	if(!strcmp(strPresition, "LL"))
		presition = PRESITION_LL;
	if(!strcmp(strPresition, "L"))
		presition = PRESITION_L;
	if(!strcmp(strPresition, "M"))
		presition = PRESITION_M;
	if(!strcmp(strPresition, "H"))
		presition = PRESITION_H;
	if(!strcmp(strPresition, "HH"))
		presition = PRESITION_HH;
	if(!strcmp(strPresition, "FINE"))
		presition = PRESITION_FINE;
	int progress = 0;
	T res = fun(&ob1, &progress, PARSER_NEW, presition);
	printf("%s(%lf;%lf)\n", Strings[IStrIntegValue].M_LANG, creal(res), cimag(res));
	printf("%s\n", Strings[IStrQuitGNUPlot].M_LANG);
	system(GNUPLOT);
	return 0;
#endif //USE_QT6
}
