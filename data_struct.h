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

//ö�ٽ�ͨ��������
enum VehicleType {
	bus, train, airplane
}; 

// ���в���
enum PolicyType
{
	timeFirst, costFirst, both
};

typedef struct route_info {
	VehicleType vehicle;
	int starttime;
	int timeLen;
	int cost;
}route_info;//·��

typedef vector <route_info> Route;

typedef struct arcnode {
	int ID;
	string city;//�յ����
	Route R;
	struct arcnode * nextarc;
} arcnode;//�յ���н��

typedef struct {//ÿ����������ͷ�ڵ� 
	string city;//��������
	struct arcnode * firstarc;
} vexnode;//ÿ����������ͷ���

//���з���
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

//����ļƻ�
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
