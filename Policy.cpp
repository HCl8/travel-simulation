#include "Policy.h"

void Policy::read_node(ifstream &fin, arcnode* p,string CityName = "") {
	route_info temp_route_info; //����һ���洢·����Ϣ���м����
	string temp;
	if(CityName == "") fin >> p->city;//�洢�յ����
	else p->city = CityName;
	fin >> temp_route_info.starttime;
	fin >> temp_route_info.timeLen;
	fin >> temp_route_info.cost;
	fin >> temp;
	if (temp == "train") temp_route_info.vehicle = VehicleType::train;
	else if (temp == "plane") temp_route_info.vehicle = VehicleType::airplane;
	else temp_route_info.vehicle = VehicleType::bus;
	p->R.push_back(temp_route_info);//����·�߷���vector����
}

int Policy::GetCityID(string name ,vexnode* Graph) {
	for (int i = 0; i < CITY_NUM; i++)
		if (Graph[i].city == name)
			return i;
	return -1;
}

//�����ʼ������ͼ
Policy::Policy()
{
	ifstream fin("data2.txt");//��ȡʱ�̱�
	int i = 0;
	string temp;
	for (int i = 0; i < CITY_NUM; i++) {
		Graph[i].city = "";
		Graph[i].firstarc = NULL;
	}
	
	while (!fin.eof()) {
		if (Graph[i].city.size() == 0) {//�����ǰ��������Ϊ��
			fin >> Graph[i].city;//�洢�³�������
			arcnode * p;
			p = new arcnode;//�����½ڵ�ռ�
			read_node(fin, p);
			p->nextarc = Graph[i].firstarc;
			Graph[i].firstarc = p;//�����յ���н������ڽӱ�
		} else {
			fin >> temp;
			if (Graph[i].city == temp) {//�����ǰ���������뵱ǰfin������ͬ
				fin >> temp;
				arcnode * p;
				p = Graph[i].firstarc;
				if (temp != p->city)
					p = NULL;
				if (p) {//������ڵ�ǰ�ڽ�ͷ�µ��ڽӱ����ҵ���ͬ�յ���У�ֱ�ӷ�����vector�У�
					read_node(fin, p, temp);
				}
				else {//�����ڵ�ǰ�ڽ�ͷ�µ��ڽӱ����ҵ���ͬ�յ���У��򽫴˽ڵ�����ڽӱ�
					p = new arcnode;//�����½ڵ�ռ� 
					read_node(fin, p, temp);
					p->nextarc = Graph[i].firstarc;
					Graph[i].firstarc = p;
				}
			}
			else {//�����ǰ���������뵱ǰfin���벻ͬ�������³�������ͷ�ڵ�
				i++;
				if (i == CITY_NUM) break;
				Graph[i].city = temp;
				arcnode * p;
				p = new arcnode;//�����½ڵ�ռ�
				read_node(fin, p);
				p->nextarc = Graph[i].firstarc;
				Graph[i].firstarc = p;
			}

		}
	}

	arcnode *arc;
	for (int i = 0; i < CITY_NUM; i++) {
		arc = Graph[i].firstarc;
		while (arc) {
			arc->ID = GetCityID(arc->city,Graph);
			arc = arc->nextarc;
		}
	}
}

void Policy::SetInputPlan(InputPlan plan)
{
	inputPlan = plan;
}

int Policy::GetArriveTime(int startTime, int timelen, int Nowtime) {
	if (startTime == -1) return Nowtime + timelen;
	return (startTime - Nowtime % 24 + 24) % 24 + Nowtime + timelen;
}

