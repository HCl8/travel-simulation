#include "WorkThread.h"

WorkThread::WorkThread(QObject* parent,Policy* polity) :QThread(parent)
{
	this->polity = polity;
}

void WorkThread::run()
{
	polity->RequstPlan();
	emit Replay();
}

WorkThread::~WorkThread()
{
	wait();
}
