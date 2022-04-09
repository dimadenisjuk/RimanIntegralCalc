#include "GraphicWin.h"
#include "GraphicThreads.h"

Window::Window(QWidget* parent) :
	QWidget(parent),
	progress(0)
	{
		setFixedSize(320, 300);
		QVBoxLayout* layoutMain = new QVBoxLayout(this);
		layoutMain->setDirection(QBoxLayout::TopToBottom);
		m_integralInput = new QLineEdit();
		m_integralInput->setPlaceholderText("Input integral");
		layoutMain->addWidget(m_integralInput);

		m_downBoundInput = new QLineEdit();
		m_downBoundInput->setPlaceholderText("a");
		m_upBoundInput = new QLineEdit();
		m_upBoundInput->setPlaceholderText("b");

		QVBoxLayout* layoutAB = new QVBoxLayout();
		layoutAB->setDirection(QBoxLayout::LeftToRight);
		layoutAB->addWidget(m_downBoundInput);
		layoutAB->addWidget(m_upBoundInput);
		layoutMain->addLayout(layoutAB);

		m_parserSelectCheckBox = new QCheckBox("Use new parser");
		m_parserSelectCheckBox->setChecked(true);
		layoutMain->addWidget(m_parserSelectCheckBox);

		m_buttonCalc = new QPushButton("Calculate");
		m_buttonCalc->setToolTip("Calculate integral and plot graphic");
		m_buttonCalc->setCheckable("true");

		QPushButton* buttonExit = new QPushButton("Exit");
		buttonExit->setToolTip("Quit from application");

		
		QVBoxLayout* layoutButtons = new QVBoxLayout();
		layoutButtons->setDirection(QBoxLayout::LeftToRight);
		layoutButtons->addWidget(m_buttonCalc);
		layoutButtons->addWidget(buttonExit);
		layoutMain->addLayout(layoutButtons);

		m_presitionLL = new QRadioButton("LL");
		m_presitionL = new QRadioButton("L");
		m_presitionM = new QRadioButton("M");
		m_presitionH = new QRadioButton("H");
		m_presitionHH = new QRadioButton("HH");
		m_presitionFINE = new QRadioButton("FINE");

		QGroupBox* presitionGroup = new QGroupBox("Presition");
		QVBoxLayout* presitionLayout = new QVBoxLayout();
		layoutMain->addWidget(presitionGroup);
		presitionLayout->setDirection(QBoxLayout::LeftToRight);
		presitionLayout->addWidget(m_presitionLL);
		presitionLayout->addWidget(m_presitionL);
		presitionLayout->addWidget(m_presitionM);
		presitionLayout->addWidget(m_presitionH);
		presitionLayout->addWidget(m_presitionHH);
		presitionLayout->addWidget(m_presitionFINE);
		presitionGroup->setLayout(presitionLayout);
		
		m_presitionM->setChecked(true);
	
		m_progressBar = new QProgressBar();
		m_progressBar->setRange(0, 100);
		m_progressBar->setValue(progress);
		m_progressBar->setFixedHeight(10);
		layoutMain->addWidget(m_progressBar);

		QGroupBox* resultGroup = new QGroupBox("Result");
		QVBoxLayout* resultLayout = new QVBoxLayout;
		layoutMain->addWidget(resultGroup);
		
		m_outputField = new QLabel("Result will appear here");
		resultLayout->addWidget(m_outputField);
		resultLayout->addStretch(1);
		resultGroup->setLayout(resultLayout);


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
