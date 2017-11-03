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
	setWindowTitle("Station CSV Parser by PMC");
}

Widget::~Widget()
{
	delete ui;
}


void Widget::readRedListCSV()
{
	// red list source CSV
	QFile file("Stations_Red.csv");
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


void Widget::createMissingStationListCSV(QString filename)
{
	// source CSV
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open source CSV file"), file.errorString());
	}
	QTextStream in(&file);

	// exported missing pad info CSV
	QFile expfile("Stations_Maddavo_Missing_Pad_Info.csv");
	if (!expfile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::information(this, tr("Unable to open export CSV file"), expfile.errorString());
	}
	QTextStream exp(&expfile);

	QString header,line;
	QStringList list;

	// statistic numbers
	int station,planetary,ls,pad,totalmissing;
	station = 0, planetary = 0, ls = 0, pad = 0, totalmissing = 0;

	// header
	header = in.readLine();
	exp << header + "\n";

	while (!in.atEnd())
	{
		line = in.readLine();
		list = line.split(",");
		/*
11-03-17 station.csv format with new planetary landing outposts at the end of the line:
unq:name@System.system_id,unq:name,ls_from_star,blackmarket,max_pad_size,market,shipyard,modified,outfitting,rearm,refuel,repair,planetary
'1 G. CAELI','Yize Camp',3490,'N','?','N','N','2016-11-26 20:45:06','N','N','N','N','Y'
		*/
		//ui->textEdit->append("System: " + list[0] + ", Station: " + list[1] + ", Ls: " + list[2] + ", Pad: " + list[4]);
		// incdement statistics numbers
		if (list[2] == "0") ls++;
		if (list[4] == "'?'") pad++;
		if (list[12] == "'N'") station++;
		if (list[12] == "'Y'") planetary++;
		// and check if we export our missing line or not, distance zero, pad unknown and not planetary
		if ( (list[2] == "0" || list[4] == "'?'") && list[12] == "'N'" && !SystemsRedList.contains(list[0]) )
		{
			// increment our exported list
			totalmissing++;
			// and export, ding!
			exp << line + "\n";
		}
	}
	file.close();
	expfile.close();
	ui->textEdit->append("Stations in CSV: " + QString::number(station) + "\nPlanetary: " + QString::number(planetary) + "\nMissing Statistics\nDistance to Star: " + QString::number(ls)
			     + "\nLanding Pad: " + QString::number(pad) + "\nTotal stations exported: " + QString::number(totalmissing) + "\nAll done, exit bitch!");
}


void Widget::runWithParameters(int argc, char *argv[])
{
	//Check_Parameters(argc, argv);
	if (argc < 2)
	{
		printf("Incorrect parameters supplied.\nUsage:\n\nexe SOURCE.csv\n\n");
		exit(1);
	}

	// run the actual program
	readRedListCSV();
	createMissingStationListCSV(argv[1]);
}