void Policy::CreatePlanMessage()
{
	//�������з���
	vector<Plan> result;
	stack<solution> solu;
	//��·��ѹջ

	int i = IPlan.endID;
	while (i != IPlan.startID) {
		solu.push({ i,path[i].PathID });
		i = path[i].PreID;
	}
	int nowID = IPlan.startID;
	Plan temp;
	//��·�߳�ջ
	while (!solu.empty()) {
		solution thisPath = solu.top();
		arcnode* arc = Graph[nowID].firstarc;
		while (arc) {
			if (arc->ID == thisPath.PreID)
				break;
			arc = arc->nextarc;
		}
		temp.startID = nowID;
		temp.endID = thisPath.PreID;
		temp.type = arc->R[thisPath.PathID].vehicle;
		temp.Cost = arc->R[thisPath.PathID].cost;
		temp.StartTime = arc->R[thisPath.PathID].timeLen;
		temp.EndTIme = arc->R[thisPath.PathID].starttime;
		result.push_back(temp);
		solu.pop();
		nowID = thisPath.PreID;
	}
	//����ʱ����
	int PreTime = inputPlan.startTime;
	for (int i = 0; i < result.size(); i++) {
		result[i].EndTIme = GetArriveTime(result[i].EndTIme, result[i].StartTime, PreTime);
		result[i].StartTime = result[i].EndTIme - result[i].StartTime;
		PreTime = result[i].EndTIme;
	}
	planMessage.plan = result;
	planMessage.PlanCost = Cost[inputPlan.endID];
	planMessage.PlanTime = Time[inputPlan.endID];
	planMessage.padding = Padding;
}

void Policy::CreatePlanMessage_vector()
{
	vector<Plan> result;

	int nowID = inputPlan.startID;
	Plan temp;
	for (int k = 0; k < Both_path.size(); k++)
	{
		solution thisPath = Both_path[k];
		arcnode* arc = Graph[nowID].firstarc;
		while (arc) {
			if (arc->ID == thisPath.PreID)
				break;
			arc = arc->nextarc;
		}
		temp.startID = nowID;
		temp.endID = thisPath.PreID;
		temp.type = arc->R[thisPath.PathID].vehicle;
		temp.Cost = arc->R[thisPath.PathID].cost;
		temp.StartTime = arc->R[thisPath.PathID].timeLen;
		temp.EndTIme = arc->R[thisPath.PathID].starttime;
		result.push_back(temp);
		nowID = thisPath.PreID;
	}
	//����ʱ����
	int PreTime = inputPlan.startTime;
	for (int i = 0; i < result.size(); i++) {
		result[i].EndTIme = GetArriveTime(result[i].EndTIme, result[i].StartTime, PreTime);
		result[i].StartTime = result[i].EndTIme - result[i].StartTime;
		PreTime = result[i].EndTIme;
	}
	planMessage.plan = result;
	planMessage.PlanCost = Cost[inputPlan.endID];
	planMessage.PlanTime = Time[inputPlan.endID];
	planMessage.padding = Padding;
}

Policy::~Policy()
{
	for (int i = 0; i < CITY_NUM; i++)
	{
		arcnode* temp = Graph[i].firstarc;
		arcnode* temp2;
		while (temp)
		{
			temp2 = temp->nextarc;
			delete temp;
			temp = temp2;
		}
	}
}

//����һ�Ͳ��Զ��ĵϽ�˹�����㷨
void Policy::minPolicy(bool flag) //falg Ϊ��Ϊ�������ٲ���  Ϊ�ٱ�ʾʱ����̲���
{
	//��ʼ��
	bool view[CITY_NUM] = { 0 };		//�ýڵ��Ƿ�Ѱ�ҵ����·��
	view[IPlan.startID] = true;
	int Cost[CITY_NUM];
	int Time[CITY_NUM];
	solution path[CITY_NUM];
	for (int i = 0; i < CITY_NUM; i++)
		Cost[i] = Time[i] = INT_MAX >> 1; //�洢���·��
	Cost[IPlan.startID] = IPlan.cost;
	Time[IPlan.startID] = IPlan.time;

	//Ѱ�����·��
	int i = IPlan.startID;
	while (1)
	{
		arcnode * arc = Graph[i].firstarc;
		while (arc) {
			int minCost = arc->R[0].cost ;
			int minTime = GetArriveTime(arc->R[0].starttime, arc->R[0].timeLen, Time[i]);
			int minID = 0 ;
			for (int j = 1; j < arc->R.size(); j++)
			{
				int temp = GetArriveTime(arc->R[j].starttime, arc->R[j].timeLen, Time[i]);
				if (flag && arc->R[j].cost == minCost && temp < minTime) 
				{ //�������ٲ���ʱ ���������� ѡ�����޵���ĳ���
					minCost = arc->R[j].cost;
					minTime = temp;
					minID = j;
				}
				else if (flag && arc->R[j].cost < minCost) {
					minCost = arc->R[j].cost;
					minTime = temp;
					minID = j;
				}
				else if (!flag && temp == minTime && arc->R[j].cost < minCost) 
				{ //��쵽�����ʱ �������ʱ��һ�� ѡ�񻨷��ٵĳ���
					minCost = arc->R[j].cost;
					minTime = temp;
					minID = j;
				}
				else if (!flag && temp < minTime) {
					minCost = arc->R[j].cost;
					minTime = temp;
					minID = j;
				}
			}
			if (flag && Cost[i] + minCost < Cost[arc->ID] || !flag && minTime < Time[arc->ID]) {
				Cost[arc->ID] = minCost + Cost[i];
				Time[arc->ID] = minTime;
				path[arc->ID] = { i,minID };
			}
			arc = arc->nextarc;
		}
		view[i] = true;
		if (i == IPlan.endID) {
			if (Cost[i] == INT_MAX >> 1)
				break;
			while (path[i].PreID != IPlan.startID)
			{
				this->Cost[i] = Cost[i];
				this->Time[i] = Time[i];
				this->path[i] = path[i];
				i = path[i].PreID;
			}
			this->Cost[i] = Cost[i];
			this->Time[i] = Time[i];
			this->path[i] = path[i];
			break;
		}
		int minCost = INT_MAX >> 1;
		int minTime = INT_MAX >> 1;
		int minID = IPlan.startID;
		for (int i = 0; i < CITY_NUM; i++)
			if ((flag && Cost[i] < minCost || !flag && Time[i] < minTime) && !view[i]) {
				minCost = Cost[i];
				minTime = Time[i];
				minID = i;
			}
		i = minID;
	}
}

