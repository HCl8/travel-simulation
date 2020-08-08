#include "Log.h"

void Log::PrintQ(QString str)
{
	QByteArray temp = GBK->fromUnicode(str);
	LogFile << temp.data();
	LogFile << endl;
	LogFile.flush();
}

void Log::Print(string str)
{
	addTime();
	LogFile << str << endl;
	LogFile.flush();
}

void Log::InputLog(InputPlan iplan)
{
	addTime();
	LogFile << "�û���������룬�����������мƻ�,����ƻ�����" << endl;
	LogFile << "��������:" << CityName[iplan.startID] << endl;
	LogFile << "������У�" << CityName[iplan.endID] << endl;
	LogFile << "����ʱ�䣺" << iplan.startTime << endl;
	LogFile << "���ԣ�" << PoltyName[iplan.policy] << endl;
	if (iplan.policy == PolicyType::both)
		LogFile << "��ʱΪ����" << iplan.arriveTime / 24 + 1 << "��" << iplan.arriveTime % 24;
	LogFile << "�Ƿ�;����" << iplan.PassBy << endl;
	if (iplan.PassBy) {
		LogFile << ";�������У�";
		for (auto i : iplan.PassByPoint)
			LogFile << CityName[i] << "  ";
		LogFile << endl;
	}
	LogFile.flush();
}

void Log::CloseFile()
{
	LogFile.close();
}

Log::Log() :LogFile("zouni.log", std::ios::app)
{
	ifstream input("�����б�.txt");
	string temp;
	for (int i = 0; i < CITY_NUM; i++) {
		input >> temp;
		CityName[i] = temp;
	}
	GBK = QTextCodec::codecForName("gb2312");
}


Log::~Log()
{
}

void Log::addTime()
{
	NowTime = time(NULL);
	Times = localtime(&NowTime);
	LogFile << Times->tm_year + 1900 << "-" << Times->tm_mon << "-" << Times->tm_mday << " "
		<< Times->tm_hour << ":" << Times->tm_min << ":" << Times->tm_sec << "\t";
}
