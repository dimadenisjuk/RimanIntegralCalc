#include "GraphicWin.h"

Window::Window(QWidget *parent) :
	QWidget(parent)
	{
		setFixedSize(200, 200);

		QTextEdit* integralInput = new QTextEdit("Input integral", this);

		QCheckBox* parserSelectCheckBox = new QCheckBox("Use new parser", this);
		parserSelectCheckBox->setGeometry(50, 70, 130, 30);

		QPushButton* buttonCalc = new QPushButton("Calculate", this);
		buttonCalc->setToolTip("Calculate integral and plot graphic");
		buttonCalc->setGeometry(50, 100, 130, 30);

		QPushButton* buttonExit = new QPushButton("Exit", this);
		buttonExit->setToolTip("Quit from application");
		buttonExit->setGeometry(50, 150, 130, 30);

		
		// exit from application
		connect(buttonExit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
	}
