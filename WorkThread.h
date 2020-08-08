#pragma once
#include <qthread.h>
#include "Policy.h"
class WorkThread :
	public QThread
{
	Q_OBJECT
public:
	Policy* polity;
	WorkThread(QObject*,Policy*);
	void run();
	~WorkThread();
signals:
	void Replay();
};

