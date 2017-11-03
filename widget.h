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
	void runWithParameters(int argc, char *argv[]);

private:
	Ui::Widget *ui;
	void createMissingStationListCSV(QString filename);
	void readRedListCSV();
	QStringList SystemsRedList;
};

#endif // WIDGET_H
