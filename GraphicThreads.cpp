#include "GraphicThreads.h"

ThreadCalc::ThreadCalc(QString threadName, Window* win) :
	name(threadName),
	window(win)
{

}
// Поток, вычисляющий значение интеграла
void ThreadCalc::run()
{
	QString string = window->m_integralInput->text();
	char* string2 = new char[string.size() + 1];
	memset(string2, 0, string.size() + 1);
	memcpy(string2, string.toLocal8Bit().data(), string.size());	
	Integ ob1;
	ob1.SetFunc(string2);
	ob1.Seta(window->m_downBoundInput->text().toDouble());
	ob1.Setb(window->m_upBoundInput->text().toDouble());
	bool useNewParser = window->m_parserSelectCheckBox->isChecked();
	long long presition;
	if(window->m_presitionLL->isChecked())
		presition = PRESITION_LL;
	else if(window->m_presitionL->isChecked())
		presition = PRESITION_L;
	else if(window->m_presitionM->isChecked())
		presition = PRESITION_M;
	else if(window->m_presitionH->isChecked())
		presition = PRESITION_H;
	else if(window->m_presitionHH->isChecked())
		presition = PRESITION_HH;
	else if(window->m_presitionFINE->isChecked())
		presition = PRESITION_FINE;
	else
		assert(false); // Не выбрана точность, такого быть не должно
	T res = fun(ob1, &window->progress, useNewParser, presition);
	emit window->signalCalculated(res);
	
}

ThreadGNUPlot::ThreadGNUPlot(QString threadName, Window* win) :
	name(threadName),
	window(win)
{

}
// Поток, в котором запускается GNUPlot
void ThreadGNUPlot::run()
{
	system(GNUPLOT);
}