void Policy::DFS(int startID,int time, int cost) {
	if (cost > DFS_maxCost || time > DFS_maxTime) return;
	if (startID == IPlan.endID) {
		if (time <= inputPlan.arriveTime && cost < Temp_minCost)
		{
			Temp_minCost = cost;
			Temp_minTime = time;
			Both_path.assign(Both_path_temp.begin(), Both_path_temp.end());
		}
		return;
	}
	DFS_view[startID] = true;
	arcnode* arc = Graph[startID].firstarc;
	while (arc) {
		if (!DFS_view[arc->ID]) {
			for (int i = 0; i < arc->R.size(); i++) {
				Both_path_temp.push_back({ arc->ID,i });
				DFS(arc->ID, GetArriveTime(arc->R[i].starttime, arc->R[i].timeLen, time), cost + arc->R[i].cost);
				Both_path_temp.pop_back();
			}
		}
		arc = arc->nextarc;
	}
	DFS_view[startID] = false;
}

void Policy::DFS_passby(int startID,int time, int cost) {
	if (time > DFS_maxTime || cost > DFS_maxCost)
		return;
	if (startID == IPlan.endID) {
		if (cost < minCost_recoder[time - DFS_minTime]) {
			for (int i = PrePathCount; i < Both_path_temp.size(); i++)
				if (!PassByView[Both_path_temp[i].PreID]) return;
			minCost_recoder[time - DFS_minTime] = cost;
			Path_recoder[time - DFS_minTime].assign(Both_path_temp.begin(), Both_path_temp.end());
		}
		return;
	}
	DFS_view[startID] = true;
	arcnode* arc = Graph[startID].firstarc;
	while (arc) {
		if (!DFS_view[arc->ID])
			for (int i = 0; i < arc->R.size(); i++) {
				Both_path_temp.push_back({ arc->ID,i });
				DFS_passby(arc->ID, GetArriveTime(arc->R[i].starttime, arc->R[i].timeLen, time), cost + arc->R[i].cost);
				Both_path_temp.pop_back();
			}
		arc = arc->nextarc;
	}
	DFS_view[startID] = false;
}

//testFuction
void Policy::FUN() {
	InputPlan plan;
	plan.startID = 1;
	plan.endID = 11;
	plan.startTime = 9;
	plan.arriveTime = 45;
	plan.PassBy = true;
	plan.PassByPoint.push_back(3);
	plan.PassByPoint.push_back(7);
	plan.PassByPoint.push_back(16);
	plan.policy = PolicyType::both;
	
	RequstPlan();
}

bool Policy::GetStatus()
{
	return RUNFLAG;
}

