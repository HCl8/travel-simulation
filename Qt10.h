#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt10.h"
#include <qpainter.h>
#include "Policy.h"
#include "WorkThread.h"
#include "Log.h"
using std::stoi;

class Qt10 : public QMainWindow
{
	Q_OBJECT

public:
	Qt10(QWidget *parent = Q_NULLPTR);
	void CloseLog();

private:
	bool inin = false;
	Policy policy;
	Ui::Qt10Class ui;
	WorkThread workThread;
	QIcon stop_img, start_img;
	QString CityID[CITY_NUM];
	InputPlan iPlan;
	string VehiceName[3] = { "Æû³µ","»ð³µ","·É»ú" };
	Log AppLog;
	int GetCityID(const QString);
	void Display_travelPlan(PlanMessage message);
public slots:
	void process_bar_change(int process);
	void stop_btn();
	void refresh_time(int newtime);
	void start_flag_change();
	void request();
	void City_click_event(int city);
	void Clear_Point();
	void ReplayAction();
	void CancelAction();
};
