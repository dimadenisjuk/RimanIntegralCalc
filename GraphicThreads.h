#ifndef GRAPHICTHREADS_H
#define GRAPHICTHREADS_H

#include "GraphicWin.h"

class ThreadCalc : public QThread
{
	Q_OBJECT
	public:
		explicit ThreadCalc(QString threadName, Window* window);
		void run();
	private:
		QString name; // name of the thread
		Window* window;
};

#endif // GRAPHICTHREADS_H