//older
void Policy::DJS_permutation(bool PolicyFlag)
{
	int minTime = INT_MAX >> 1;
	int minCost = INT_MAX >> 1;
	vector<solution> tempsolution;
	int end;
	std::sort(inputPlan.PassByPoint.begin(), inputPlan.PassByPoint.end());
	do
	{
		Both_path_temp.clear();
		int PreID = inputPlan.startID;
		Cost[PreID] = 0;
		Time[PreID] = inputPlan.startTime;
		for (auto i : inputPlan.PassByPoint) {
			IPlan.cost = Cost[PreID];
			IPlan.time = Time[PreID];
			IPlan.startID = PreID;
			IPlan.endID = i;
			minPolicy(PolicyFlag);
			end = IPlan.endID;
			tempsolution.clear();
			while (end != IPlan.startID) {
				tempsolution.push_back({ end,path[end].PathID });
				end = path[end].PreID;
			}
			for (auto k = tempsolution.end(); k != tempsolution.begin();)
				Both_path_temp.push_back(*(--k));
			PreID = i;
		}
		IPlan.cost = Cost[PreID];
		IPlan.time = Time[PreID];
		IPlan.startID = PreID;
		IPlan.endID = inputPlan.endID;
		minPolicy(PolicyFlag);
		tempsolution.clear();
		end = IPlan.endID;
		while (end != IPlan.startID) {
				tempsolution.push_back({ end,path[end].PathID });
				end = path[end].PreID;
			}
		for (auto i = tempsolution.end(); i != tempsolution.begin();)
			Both_path_temp.push_back(*(--i));
		if (PolicyFlag) {
			if (minCost > this->Cost[inputPlan.endID]) {
				minTime = Time[inputPlan.endID];
				minCost = Cost[inputPlan.endID];
				Both_path.swap(Both_path_temp);
			}
		}
		else {
			if (minTime > this->Time[inputPlan.endID]) {
				minTime = Time[inputPlan.endID];
				minCost = Cost[inputPlan.endID];
				Both_path.swap(Both_path_temp);
			}
		}
	} while (next_permutation(inputPlan.PassByPoint.begin(), inputPlan.PassByPoint.end()));

	Time[inputPlan.endID] = minTime;
	Cost[inputPlan.endID] = minCost;
}

bool Policy::DJS_searchTree(SearchTree* Tree)
{
	if (!RUNFLAG)return false;
	Tree->minTIme = -1;
	Tree->flag = false;
	int PrepathID = PrePathCount;
	if (Tree->Son == NULL) {
		IPlan.startID = Tree->ID;
		IPlan.endID = inputPlan.endID;
		IPlan.cost = Cost[Tree->ID];
		IPlan.time = Time[Tree->ID];
		minPolicy(inputPlan.policy);
		if (AddPath()) return true;
		if (inputPlan.policy) {
			if (Temp_minCost > this->Cost[inputPlan.endID]) {
				Temp_minTime = Time[inputPlan.endID];
				Temp_minCost = Cost[inputPlan.endID];
				Both_path.assign(Both_path_temp.begin(), Both_path_temp.end());
			}
		}
		else {
			if (Temp_minTime > this->Time[inputPlan.endID]) {
				Temp_minTime = Time[inputPlan.endID];
				Temp_minCost = Cost[inputPlan.endID];
				Both_path.assign(Both_path_temp.begin(), Both_path_temp.end());
			}
		}
		//cout << Time[inputPlan.endID] << std::endl;
		//cout << Cost[inputPlan.endID] << std::endl;
		if (Time[inputPlan.endID] > inputPlan.arriveTime) return false;
		Tree->flag = true;
		return true;
	}
	SearchTree* temp = Tree->Son;
	while (temp)
	{
		IPlan.startID = Tree->ID;
		IPlan.endID = temp->ID;
		IPlan.cost = Cost[Tree->ID];
		IPlan.time = Time[Tree->ID];
		minPolicy(inputPlan.policy);
		PassByView[temp->ID] = true;
		if (AddPath()) {
			temp = temp->Brother; 
			continue;
		}
		if (DJS_searchTree(temp)) temp->flag = true;
		PassByView[temp->ID] = false;
		Both_path_temp.erase(Both_path_temp.begin() + PrepathID, Both_path_temp.end());
		PrePathCount = PrepathID;
		temp = temp->Brother;
	}
	temp = Tree->Son;
	while (temp) {
		if (temp->flag) return true;
		temp = temp->Brother;
	}
	return false;
}

