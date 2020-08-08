#include "timeRun.h"

timeRun::timeRun(QObject *parent)
	:QObject(parent)
{
	timeID = startTimer(100);
	msc = 0;
}


timeRun::~timeRun()
{
}

void timeRun::setNowTime(int NewTime)
{
	NowTime = NewTime;
}

int timeRun::GetNowTime()
{
	return 0;
}

void timeRun::timerEvent(QTimerEvent * event)
{
	if (event->timerId() == timeID)
	{
		msc++;
		if (msc == 1000)
			NowTime++;

		if (startFlag) {
			trave();
		}
	}
}

void timeRun::trave(void)
{
	if (msc%travelLen == 0);
	if (process == 100) {
		
	}
}
