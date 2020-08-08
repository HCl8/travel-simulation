#include "PaintMap.h"
#include <qdebug.h>
#include <cmath>


PaintMap::PaintMap(QWidget *parent)
	:QWidget(parent)
	,Map(":/Qt10/Resources/Map2.0.jpg")
	,Airplane(":/Qt10/Resources/airplane.png")
	,Bus(":/Qt10/Resources/bus.png")
	,Train(":/Qt10/Resources/train.png")
	,Wait(":/Qt10/Resources/wait.png")
	,flag(":/Qt10/Resources/flag.png")
	,BusS(":/Qt10/Resources/bus_s.png")
	,TrainS(":/Qt10/Resources/train_s.png")
{
	ifstream input("城市坐标2.0.txt");
	if (!input.is_open())
		qDebug() << "open city point error";
	else {
		int i=0;
		while( 
			input >> City_point[i].name>> City_point[i].x>> City_point[i++].y
			);
	}

	timer10 = startTimer(3);
	mes = 0;
}

PaintMap::~PaintMap()
{
}

//重写的画图函数，根据当前时间画图
void PaintMap::paintEvent(QPaintEvent *)
{
	QPainter a(this);
	QPen pen;
	pen.setColor(QColor(6, 176, 37));
	pen.setWidth(3);
	pen.setStyle(Qt::PenStyle::DashLine);
	pen.setCapStyle(Qt::PenCapStyle::RoundCap);
	a.drawImage(this->rect(), Map);
	a.setPen(pen);
	
	int pre_i = 0;
	if(points.size() > 1)
		pre_i = points[1];
	for (auto i = points.begin() + 2; i < points.end(); i++) {
		a.drawLine(City_point[pre_i].x, City_point[pre_i].y, City_point[*i].x, City_point[*i].y);
		pre_i = *i;
	}
	if (run_flag) {
		pen.setColor(Qt::GlobalColor::darkRed);
		a.setPen(pen);
		a.drawLine(City_point[points[1]].x, City_point[points[1]].y, start_x, start_y);
		a.translate(start_x, start_y);
		a.rotate(rota);
		if (abs(rota) <= 90) {
			switch (vehicle)
			{
			case VehicleType::bus: a.drawImage(-30, -25, Bus); break;
			case VehicleType::train: a.drawImage(-30, -25, Train); break;
			case VehicleType::airplane: a.drawImage(-30, -25, Airplane); break;
			}
		}
		else {
			switch (vehicle)
			{
			case VehicleType::bus: a.drawImage(-30, -25, BusS); break;
			case VehicleType::train: a.drawImage(-30, -25, TrainS); break;
			case VehicleType::airplane: a.drawImage(-30, -25, Airplane); break;
			}
		}
	}
	else if(Start_flag | begin_flag){
		a.drawLine(City_point[points[0]].x, City_point[points[0]].y, City_point[points[1]].x, City_point[points[1]].y);
		a.drawImage(City_point[points[0]].x - 30, City_point[points[0]].y - 25, Wait);
	}
	else if(arrive_flag)
		a.drawImage(City_point[points[1]].x - 30, City_point[points[1]].y - 25, flag);
}

//当到达一个城市后 ，把出发城市移除绘图列表
void PaintMap::remove_first_city(void)
{
	if (points.length() > 2)
		points.remove(0);
	else {
		Start_flag = false;		//时间计时器停止
		arrive_flag = true;		//显示到达标志
		emit start_flag_change();
		emit process_change(100);
	}
	if (TravelTimes.length() > 1)
		TravelTimes.remove(0);
}

//当前路线的行进进度 用来修改绘图函数的参数
void PaintMap::set_process()
{
	int x_change = round(process * x_length / 1000.0);
	int y_change = round(process * y_length / 1000.0);
	start_x = City_point[points[0]].x + x_change;
	start_y = City_point[points[0]].y + y_change;
	update();
}

//根据当前路线计算交通工具旋转角度
void PaintMap::set_rota()
{
	int pointa = points[0];
	int pointb = points[1];
	x_length = (City_point[pointb].x - City_point[pointa].x);
	y_length = City_point[pointb].y - City_point[pointa].y;
	TravelLengh = TravelTimes[0].TimeLength;
	vehicle = TravelTimes[0].type;
	double slope = ((double)City_point[pointa].y - City_point[pointb].y) / (City_point[pointa].x - City_point[pointb].x);
	slope = atan(slope);
	if (x_length >= 0)
		rota = 180 * slope / 3.1415926;
	else
		rota = 180 + 180 * slope / 3.1415926;
	//cout << rota << std::endl;
}

string PaintMap::GetCityName(int a)
{
	if (a >= 0 && a <= 21)
		return City_point[a].name;
	else return 0;
}

//响应定时器时间，用来驱动时间
void PaintMap::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timer10 && Start_flag) {
		if (mes % TravelLengh == 0 && run_flag) {
			if (process == 1000) {
				process = 1;
				remove_first_city();
				run_flag = false;
			}
			else{
				process++;
				set_process();
			}
		}
		if (staticmes % Traveltime == 0)
			emit process_change(++process_bar_process);
		if (mes % 1000 == 0) {
			Nowtime++;
			emit time_change(Nowtime);
			if (Nowtime == TravelTimes[0].startTime) {
				run_flag = true;
				set_rota();
				process = 1;
				set_process();
				mes = 0;
			}
			else if (Nowtime < TravelTimes[0].startTime || Nowtime > TravelTimes[0].startTime + TravelTimes[0].TimeLength) {
				run_flag = false;
				mes = 0;
			}
			update();
		}
		mes++;
		staticmes++;
	}
}

void PaintMap::mousePressEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();
	for (int i = 0; i < CITY_NUM; i++)
		if (abs(x - City_point[i].x) <= SELECT_RECT && abs(y - City_point[i].y) <= SELECT_RECT)
			emit City_Clicked(i);
}

//根据旅行计划创建一系列的绘图点
void PaintMap::CreatePlan(vector<Plan> plan)
{
	TravelPlan temp;
	for (auto i : plan) {
		points.append(i.startID);
		temp.type = i.type;
		temp.startTime = i.StartTime;
		temp.TimeLength = i.EndTIme - i.StartTime;
		TravelTimes.append(temp);
	}
	auto i = plan.end() - 1;
	points.append((*i).endID);
	Traveltime = (*i).EndTIme - Nowtime - 1;
	Traveltime = 1000 / (100.0 / Traveltime);
	process_bar_process = 0;

	begin_flag = true;
}

//设置系统时间
void PaintMap::set_time(int newtime)
{
	Nowtime = newtime - 1;
	mes = 1000;
	staticmes = 1000;
}

void PaintMap::set_start_flag(bool flag)
{
	Start_flag = flag;
}

bool PaintMap::start_flag()
{
	return Start_flag;
}

void PaintMap::set_begin_flag()
{
	begin_flag = false;
}

void PaintMap::clear(void)
{
	Start_flag = false;
	run_flag = false;
	emit start_flag_change();
	emit process_change(0);
	begin_flag = false;
	arrive_flag = false;
	TravelTimes.clear();
	points.clear();
}