void Policy::DFS_searchTree(SearchTree* Tree)
{
	if (!RUNFLAG)return;
	int minTime = DFS_minTime;
	int maxTime = DFS_maxTime;
	vector<vector<solution>> path_recorder_local(Path_recoder);
	vector<int> minCost_local(minCost_recoder);

	if (Tree->Son == NULL) {

		IPlan.startID = Tree->ID;
		IPlan.endID = inputPlan.endID;

		for (int i = 0; i < Path_recoder.size(); i++)
			if (!Path_recoder[i].empty()) {
				IPlan.cost = minCost_local[i];
				IPlan.time = minTime + i;
				minPolicy(false);
				DFS_maxCost = Cost[IPlan.endID];
				DFS_maxTime = inputPlan.arriveTime;

				Cost[IPlan.endID] = INT_MAX >> 1;
				Both_path_temp.assign(Path_recoder[i].begin(), Path_recoder[i].end());
				DFS(Tree->ID, minTime + i, minCost_local[i]);
			}
		return;
	}
	SearchTree* temp = Tree->Son;
	while (temp)
	{
		if (temp->minTIme == -1) {
			temp = temp->Brother;
			continue;
		}

		IPlan.startID = Tree->ID;
		IPlan.endID = temp->ID;
		IPlan.cost = 0;
		IPlan.time = minTime;
		minPolicy(false);
		DFS_maxCost = Cost[IPlan.endID];

		DFS_minTime = temp->minTIme;
		DFS_maxTime = temp->flag;

		Path_recoder.resize(DFS_maxTime - DFS_minTime + 1);
		minCost_recoder.resize(DFS_maxTime - DFS_minTime + 1);
		for (int i = 0; i < minCost_recoder.size(); i++)
			minCost_recoder[i] = INT_MAX;
		PassByView[temp->ID] = true;
		if (path_recorder_local.empty()) {
			Both_path_temp.clear();
			memset(DFS_view, 0, sizeof(DFS_view));
			DFS_passby(Tree->ID, minTime, 0);
		}
		for (int i = 0; i < path_recorder_local.size(); i++)
			if (!path_recorder_local[i].empty()) {
				IPlan.cost = minCost_local[i];
				IPlan.time = minTime + i;
				minPolicy(false);
				DFS_maxCost = Cost[IPlan.endID];
				minPolicy(true);
				DFS_maxTime = Time[IPlan.endID];
				if (DFS_maxTime > temp->flag) DFS_maxTime = temp->flag;

				Both_path_temp.assign(path_recorder_local[i].begin(), path_recorder_local[i].end());
				memset(DFS_view, 0, sizeof(DFS_view));
				DFS_passby(Tree->ID, minTime + i, minCost_local[i]);
			}
		int temp_min_cost = INT_MAX >> 1;
		for (int i = 0; i < Path_recoder.size(); i++) {
			if (!Path_recoder[i].empty()) {
				if (temp_min_cost < minCost_recoder[i])
					Path_recoder[i].clear();
				else temp_min_cost = minCost_recoder[i];
			}
		}
		
		DFS_maxTime = temp->flag;
		DFS_searchTree(temp);
		PassByView[temp->ID] = false;
		temp = temp->Brother;
	}
}

void Policy::CreateSearchTree(bool* View,SearchTree* &Tree)
{
	SearchTree* temp;
	SearchTree* brothtemp = NULL;
	for (int i = 0; i < inputPlan.PassByPoint.size(); i++)
		if (!View[i]) {
			temp = new SearchTree;
			temp->Brother = NULL;
			temp->Son = NULL;
			if (Tree == NULL)
				Tree = temp;
			else
				brothtemp->Brother = temp;
			brothtemp = temp;
			temp->ID = inputPlan.PassByPoint[i];
			View[i] = true;
			CreateSearchTree(View, temp->Son);
			View[i] = false;
		}
}

void Policy::DeleteTree(SearchTree* Tree)
{
	if (!Tree) return;
	SearchTree* temp = Tree;
	SearchTree* pretemp;
	while (temp)
	{
		DeleteTree(temp->Son);
		pretemp = temp->Brother;
		delete temp;
		temp = pretemp;
	}
}

bool Policy::AddPath()
{
	int end = IPlan.endID;
	tempSolution.clear();
	while (end != IPlan.startID) {
		if (!PassByView[end]) return true;
		tempSolution.push_back({ end,path[end].PathID });
		end = path[end].PreID;
	}
	for (auto k = tempSolution.end(); k != tempSolution.begin();) {
		Both_path_temp.push_back(*(--k));
		PrePathCount++;
	}
	return false;
}

