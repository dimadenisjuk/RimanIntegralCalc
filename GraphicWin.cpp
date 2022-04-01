#include "GraphicWin.h"
#include "GraphicThreads.h"

Window::Window(QWidget* parent) :
	QWidget(parent)
	{
		setFixedSize(200, 200);

		m_integralInput = new QTextEdit(this);
		m_integralInput->setPlaceholderText("Input integral");
		m_downBoundInput = new QLineEdit(this);
		m_downBoundInput->setPlaceholderText("a");
		m_upBoundInput = new QLineEdit(this);
		m_upBoundInput->setPlaceholderText("b");
		m_downBoundInput->setGeometry(30, 40, 50, 20);
		m_upBoundInput->setGeometry(90, 40, 50, 20);

		QCheckBox* parserSelectCheckBox = new QCheckBox("Use new parser", this);
		parserSelectCheckBox->setGeometry(50, 70, 130, 30);

		m_buttonCalc = new QPushButton("Calculate", this);
		m_buttonCalc->setToolTip("Calculate integral and plot graphic");
		m_buttonCalc->setGeometry(50, 100, 130, 30);
		m_buttonCalc->setCheckable("true");

		QPushButton* buttonExit = new QPushButton("Exit", this);
		buttonExit->setToolTip("Quit from application");
		buttonExit->setGeometry(50, 150, 130, 30);
		

		// binding calculating the integral
		connect(m_buttonCalc, SIGNAL(clicked(bool)), this, SLOT(slotButtonClicked(bool)));
		connect(m_buttonCalc, SIGNAL(clicked()), this, SLOT(slotStartCalculate()));

		// binding exit from application
		connect(buttonExit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
		
		// binding end of calculating the integral
		connect(this, SIGNAL(signalCalculated(T)), this, SLOT(slotCalculated(T)));
	}

void Window::slotStartCalculate() {
	ThreadCalc*  threadCalc = new ThreadCalc("threadCalc", this);
	threadCalc->start();
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
	m_buttonCalc->setText(str);
}
