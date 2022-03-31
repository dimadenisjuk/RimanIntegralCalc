#ifndef GRAPHICWIN_H
#define GRAPHICWIN_H

#include <QtGui>
#include <QtWidgets>

class QTextEdit;
class QCheckBox;
class QPushButton;
class QProgressBar;
class QSlider;
class Window : public QWidget {
	Q_OBJECT
	public:
		explicit Window(QWidget *parent = 0);
	private:
		QPushButton *m_button;
};
#endif // GRAPHICWIN_H