int Policy::FindMaxTime(int startID, int EndID, int startTime, int EndTime)
{
	IPlan.cost = 0;
	IPlan.startID = startID;
	IPlan.endID = EndID;
	stack<solution> solu;
	solution currentPath;
	int NowTime;
	int NowID;
	int result = startTime;
	do {
		while (!solu.empty())
			solu.pop();
		IPlan.time = startTime;
		NowTime = startTime;
		minPolicy(false);
		if (Time[EndID] > EndTime)
			return result;
		int i = IPlan.endID;
		while (i != IPlan.startID) {
			solu.push({ i,path[i].PathID });
			i = path[i].PreID;
		}
		NowID = IPlan.startID;
		result = startTime;
		while (!solu.empty())
		{
			currentPath = solu.top();
			arcnode* arc = Graph[NowID].firstarc;
			while (arc) {
				if (arc->ID == currentPath.PreID)
					break;
				arc = arc->nextarc;
			}
			if (arc->R[currentPath.PathID].vehicle != VehicleType::bus) {
				int k = GetArriveTime(arc->R[currentPath.PathID].starttime, arc->R[currentPath.PathID].timeLen, NowTime);
				startTime += (k - arc->R[currentPath.PathID].timeLen) - NowTime;
				break;
			}
			NowTime += arc->R[currentPath.PathID].timeLen;
			solu.pop();
			NowID = currentPath.PreID;
		}
		startTime++;
	} while (Time[EndID] <= EndTime);
	return startTime;
}

void Policy::Create_maxTime_SearchTree(SearchTree* Tree)
{
	Tree->flag = 0;
	int TempFlag;
	SearchTree* temp = Tree->Son;
	if (temp == NULL) {
		Tree->flag = FindMaxTime(Tree->ID, inputPlan.endID, Tree->minTIme, inputPlan.arriveTime);
	}
	else {
		while (temp)
		{
			if (!temp->flag) {
				temp = temp->Brother;
				continue;
			}
			IPlan.cost = 0;
			IPlan.startID = Tree->ID;
			IPlan.endID = temp->ID;
			IPlan.time = Tree->minTIme;
			minPolicy(false);
			temp->minTIme = Time[IPlan.endID];
			Create_maxTime_SearchTree(temp);
			TempFlag = FindMaxTime(Tree->ID, temp->ID, Tree->minTIme, temp->flag);
			if (Tree->flag < TempFlag)
				Tree->flag = TempFlag;
			temp = temp->Brother;
		}
	}
}

