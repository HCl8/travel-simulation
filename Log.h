#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <qstring.h>
#include <qtextcodec.h>
#include "data_struct.h"
using std::endl;
using std::cout;
using std::ofstream;
using std::string;

class Log
{
private:
	QTextCodec* GBK;
	ofstream LogFile;
	time_t NowTime;
	tm* Times;
	string CityName[CITY_NUM];
	string PoltyName[3] = { "最快达到","最少花费","限时最少花费" };
	void addTime();
public:
	void PrintQ(QString str);
	void Print(string str);
	void InputLog(InputPlan iplan);
	void CloseFile();
	Log();
	~Log();
	void FUN();
};

