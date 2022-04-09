#ifndef GRAPHICWIN_H
#define GRAPHICWIN_H

#include "Definitions.h"
#include "Integral.h"
#include <QtGui>
#include <QtWidgets>

class QTextEdit;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QProgressBar;
class QSlider;
class QLabel;
class QGroupBox;
class QVBoxLayout;
class Window : public QWidget {
	Q_OBJECT
	public:
		explicit Window(QWidget *parent = 0);
	private:
		QPushButton* m_buttonCalc;
		QLabel* m_outputField;
		QProgressBar* m_progressBar;
	public: // temporary?
		QLineEdit* m_integralInput;
		QLineEdit* m_downBoundInput;
		QLineEdit* m_upBoundInput;
		QCheckBox* m_parserSelectCheckBox;
		int progress;
	private:
		T _a, _b;
		char* _func;
	private slots:
		void slotButtonClicked(bool checked);
		void slotCalculated(T value);
		void slotStartCalculate();
		//void integralChanged(int);
		//void aChanged(int);
		//void bChanged(int);
	public slots:
		void slotChangeProgress();
	signals:
		void signalCalculated(T value);
};
#endif // GRAPHICWIN_H