void Policy::RequstPlan()
{
	RUNFLAG = true;
	Padding = "";
	IPlan.startID = inputPlan.startID;
	IPlan.endID = inputPlan.endID;
	IPlan.cost = 0;
	IPlan.time = inputPlan.startTime;
	if (!inputPlan.PassBy) {
		if (inputPlan.policy != PolicyType::both) {
			minPolicy(inputPlan.policy);
			CreatePlanMessage();
			return ;
		}
		else {
			//��������ĵ���ʱ��
			minPolicy(true);
			DFS_maxTime = Time[IPlan.endID];
			if (inputPlan.arriveTime >= DFS_maxTime) {
				Padding = "��������ʱ�䳬�����ٻ��Ѳ��Ե�ʱ�䣬�Ѹ�����С���Ѳ���";
				CreatePlanMessage();
				return ;
			}
			minPolicy(false);
			DFS_maxCost = Cost[inputPlan.endID];
			if (inputPlan.arriveTime <= Time[inputPlan.endID]) {
				Padding = "��������ʱ��������쵽����Ե�ʱ�䣬�Ѹ�����쵽�����";
				CreatePlanMessage();
				return ;
			}
			//�����������·��
			DFS_maxTime = inputPlan.arriveTime;
			Cost[inputPlan.endID] = Time[inputPlan.endID] = INT_MAX >> 1;
			memset(DFS_view, 0, sizeof(DFS_view));
			Both_path_temp.clear();
			DFS(inputPlan.startID,inputPlan.startTime, 0);
			Time[inputPlan.endID] = Temp_minTime;
			Cost[inputPlan.endID] = Temp_minCost;
			CreatePlanMessage_vector();
			return ;
		}
	}
	else {
		if (inputPlan.policy != PolicyType::both) {
			bool* view = new bool[inputPlan.PassByPoint.size()];
			memset(view, 0, sizeof(bool) * inputPlan.PassByPoint.size());
			SearchTree* tree = new SearchTree;
			tree->ID = inputPlan.startID;
			tree->Son = NULL;
			tree->Brother = NULL;
			CreateSearchTree(view, tree->Son);

			memset(PassByView, 1, sizeof(PassByView));
			PrePathCount = 0;
			for (auto i : inputPlan.PassByPoint)
				PassByView[i] = false;
			Cost[inputPlan.startID] = 0;
			Time[inputPlan.startID] = inputPlan.startTime;
			Temp_minCost = INT_MAX >> 1;
			Temp_minTime = INT_MAX >> 1;
			inputPlan.arriveTime = 100;
			Both_path_temp.clear();
			PrePathCount = 0;
			DJS_searchTree(tree);
			Time[inputPlan.endID] = Temp_minTime;
			Cost[inputPlan.endID] = Temp_minCost;
			DeleteTree(tree);
			if(RUNFLAG) CreatePlanMessage_vector();
			return ;
		}
		else {
			//����������
			bool* view = new bool[inputPlan.PassByPoint.size()];
			memset(view, 0, sizeof(bool) * inputPlan.PassByPoint.size());
			SearchTree* tree = new SearchTree;
			tree->ID = inputPlan.startID;
			tree->Son = NULL;
			tree->Brother = NULL;
			CreateSearchTree(view, tree->Son);

			//��鵽��ʱ���Ƿ��ں���Χ��
			memset(PassByView, 1, sizeof(PassByView));
			for (auto i : inputPlan.PassByPoint)
				PassByView[i] = false;
			Cost[inputPlan.startID] = 0;
			Time[inputPlan.startID] = inputPlan.startTime;
			Temp_minCost = INT_MAX >> 1;
			Temp_minTime = INT_MAX >> 1;
			Path_recoder.clear();
			inputPlan.policy = PolicyType::costFirst;
			Both_path_temp.clear();
			PrePathCount = 0;
			DJS_searchTree(tree);
			Time[inputPlan.endID] = Temp_minTime;
			Cost[inputPlan.endID] = Temp_minCost;
			if (inputPlan.arriveTime >= Temp_minTime) {
				Padding = "��������ʱ�䳬�����ٻ��Ѳ��Ե�ʱ�䣬�Ѹ�����С���Ѳ���";
				if(RUNFLAG) CreatePlanMessage_vector();
				delete(tree);
				return ;
			}
			
			//ȷ������·��
			memset(PassByView, 1, sizeof(PassByView));
			Cost[inputPlan.startID] = 0;
			Time[inputPlan.startID] = inputPlan.startTime;
			Temp_minCost = INT_MAX >> 1;
			Temp_minTime = INT_MAX >> 1;
			Path_recoder.clear();
			inputPlan.policy = PolicyType::timeFirst;
			Both_path_temp.clear();
			PrePathCount = 0;
			DJS_searchTree(tree);
			Time[inputPlan.endID] = Temp_minTime;
			Cost[inputPlan.endID] = Temp_minCost;
			if (inputPlan.arriveTime <= Temp_minTime) {
				Padding = "��������ʱ��������쵽����Ե�ʱ�䣬�Ѹ�����쵽�����";
				if(RUNFLAG) CreatePlanMessage_vector();
				delete(tree);
				return ;
			}

			tree->flag = true;
			tree->minTIme = inputPlan.startTime;
			Create_maxTime_SearchTree(tree);

			//���������������������
			DFS_maxCost = 0;
			DFS_minTime = inputPlan.startTime;
			DFS_maxTime = inputPlan.startTime;
			Temp_minCost = INT_MAX >> 1;
			Temp_minTime = INT_MAX >> 1;
			Path_recoder.clear();
			minCost_recoder.clear();
			tree->flag = true;
			DFS_searchTree(tree);
			Time[inputPlan.endID] = Temp_minTime;
			Cost[inputPlan.endID] = Temp_minCost;
			DeleteTree(tree);
			if(RUNFLAG) CreatePlanMessage_vector();
			return ;
		}
	}
}

PlanMessage Policy::GetPlanMeassage()
{
	return planMessage;
}

void Policy::STOP()
{
	RUNFLAG = false;
}
