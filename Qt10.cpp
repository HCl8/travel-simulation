#include "Qt10.h"

Qt10::Qt10(QWidget* parent)
	: QMainWindow(parent), start_img(":/Qt10/play.png"), stop_img(":/Qt10/stop.png"), policy(), workThread(this, &policy), AppLog()
{
	ui.setupUi(this);
	AppLog.Print("��������");

	connect(ui.display, SIGNAL(process_change(int)), this, SLOT(process_bar_change(int)));
	connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(stop_btn()));
	connect(ui.display, SIGNAL(time_change(int)), this, SLOT(refresh_time(int)));
	connect(ui.display, SIGNAL(start_flag_change()), this, SLOT(start_flag_change()));
	connect(ui.RequestBtn, SIGNAL(clicked()), this, SLOT(request()));
	connect(ui.display, SIGNAL(City_Clicked(int)), this, SLOT(City_click_event(int)));
	connect(ui.btnPointClear, SIGNAL(clicked()), this, SLOT(Clear_Point()));
	connect(&workThread, SIGNAL(Replay()), this, SLOT(ReplayAction()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(CancelAction()));

	ifstream input("�����б�.txt");
	string temp;
	for (int i = 0; i < CITY_NUM; i++) {
		input >> temp;
		CityID[i] = QString::fromLocal8Bit(temp.data());
	}

	ui.TimeStart->setValidator(new QIntValidator(0, 24));
	ui.TimeEndDay->setValidator(new QIntValidator(0, 10));
	ui.TimeEndHours->setValidator(new QIntValidator(0, 24));
	iPlan.startID = 0;
	iPlan.endID = 0;
	policy.RUNFLAG = false;
}

void Qt10::process_bar_change(int process) {
	ui.progressBar->setValue(process);
}

void Qt10::CloseLog()
{
	AppLog.Print("�û��˳�����");
	AppLog.CloseFile();
}

int Qt10::GetCityID(const QString name)
{
	for (int i = 0; i < CITY_NUM; i++)
		if (name == CityID[i])
			return i;
	return -1;
}

void Qt10::Display_travelPlan(PlanMessage message)
{
	int i;
	string temp;
	string tempi;
	temp.append(message.padding);
	tempi = "����ʱ�䣺��" + to_string(message.PlanTime / 24 + 1) + "��" + to_string(message.PlanTime % 24) + "�㵽��\n";
	temp.append(tempi);
	tempi = "���ѣ�" + to_string(message.PlanCost) + "\n";
	temp.append(tempi);
	for (i = 0; i < message.plan.size(); i++) {
		tempi = to_string(i + 1) + ":��ͨ����:" + VehiceName[message.plan[i].type] + "  ���ѣ�" + to_string(message.plan[i].Cost) + "\n";
		temp.append(tempi);
		int day = message.plan[i].StartTime / 24 + 1;
		int hours = message.plan[i].StartTime % 24;
		tempi = to_string(i + 1) + ":" + ui.display->GetCityName(message.plan[i].startID) + "��" + to_string(day) + "�� " + to_string(hours) + "�����\n";
		temp.append(tempi);
		day = message.plan[i].EndTIme / 24 + 1;
		hours = message.plan[i].EndTIme % 24;
		tempi = to_string(i + 1) + ":" + ui.display->GetCityName(message.plan[i].endID) + "��" + to_string(day) + "�� " + to_string(hours) + "�㵽��\n";
		temp.append(tempi);
	}
	ui.textB_Plan->setText(QString::fromLocal8Bit(temp.data()));
}

void Qt10::stop_btn()
{
	if (!inin)return;
	if (ui.display->start_flag()) {
		ui.display->set_start_flag(false);
		ui.toolButton->setIcon(start_img);
	}
	else {
		ui.display->set_start_flag(true);
		ui.toolButton->setIcon(stop_img);
	}
	ui.display->set_begin_flag();
}

