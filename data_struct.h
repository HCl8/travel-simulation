#pragma once

#include <iostream>
#include<vector>
#include<string>
#include<fstream>
#include<stack>
using std::stack;
using std::vector;
using std::string;
using std::cout;
using std::ifstream;

#define CITY_NUM 20

//枚举交通工具类型
enum VehicleType {
	bus, train, airplane
}; 

// 旅行策略
enum PolicyType
{
	timeFirst, costFirst, both
};

typedef struct route_info {
	VehicleType vehicle;
	int starttime;
	int timeLen;
	int cost;
}route_info;//路线

typedef vector <route_info> Route;

typedef struct arcnode {
	int ID;
	string city;//终点城市
	Route R;
	struct arcnode * nextarc;
} arcnode;//终点城市结点

typedef struct {//每个出发城市头节点 
	string city;//出发城市
	struct arcnode * firstarc;
} vexnode;//每个出发城市头结点

//旅行方案
struct Plan
{
	VehicleType type;
	int StartTime;
	int EndTIme;
	int startID;
	int endID;
	int Cost;
};

struct PlanMessage
{
	int PlanCost;
	int PlanTime;
	string padding;
	vector<Plan> plan;
};

//输入的计划
struct InputPlan
{
	bool PassBy = false;
	int startID;
	int endID;
	int startTime;
	int arriveTime;
	PolicyType policy;
	vector<int> PassByPoint;
};
