#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();
	void RunMe(int argc, char *argv[]);

private:
	Ui::Widget *ui;
	void Doit(QString filename);
	void RedList();
	QStringList SystemsRedList;
};

#endif // WIDGET_H
