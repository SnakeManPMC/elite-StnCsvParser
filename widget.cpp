#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
	RedList();
	Doit();
}

Widget::~Widget()
{
	delete ui;
}


void Widget::RedList()
{
	// red list source CSV
	QFile file("d:\\coding\\test_files\\Stations_Red.csv");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open Red List source CSV file"), file.errorString());
	}
	QTextStream in(&file);

	QString line;
	QStringList list;

	while (!in.atEnd())
	{
		line = in.readLine();
		list = line.split(",");
		// if our system is yet not in the list, add it.
		if (!SystemsRedList.contains(list[0]))
		{
			SystemsRedList.append(list[0]);
			ui->textEdit->append("Added to SystemRedList: " + list[0]);
		}
	}
	file.close();
	ui->textEdit->append("SystemsRedList: " + QString::number(SystemsRedList.count()));
}


void Widget::Doit()
{
	// source CSV
	QFile file("d:\\coding\\test_files\\Stations_Maddavo.csv");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open source CSV file"), file.errorString());
	}
	QTextStream in(&file);

	// exported missing pad info CSV
	QFile expfile("d:\\coding\\test_files\\Stations_Maddavo_Missing_Pad_Info.csv");
	if (!expfile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open export CSV file"), expfile.errorString());
	}
	QTextStream exp(&expfile);

	QString header,line;
	QStringList list;

	// statistic numbers
	int stations,ls,pad,totalmissing;
	stations = 0, ls = 0, pad = 0, totalmissing = 0;

	// header
	header = in.readLine();
	exp << header + "\n";

	while (!in.atEnd())
	{
		line = in.readLine();
		// increment stations
		stations++;
		list = line.split(",");
		//ui->textEdit->append("System: " + list[0] + ", Station: " + list[1] + ", Ls: " + list[2] + ", Pad: " + list[4]);
		// incdement statistics numbers
		if (list[2] == "0") ls++;
		if (list[4] == "'?'") pad++;
		// and check if we export our missing line or not
		if ( (list[2] == "0" || list[4] == "'?'") && !SystemsRedList.contains(list[0]) )
		{
			// increment our exported list
			totalmissing++;
			// and export, ding!
			exp << line + "\n";
		}
	}
	file.close();
	expfile.close();
	ui->textEdit->append("Stations in CSV: " + QString::number(stations) + "\nMissing Statistics\nDistance to Star: " + QString::number(ls)
			     + "\nLanding Pad: " + QString::number(pad) + "\nTotal stations exported: " + QString::number(totalmissing) + "\nAll done, exit bitch!");
}
