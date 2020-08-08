#pragma once
#include <iostream>
using std::string;

//交通工具类型
enum VehicleType {
	bus, train, airplane
};

//旅行策略
enum Policy
{
	timeFirst, costFirst, both
};

//时间
struct Time
{
	int year, manth, day, hour;
};

//时刻表
struct TimeTable
{
	VehicleType type;
	int StartTime;
	int EndTime;
	int cost;
	string TranNumber;
	TimeTable * next;
};

//图的边
struct Arc
{
	int Node;
	int tempTime;
	TimeTable *data;
	Arc * next;
};

//图的节点
struct Node
{
	string name;
	Arc * FirstArc;
};

//旅行方案
struct Plan
{
	VehicleType type;
	int StartTime;
	int EndTIme;
	int startID;
	int endID;
};

//输入旅行计划
struct InputPlan
{
	bool PassBy = false;
	Policy policy;
	int StartTime;
	int startNode;
	int ArriveNode;
	int PassByNode;
};