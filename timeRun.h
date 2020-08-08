#pragma once
#include <QObject>
#include <QTimerEvent>
#include <vector>
using std::vector;

class timeRun : public QObject
{
	Q_OBJECT
public:
	timeRun(QObject *);
	~timeRun();
	void setNowTime(int NewTime);
	int GetNowTime();
private:
	void timerEvent(QTimerEvent *event);
	void trave(void);
	bool startFlag;
	int timeID;
	int NowTime;
	int msc;
	int travelLen;
	int process;
	vector<Plan> TravelPlan;
};

