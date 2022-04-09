#include "GraphicThreads.h"

ThreadCalc::ThreadCalc(QString threadName, Window* win) :
	name(threadName),
	window(win)
{

}

void ThreadCalc::run()
{
	QString string = window->m_integralInput->toPlainText();
	char* string2 = new char[string.size() + 1];
	memset(string2, 0, string.size() + 1);
	memcpy(string2, string.toLocal8Bit().data(), string.size());	
	Integ ob1;
	ob1.SetFunc(string2);
	ob1.Seta(window->m_downBoundInput->text().toInt());
	ob1.Setb(window->m_upBoundInput->text().toInt());
	T res = fun(ob1, &window->progress, PARSER_NEW, PRESITION_M);
	emit window->signalCalculated(res);
	
}


ThreadGNUPlot::ThreadGNUPlot(QString threadName, Window* win) :
	name(threadName),
	window(win)
{

}

void ThreadGNUPlot::run()
{
	system(GNUPLOT);
}
