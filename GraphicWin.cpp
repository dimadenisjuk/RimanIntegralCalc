#include "GraphicWin.h"
#include "GraphicThreads.h"

Window::Window(QWidget* parent) :
	QWidget(parent),
	progress(0)
	{
		setFixedSize(200, 200);

		m_integralInput = new QLineEdit(this);
		m_integralInput->setPlaceholderText("Input integral");
		m_downBoundInput = new QLineEdit(this);
		m_downBoundInput->setPlaceholderText("a");
		m_upBoundInput = new QLineEdit(this);
		m_upBoundInput->setPlaceholderText("b");
		m_integralInput->setGeometry(10, 10, 180, 20);
		m_downBoundInput->setGeometry(30, 40, 50, 20);
		m_upBoundInput->setGeometry(90, 40, 50, 20);

		QCheckBox* parserSelectCheckBox = new QCheckBox("Use new parser", this);
		parserSelectCheckBox->setGeometry(50, 70, 130, 30);

		m_buttonCalc = new QPushButton("Calculate", this);
		m_buttonCalc->setToolTip("Calculate integral and plot graphic");
		m_buttonCalc->setGeometry(20, 100, 70, 30);
		m_buttonCalc->setCheckable("true");

		QPushButton* buttonExit = new QPushButton("Exit", this);
		buttonExit->setToolTip("Quit from application");
		buttonExit->setGeometry(110, 100, 70, 30);
	
		m_progressBar = new QProgressBar(this);
		m_progressBar->setRange(0, 100);
		m_progressBar->setValue(progress);
		m_progressBar->setGeometry(10, 180, 180, 10);

		m_outputField = new QLabel(this);
		m_outputField->setGeometry(10, 140, 180, 20);
		m_outputField->setText("Result will appear here");


		// binding calculating the integral
		connect(m_buttonCalc, SIGNAL(clicked(bool)), this, SLOT(slotButtonClicked(bool)));
		connect(m_buttonCalc, SIGNAL(clicked()), this, SLOT(slotStartCalculate()));

		// binding exit from application
		connect(buttonExit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

		// binding end of calculating the integral
		connect(this, SIGNAL(signalCalculated(T)), this, SLOT(slotCalculated(T)));
		QTimer* timer = new QTimer;
    		timer->start(100); //каждую 1/10 секунды
    		connect(timer, SIGNAL(timeout()), this, SLOT(slotChangeProgress()));
	}

void Window::slotStartCalculate() { // костыль!!!
	ThreadCalc*  threadCalc = new ThreadCalc("threadCalc", this);
	threadCalc->start();
}

void Window::slotChangeProgress() {
	m_progressBar->setValue(progress);
}

void Window::slotButtonClicked(bool checked) {
	if(checked) {
		m_buttonCalc->setText("Wait...");
		this->show();
	} else {
		m_buttonCalc->setText("Recalculate");
	}
}

void Window::slotCalculated(T value) {
	m_buttonCalc->setChecked(false);
	m_buttonCalc->setText("Recalculate");
	QString str;
	str = QString::fromStdString("(" + to_string(value.real()) + " ; " + to_string(value.imag()) + ")");
	m_outputField->setText(str);
	ThreadGNUPlot* threadGNUPlot = new ThreadGNUPlot("threadGNUPlot", this);
	threadGNUPlot->start();
}
