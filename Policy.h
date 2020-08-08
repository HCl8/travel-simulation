#pragma once
#include "data_struct.h"
#include <algorithm>
using std::next_permutation;

struct solution {
	int PreID;
	int PathID;
};

struct SampleIPlan
{
	int startID;
	int endID;
	int time;
	int cost;
};

struct SearchTree
{
	int flag;
	int minTIme;
	int ID;
	SearchTree* Son;
	SearchTree* Brother;
};

class Policy
{
private:
	vexnode Graph[CITY_NUM];
	void inline read_node(ifstream& fin, arcnode* p, string CityName);
	int GetCityID(string name, vexnode* Graph);

	int DFS_maxTime;
	int DFS_minTime;
	int DFS_maxCost;
	int Temp_minCost;
	int Temp_minTime;
	int PrePathCount;
	SampleIPlan IPlan;
	InputPlan inputPlan;
	PlanMessage planMessage;
	solution path[CITY_NUM];
	int Time[CITY_NUM];
	int Cost[CITY_NUM];
	bool DFS_view[CITY_NUM];
	bool PassByView[CITY_NUM];
	vector<solution> tempSolution;
	vector<solution> Both_path;
	vector<solution> Both_path_temp;
	vector<vector<solution>> Path_recoder;
	vector<int> minCost_recoder;
	string Padding;
	int inline GetArriveTime(int startTime, int timelen, int Nowtime);
	void DJS_permutation(bool PolicyFlag);
	bool DJS_searchTree(SearchTree* Tree);
	void DFS_searchTree(SearchTree* Tree);
	void CreateSearchTree(bool* View,SearchTree* &Tree);
	void DeleteTree(SearchTree* tree);
	bool AddPath();
	int FindMaxTime(int startID, int EndID, int startTime, int EndTime);
	void Create_maxTime_SearchTree(SearchTree* Tree);
	
	void CreatePlanMessage();
	void CreatePlanMessage_vector();
	void minPolicy(bool flag);
	void DFS(int startID, int time, int cost);
	void DFS_passby(int startID, int time, int cost);
	
public:
	bool RUNFLAG;
	Policy();
	void SetInputPlan(InputPlan plan);
	void RequstPlan();
	PlanMessage GetPlanMeassage();
	void STOP();
	void FUN();
	bool GetStatus();
	~Policy();
};

