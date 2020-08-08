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
	LogFile << "用户完成了输入，请求生成旅行计划,输入计划如下" << endl;
	LogFile << "出发城市:" << CityName[iplan.startID] << endl;
	LogFile << "到达城市：" << CityName[iplan.endID] << endl;
	LogFile << "出发时间：" << iplan.startTime << endl;
	LogFile << "策略：" << PoltyName[iplan.policy] << endl;
	if (iplan.policy == PolicyType::both)
		LogFile << "限时为：第" << iplan.arriveTime / 24 + 1 << "天" << iplan.arriveTime % 24;
	LogFile << "是否途经：" << iplan.PassBy << endl;
	if (iplan.PassBy) {
		LogFile << "途径城市有：";
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
	ifstream input("城市列表.txt");
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
