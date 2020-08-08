#pragma once

#include <qwidget.h>
#include <qpainter.h>
#include <QTimerEvent>
#include <QMouseEvent>
#include <iostream>
#include <fstream>
#include <vector>
#include "data_struct.h"
using std::string;
using std::ifstream;
using std::vector;
using std::to_string;
#define SELECT_RECT 35

struct city {
	int x;
	int y;
	string name;
};

struct TravelPlan
{
	VehicleType type;
	int startTime;
	int TimeLength;
};

class PaintMap :
	public QWidget
{
	Q_OBJECT
public:
	PaintMap(QWidget *);
	~PaintMap();
	void paintEvent(QPaintEvent *);	
	void remove_first_city(void);
	void set_process();
	void set_rota();
	void CreatePlan(vector<Plan>);
	void set_time(int newtime);
	void set_start_flag(bool flag);
	bool start_flag();
	void set_begin_flag();
	void clear(void);
	string GetCityName(int a);
private:
	city City_point[22];
	QImage Map, Train, Airplane, Bus, Wait, flag, BusS, TrainS;
	QVector<int> points;
	QVector<TravelPlan> TravelTimes;
	int start_x, start_y;
	VehicleType vehicle;
	bool Start_flag = false, run_flag = false, begin_flag = false, arrive_flag = false;
	int rota, x_length, y_length;
	int timer10, process = 0, TravelLengh, mes,staticmes;
	int Nowtime, Traveltime,process_bar_process;
	void timerEvent(QTimerEvent *);
	void mousePressEvent(QMouseEvent*);
signals:
	void process_change(int process);
	void time_change(int nowtime);
	void start_flag_change();
	void City_Clicked(int);
};

