#pragma once
#include <iostream>
using std::string;

//��ͨ��������
enum VehicleType {
	bus, train, airplane
};

//���в���
enum Policy
{
	timeFirst, costFirst, both
};

//ʱ��
struct Time
{
	int year, manth, day, hour;
};

//ʱ�̱�
struct TimeTable
{
	VehicleType type;
	int StartTime;
	int EndTime;
	int cost;
	string TranNumber;
	TimeTable * next;
};

//ͼ�ı�
struct Arc
{
	int Node;
	int tempTime;
	TimeTable *data;
	Arc * next;
};

//ͼ�Ľڵ�
struct Node
{
	string name;
	Arc * FirstArc;
};

//���з���
struct Plan
{
	VehicleType type;
	int StartTime;
	int EndTIme;
	int startID;
	int endID;
};

//�������мƻ�
struct InputPlan
{
	bool PassBy = false;
	Policy policy;
	int StartTime;
	int startNode;
	int ArriveNode;
	int PassByNode;
};