void Qt10::refresh_time(int newtime)
{
	int day = newtime / 24 + 1;
	int hours = newtime % 24;
	QString a = QString::fromLocal8Bit(("��" + to_string(day) + "��\n" + to_string(hours) + ":00").data());
	ui.Timelab->setText(a);
}

void Qt10::start_flag_change()
{
	ui.toolButton->setIcon(start_img);
}

void Qt10::request()
{
	if (policy.GetStatus())return;
	iPlan.startTime = ui.TimeStart->text().toInt();
	iPlan.arriveTime = ui.TimeEndDay->text().toInt() * 24 + ui.TimeEndHours->text().toInt() - 24;
	if (iPlan.arriveTime < 0) iPlan.arriveTime += 24;
	iPlan.policy = (PolicyType)ui.Polity->currentIndex();
	if (iPlan.PassByPoint.size() != 0) iPlan.PassBy = true;
	else iPlan.PassBy = false;
	if (iPlan.PassByPoint.size() > 10 || iPlan.policy == PolicyType::both && iPlan.PassByPoint.size() > 7) {
		ui.textB_Plan->setText(QString::fromLocal8Bit(";������̫�࣬����������"));
		AppLog.Print("�û�����������ͣ�;������̫��");
		return;
	}
	if (iPlan.startID == iPlan.endID ||
		iPlan.policy == PolicyType::both && iPlan.startTime >= iPlan.arriveTime
		) {
		ui.textB_Plan->setText(QString::fromLocal8Bit("���������������"));
		AppLog.Print("�û�����������ͣ��������");
		return;
	}
	if (iPlan.PassBy)
		for (auto i : iPlan.PassByPoint)
			if (i == iPlan.startID || i == iPlan.endID) {
				ui.textB_Plan->setText(QString::fromLocal8Bit("���������������"));
				AppLog.Print("�û�����������ͣ��������");
				return;
			}

	policy.SetInputPlan(iPlan);
	workThread.start();
	ui.textB_Plan->setText(QString::fromLocal8Bit("����Ѱ��·����������Ҫ��ʱ��"));
	AppLog.InputLog(iPlan);
}

void Qt10::City_click_event(int city)
{
	if (ui.btnStart->isChecked()) {
		iPlan.startID = city;
		ui.lableStart->setText(CityID[city]);
		ui.btnStart->setChecked(false);
	}
	else if (ui.btnEnd->isChecked()) {
		iPlan.endID = city;
		ui.btnEnd->setChecked(false);
		ui.lableEnd->setText(CityID[city]);
	}
	else if (ui.btnAddPoint->isChecked()) {
		for (auto i : iPlan.PassByPoint)
			if (i == city)return;
		iPlan.PassByPoint.push_back(city);
		QString temp = ui.labelPassPoint->text();
		if (temp == "") ui.labelPassPoint->setText(CityID[city]);
		else {
			if (iPlan.PassByPoint.size() == 8) temp += "\n" + CityID[city];
			else temp += "," + CityID[city];
			ui.labelPassPoint->setText(temp);
		}
	}
}

void Qt10::Clear_Point()
{
	ui.labelPassPoint->setText("");
	iPlan.PassByPoint.clear();
}

void Qt10::ReplayAction()
{
	if (!policy.GetStatus()) {
		ui.textB_Plan->setText(QString::fromLocal8Bit("��ȡ��"));
		return;
	}
	policy.STOP();
	inin = true;
	PlanMessage message = policy.GetPlanMeassage();
	ui.display->clear();
	ui.display->set_time(iPlan.startTime);
	ui.display->CreatePlan(message.plan);
	Display_travelPlan(message);
	ui.display->update();
	AppLog.Print("�������з���");
	AppLog.PrintQ(ui.textB_Plan->toPlainText());
}

void Qt10::CancelAction()
{
	AppLog.Print("�û�ȡ���������з���");
	policy.RUNFLAG = false;
}
