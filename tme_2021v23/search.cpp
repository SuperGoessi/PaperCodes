#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include "inout.h"
#include "checkFunc.h"
#include "timeexpanded.h"
#include "search.h"
#include "binaryheap.h"
#include "netbuild.h"
#include "test.h"
#include "sepsearch.h"
#include "deleteStation.h"

int		INF_DIST = 2000000;
using namespace std;

void assign_stp_ap_init(int startNode, int endNode, int starttime, int timebudget, int maxTransferTime, int approxATime, int densityfactor)
{
	instance_stpap->startNode = startNode;
	instance_stpap->endNode = endNode;
	instance_stpap->startTime = starttime;
	instance_stpap->timeBudget = timebudget;
	instance_stpap->maxTrasferTime = maxTransferTime;
	instance_stpap->approxActivityTime = approxATime;
	instance_stpap->densityfactor = densityfactor;
}

void allocate_space_result_pv0_forward(int num)
{
	sp_sepresult_pv0_forward->numOfNode = instance_pvcoords->roadNodeNum;

	sp_sepresult_pv0_forward->searched = generate_int_matrix(num, sp_sepresult_pv0_forward->numOfNode);
	sp_sepresult_pv0_forward->parent = generate_int_matrix(num, sp_sepresult_pv0_forward->numOfNode);
	sp_sepresult_pv0_forward->cost = generate_int_matrix(num, sp_sepresult_pv0_forward->numOfNode);

	init_space_result_pv0_forward(num);
}

void allocate_space_result_pt0_forward(int num)
{
	sp_sepresult_pt0_forward->numOfNode = instance_spnetpt->nodeNum;
	sp_sepresult_pt0_forward->numOfSta = instance_ptstationcoords->stationNum;

	sp_sepresult_pt0_forward->trscost = generate_int_matrix(num, sp_sepresult_pt0_forward->numOfNode);
	sp_sepresult_pt0_forward->parent = generate_int_matrix(num, sp_sepresult_pt0_forward->numOfNode);
	sp_sepresult_pt0_forward->cost = generate_int_matrix(num, sp_sepresult_pt0_forward->numOfNode);

	sp_sepresult_pt0_forward->stationcost = generate_int_matrix(num, sp_sepresult_pt0_forward->numOfSta);
	sp_sepresult_pt0_forward->searched = generate_int_matrix(num, sp_sepresult_pt0_forward->numOfSta);
	

	init_space_result_pt0_forward(num);
}
void allocate_space_result_pt1_forward(int num)
{
	sp_sepresult_pt1_forward->numOfNode = instance_spnetpt->nodeNum;
	sp_sepresult_pt1_forward->numOfSta = instance_ptstationcoords->stationNum;

	sp_sepresult_pt1_forward->trscost = generate_int_matrix(num, sp_sepresult_pt1_forward->numOfNode);
	sp_sepresult_pt1_forward->parent = generate_int_matrix(num, sp_sepresult_pt1_forward->numOfNode);
	sp_sepresult_pt1_forward->cost = generate_int_matrix(num, sp_sepresult_pt1_forward->numOfNode);

	sp_sepresult_pt1_forward->stationcost = generate_int_matrix(num, sp_sepresult_pt1_forward->numOfSta);
	sp_sepresult_pt1_forward->searched = generate_int_matrix(num, sp_sepresult_pt1_forward->numOfSta);


	init_space_result_pt1_forward(num);
}
void allocate_space_result_pv1_forward(int num)
{
	sp_sepresult_pv1_forward->numOfNode = instance_pvcoords->roadNodeNum;

	sp_sepresult_pv1_forward->searched = generate_int_matrix(num, sp_sepresult_pv1_forward->numOfNode);
	sp_sepresult_pv1_forward->parent = generate_int_matrix(num, sp_sepresult_pv1_forward->numOfNode);
	sp_sepresult_pv1_forward->cost = generate_int_matrix(num, sp_sepresult_pv1_forward->numOfNode);

	init_space_result_pv1_forward(num);
}

void allocate_space_result_pv1_backward(int num)
{
	sp_sepresult_pv1_backward->numOfNode = instance_pvcoords->roadNodeNum;

	sp_sepresult_pv1_backward->searched = generate_int_matrix(num, sp_sepresult_pv1_backward->numOfNode);
	sp_sepresult_pv1_backward->parent = generate_int_matrix(num, sp_sepresult_pv1_backward->numOfNode);
	sp_sepresult_pv1_backward->cost = generate_int_matrix(num, sp_sepresult_pv1_backward->numOfNode);

	init_space_result_pv1_backward(num);
}
void allocate_space_result_pt1_backward(int num)
{
	sp_sepresult_pt1_backward->numOfNode = instance_spnetpt->nodeNum;
	sp_sepresult_pt1_backward->numOfSta = instance_ptstationcoords->stationNum;

	sp_sepresult_pt1_backward->trscost = generate_int_matrix(num, sp_sepresult_pt1_backward->numOfNode);
	sp_sepresult_pt1_backward->parent = generate_int_matrix(num, sp_sepresult_pt1_backward->numOfNode);
	sp_sepresult_pt1_backward->cost = generate_int_matrix(num, sp_sepresult_pt1_backward->numOfNode);

	sp_sepresult_pt1_backward->stationcost = generate_int_matrix(num, sp_sepresult_pt1_backward->numOfSta);
	sp_sepresult_pt1_backward->searched = generate_int_matrix(num, sp_sepresult_pt1_backward->numOfSta);

	init_space_result_pt1_backward(num);
}

void allocate_space_result_pt0_backward(int num)
{
	sp_sepresult_pt0_backward->numOfNode = instance_spnetpt->nodeNum;
	sp_sepresult_pt0_backward->numOfSta = instance_ptstationcoords->stationNum;

	sp_sepresult_pt0_backward->trscost = generate_int_matrix(num, sp_sepresult_pt0_backward->numOfNode);
	sp_sepresult_pt0_backward->parent = generate_int_matrix(num, sp_sepresult_pt0_backward->numOfNode);
	sp_sepresult_pt0_backward->cost = generate_int_matrix(num, sp_sepresult_pt0_backward->numOfNode);

	sp_sepresult_pt0_backward->stationcost = generate_int_matrix(num, sp_sepresult_pt0_backward->numOfSta);
	sp_sepresult_pt0_backward->searched = generate_int_matrix(num, sp_sepresult_pt0_backward->numOfSta);

	init_space_result_pt0_backward(num);
}

void allocate_space_result_pv0_backward(int num)
{
	sp_sepresult_pv0_backward->numOfNode = instance_pvcoords->roadNodeNum;

	sp_sepresult_pv0_backward->searched = generate_int_matrix(num, sp_sepresult_pv0_backward->numOfNode);
	sp_sepresult_pv0_backward->parent = generate_int_matrix(num, sp_sepresult_pv0_backward->numOfNode);
	sp_sepresult_pv0_backward->cost = generate_int_matrix(num, sp_sepresult_pv0_backward->numOfNode);

	init_space_result_pv0_backward(num);
}

void init_space_result_pv0_forward(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < sp_sepresult_pv0_forward->numOfNode; j++)
		{
			sp_sepresult_pv0_forward->searched[i][j] = 0;
			sp_sepresult_pv0_forward->parent[i][j] = -1;
			sp_sepresult_pv0_forward->cost[i][j] = INF_DIST;
		}
	}
}
void init_space_result_pt0_forward(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < sp_sepresult_pt0_forward->numOfNode; j++)
		{
			sp_sepresult_pt0_forward->trscost[i][j] = 0;
			sp_sepresult_pt0_forward->parent[i][j] = -1;
			sp_sepresult_pt0_forward->cost[i][j] = INF_DIST;
		}

		for (int k = 0; k < sp_sepresult_pt0_forward->numOfSta; k++)
		{
			sp_sepresult_pt0_forward->searched[i][k] = 0;
			sp_sepresult_pt0_forward->stationcost[i][k] = INF_DIST;
			
		}
	}
}

void init_space_result_pt1_forward(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < sp_sepresult_pt1_forward->numOfNode; j++)
		{
			sp_sepresult_pt1_forward->trscost[i][j] = 0;
			sp_sepresult_pt1_forward->parent[i][j] = -1;
			sp_sepresult_pt1_forward->cost[i][j] = INF_DIST;
		}

		for (int k = 0; k < sp_sepresult_pt1_forward->numOfSta; k++)
		{
			sp_sepresult_pt1_forward->searched[i][k] = 0;
			sp_sepresult_pt1_forward->stationcost[i][k] = INF_DIST;

		}
	}
}
void init_space_result_pv1_forward(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < sp_sepresult_pv1_forward->numOfNode; j++)
		{
			sp_sepresult_pv1_forward->searched[i][j] = 0;
			sp_sepresult_pv1_forward->parent[i][j] = -1;
			sp_sepresult_pv1_forward->cost[i][j] = INF_DIST;
		}
	}
}

void init_space_result_pv1_backward(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < sp_sepresult_pv1_backward->numOfNode; j++)
		{
			sp_sepresult_pv1_backward->searched[i][j] = 0;
			sp_sepresult_pv1_backward->parent[i][j] = -1;
			sp_sepresult_pv1_backward->cost[i][j] = INF_DIST;
		}
	}
}
void init_space_result_pt1_backward(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < sp_sepresult_pt1_backward->numOfNode; j++)
		{
			sp_sepresult_pt1_backward->trscost[i][j] = 0;
			sp_sepresult_pt1_backward->parent[i][j] = -1;
			sp_sepresult_pt1_backward->cost[i][j] = INF_DIST;
		}

		for (int k = 0; k < sp_sepresult_pt1_backward->numOfSta; k++)
		{
			sp_sepresult_pt1_backward->searched[i][k] = 0;
			sp_sepresult_pt1_backward->stationcost[i][k] = INF_DIST;

		}
	}
}
void init_space_result_pt0_backward(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < sp_sepresult_pt0_backward->numOfNode; j++)
		{
			sp_sepresult_pt0_backward->trscost[i][j] = 0;
			sp_sepresult_pt0_backward->parent[i][j] = -1;
			sp_sepresult_pt0_backward->cost[i][j] = INF_DIST;
		}

		for (int k = 0; k < sp_sepresult_pt0_backward->numOfSta; k++)
		{
			sp_sepresult_pt0_backward->searched[i][k] = 0;
			sp_sepresult_pt0_backward->stationcost[i][k] = INF_DIST;

		}
	}
}

void init_space_result_pv0_backward(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < sp_sepresult_pv0_backward->numOfNode; j++)
		{
			sp_sepresult_pv0_backward->searched[i][j] = 0;
			sp_sepresult_pv0_backward->parent[i][j] = -1;
			sp_sepresult_pv0_backward->cost[i][j] = INF_DIST;
		}
	}
}

void delete_pre_selection_space()
{
	pre_PT_selection->numOfStation = 0;

	delete [] pre_PT_selection->timeFromA1;
	delete[] pre_PT_selection->timeFromA2;
	delete[] pre_PT_selection->timeFromStart;
	delete[] pre_PT_selection->timeFromEnd;
	delete[] pre_PT_selection->planarPPA_final;
	delete[] pre_PT_selection->PC4;
	delete[] pre_PT_selection->triangle;
	delete[] pre_PT_selection->routePerSta; // route count per station

	delete[] pre_PT_selection->result;
	delete[] pre_PT_selection->label;
}


void delete_allocate_space(int pv0f, int pt0f, int pt1f, int pv1f, int pv1b, int pt1b, int pt0b, int pv0b)
{
	for (int i = 0; i < pv0f; i++)
	{
		delete[] sp_sepresult_pv0_forward->cost[i];
		delete[] sp_sepresult_pv0_forward->searched[i];
		delete[] sp_sepresult_pv0_forward->parent[i];
	}
	delete[] sp_sepresult_pv0_forward->cost;
	delete[] sp_sepresult_pv0_forward->searched;
	delete[] sp_sepresult_pv0_forward->parent;

	for (int i = 0; i < pt0f; i++)
	{
		delete[] sp_sepresult_pt0_forward->trscost[i];
		delete[] sp_sepresult_pt0_forward->parent[i];
		delete[] sp_sepresult_pt0_forward->cost[i];
		delete[] sp_sepresult_pt0_forward->stationcost[i];
		delete[] sp_sepresult_pt0_forward->searched[i];
	}
	delete[] sp_sepresult_pt0_forward->trscost;
	delete[] sp_sepresult_pt0_forward->parent;
	delete[] sp_sepresult_pt0_forward->cost;
	delete[] sp_sepresult_pt0_forward->stationcost;
	delete[] sp_sepresult_pt0_forward->searched;

	/*
	for (int i = 0; i < pt1f; i++)
	{
		delete[] sp_sepresult_pt1_forward->trscost[i];
		delete[] sp_sepresult_pt1_forward->parent[i];
		delete[] sp_sepresult_pt1_forward->cost[i];
		delete[] sp_sepresult_pt1_forward->stationcost[i];
		delete[] sp_sepresult_pt1_forward->searched[i];
	}
	delete[] sp_sepresult_pt1_forward->trscost;
	delete[] sp_sepresult_pt1_forward->parent;
	delete[] sp_sepresult_pt1_forward->cost;
	delete[] sp_sepresult_pt1_forward->stationcost;
	delete[] sp_sepresult_pt1_forward->searched;

	for (int i = 0; i < pv1f; i++)
	{
		delete[] sp_sepresult_pv1_forward->cost[i];
		delete[] sp_sepresult_pv1_forward->searched[i];
		delete[] sp_sepresult_pv1_forward->parent[i];
	}
	delete[] sp_sepresult_pv1_forward->cost;
	delete[] sp_sepresult_pv1_forward->searched;
	delete[] sp_sepresult_pv1_forward->parent;

	for (int i = 0; i < pv1b; i++)
	{
		delete[] sp_sepresult_pv1_backward->cost[i];
		delete[] sp_sepresult_pv1_backward->searched[i];
		delete[] sp_sepresult_pv1_backward->parent[i];
	}
	delete[] sp_sepresult_pv1_backward->cost;
	delete[] sp_sepresult_pv1_backward->searched;
	delete[] sp_sepresult_pv1_backward->parent;

	for (int i = 0; i < pt1b; i++)
	{
		delete[] sp_sepresult_pt1_backward->trscost[i];
		delete[] sp_sepresult_pt1_backward->parent[i];
		delete[] sp_sepresult_pt1_backward->cost[i];
		delete[] sp_sepresult_pt1_backward->stationcost[i];
		delete[] sp_sepresult_pt1_backward->searched[i];
	}
	delete[] sp_sepresult_pt1_backward->trscost;
	delete[] sp_sepresult_pt1_backward->parent;
	delete[] sp_sepresult_pt1_backward->cost;
	delete[] sp_sepresult_pt1_backward->stationcost;
	delete[] sp_sepresult_pt1_backward->searched;
	*/
	for (int i = 0; i < pt0b; i++)
	{
		delete[] sp_sepresult_pt0_backward->trscost[i];
		delete[] sp_sepresult_pt0_backward->parent[i];
		delete[] sp_sepresult_pt0_backward->cost[i];
		delete[] sp_sepresult_pt0_backward->stationcost[i];
		delete[] sp_sepresult_pt0_backward->searched[i];
	}
	delete[] sp_sepresult_pt0_backward->trscost;
	delete[] sp_sepresult_pt0_backward->parent;
	delete[] sp_sepresult_pt0_backward->cost;
	delete[] sp_sepresult_pt0_backward->stationcost;
	delete[] sp_sepresult_pt0_backward->searched;

	for (int i = 0; i < pv0b; i++)
	{
		delete[] sp_sepresult_pv0_backward->cost[i];
		delete[] sp_sepresult_pv0_backward->searched[i];
		delete[] sp_sepresult_pv0_backward->parent[i];
	}
	delete[] sp_sepresult_pv0_backward->cost;
	delete[] sp_sepresult_pv0_backward->searched;
	delete[] sp_sepresult_pv0_backward->parent;
}


int get_A_station_time1(int station, int* endStations, int numofendststions)
{
	int value = INF_DIST;
	for (int i = 0; i < numofendststions; i++)
	{
		int dist = get_A_station_time2(station, endStations[i]);
		if (dist < value)
			value = dist;
	}
	return value / 60; // in min
}

int get_A_station_time2(int station1, int station2)
{
	int x1 = instance_ptstationcoords->stationcoordx[station1];
	int y1 = instance_ptstationcoords->stationcoordy[station1];
	int x2 = instance_ptstationcoords->stationcoordx[station2];
	int y2 = instance_ptstationcoords->stationcoordy[station2];

	return int(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) / (35 * 1000 / 60)); // in min
}

int get_A_tv2_time(int node1, int* nodes, int numofnodes, int pvflag)
{
	int value = INF_DIST;
	for (int i = 0; i < numofnodes; i++)
	{
		int dist = get_A_tvl_time(node1, nodes[i], pvflag);
		if (dist < value)
			value = dist;
	}
	return value/60; // in min
}

int get_A_tvl_time(int node1, int node2, int pvflag)
{
	int i = findI_pv(node1);
	int j = findI_pv(node2);
	int x1 = instance_pvcoords->roadNodecoordx[i];
	int y1 = instance_pvcoords->roadNodecoordy[i];
	int x2 = instance_pvcoords->roadNodecoordx[j];
	int y2 = instance_pvcoords->roadNodecoordy[j];

	int length = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	int speed = getSpeed(pvflag);
	int time = costCal_pre(length, speed);

	return time;
}

int findI_pv(int newIdx)
{
	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		if (instance_pvcoords->roadNodeNewIdx[i] == newIdx)
			return i;
	}
}

void insert_val_to_sp_heap(heap_sch_struct* heap, int source, int topKey, int srcDist, int srcState, int hpSize)
{
	heap->topNode = source;
	heap->topKey = topKey;
	heap->dist[source] = srcDist;
	heap->state[source] = srcState;
	//heap->tempTime[source] = 0;
	heap->size = hpSize;
}

vector<vector<int>> findStartInfos_mo(int station, int startTime, int pastCost, int loopidx2)
{
	vector<vector<int>> result;
	vector<vector<int>> info = findStartInfo(station, startTime, pastCost);
	if (info.size() != 0)
	{
		sp_sepresult_pt1_forward->parent[loopidx2][info[0][0]] = info[0][0];
		sp_sepresult_pt1_forward->cost[loopidx2][info[0][0]] = info[0][1];
		sp_sepresult_pt1_forward->stationcost[loopidx2][info[0][3]] = info[0][1];
		result.insert(result.end(), info.begin(), info.end());
	}
	sort(result.begin(), result.end(), sortFunc1);
	return result;
}

vector<vector<int>> findStartInfos(int station, int startTime, int pastCost, int loopidx2)
{
	vector<vector<int>> result;
    vector<vector<int>> info = findStartInfo(station, startTime, pastCost);
	if (info.size() != 0)
	{

		sp_sepresult_pt0_forward->parent[loopidx2][info[0][0]] = info[0][0];
		sp_sepresult_pt0_forward->cost[loopidx2][info[0][0]] = info[0][1];
		sp_sepresult_pt0_forward->stationcost[loopidx2][info[0][3]] = info[0][1];

		result.insert(result.end(), info.begin(), info.end());
	}
	sort(result.begin(), result.end(), sortFunc1);
	return result;
}

vector<vector<int>> findEndInfos_mo(int endStation, int endTime, int pastCost, int loopidx2)
{
	vector<vector<int>> result;

	vector<vector<int>> info = findEndInfo(endStation, endTime, pastCost);

	if (info.size() != 0)
	{
		sp_sepresult_pt1_backward->parent[loopidx2][info[0][0]] = info[0][0];
		sp_sepresult_pt1_backward->cost[loopidx2][info[0][0]] = info[0][1];
		sp_sepresult_pt1_backward->stationcost[loopidx2][info[0][3]] = info[0][1];
		result.insert(result.end(), info.begin(), info.end());
	}

	sort(result.begin(), result.end(), sortFunc1);
	return result;
}

vector<vector<int>> findEndInfos(int endStation, int endTime, int pastCost, int loopidx2)
{
	vector<vector<int>> result;

	vector<vector<int>> info = findEndInfo(endStation, endTime, pastCost);
	if (info.size() != 0)
	{
		sp_sepresult_pt0_backward->parent[loopidx2][info[0][0]] = info[0][0];
		sp_sepresult_pt0_backward->cost[loopidx2][info[0][0]] = info[0][1];
		sp_sepresult_pt0_backward->stationcost[loopidx2][info[0][3]] = info[0][1];
		result.insert(result.end(), info.begin(), info.end());
	}
	sort(result.begin(), result.end(), sortFunc1);
	return result;
}

vector<vector<int>> findStartInfo(int startStation, int startTime, int pastCost)
{
	vector<vector<int>> temp_arr;


	for (int i = 0; i < instance_spnetpt->edgeNum; i++)
	{
		vector<int> info;
		int station = instance_spnetpt->edgeFistS[i];
		int time = instance_spnetpt->edgeDisUFist[i];
		int flag = instance_spnetpt->Flag[i];
		if (station == startStation && time - startTime >= 0 && (time - startTime) <= (instance_stpap->timeBudget / 10000 - pastCost))
		{
			info.push_back(instance_spnetpt->edgeFist[i]); // start point
			info.push_back(time - startTime + pastCost); // wait time + already cost
			info.push_back(pastCost); // already cost time without wait time
			info.push_back(station); // station
			info.push_back(instance_spnetpt->lineNum[i]);
			info.push_back(instance_spnetpt->routeNum[i]);
			temp_arr.push_back(info);
		}
	}
	sort(temp_arr.begin(), temp_arr.end(), sortFunc1);
	return temp_arr;
}

vector<vector<int>> findEndInfo(int endStation, int endTime, int pastCost)
{
	vector<vector<int>> infos;

	for (int i = 0; i < instance_spnetpt_reverse->edgeNum; i++)
	{
		int station = instance_spnetpt_reverse->edgeFistS[i];
		int temp_time = endTime - instance_spnetpt_reverse->edgeDisUFist[i];
		int flag = instance_spnetpt_reverse->Flag[i];
		if (station == endStation && temp_time >= 0 && temp_time <= instance_stpap->timeBudget/10000 - pastCost)
		{
			vector<int> info;
			info.push_back(instance_spnetpt_reverse->edgeFist[i]);
			info.push_back(temp_time + pastCost);
			//info.push_back(pastCost);
			info.push_back(pastCost);
			info.push_back(station);
			info.push_back(instance_spnetpt_reverse->lineNum[i]);
			info.push_back(instance_spnetpt_reverse->routeNum[i]);
			infos.push_back(info);
		}
	}
	sort(infos.begin(), infos.end(), sortFunc1);
	return infos;
}



vector<vector<int>> findTransferInfo_forward(int startStation, int startTime, int pastCost)
{
	vector<vector<int>> temp_arr;


	for (int i = 0; i < instance_spnetpt->edgeNum; i++)
	{
		vector<int> info;
		int station = instance_spnetpt->edgeFistS[i];
		int time = instance_spnetpt->edgeDisUFist[i];
		int flag = instance_spnetpt->Flag[i];
		if (station == startStation && time - startTime >= 0 && (time - startTime) <= (instance_stpap->timeBudget - pastCost))
		{
			info.push_back(instance_spnetpt->edgeFist[i]); // start point
			info.push_back(0); // wait time + already cost
			info.push_back(0); // already cost time without wait time
			info.push_back(station); // station
			info.push_back(instance_spnetpt->lineNum[i]);
			info.push_back(instance_spnetpt->routeNum[i]);
			temp_arr.push_back(info);
		}
	}
	sort(temp_arr.begin(), temp_arr.end(), sortFunc1);
	return temp_arr;
}


vector<vector<int>> findTransferInfo_backward(int endStation, int endTime, int pastCost)
{
	vector<vector<int>> infos;

	for (int i = 0; i < instance_spnetpt_reverse->edgeNum; i++)
	{
		int station = instance_spnetpt_reverse->edgeFistS[i];
		int temp_time = endTime - instance_spnetpt_reverse->edgeDisUFist[i];
		int flag = instance_spnetpt_reverse->Flag[i];
		if (station == endStation && temp_time >= 0 && temp_time <= instance_stpap->timeBudget - pastCost)
		{
			vector<int> info;
			info.push_back(instance_spnetpt_reverse->edgeFist[i]);
			info.push_back(0);
			info.push_back(0);
			info.push_back(station);
			info.push_back(instance_spnetpt_reverse->lineNum[i]);
			info.push_back(instance_spnetpt_reverse->routeNum[i]);
			infos.push_back(info);
		}
	}
	sort(infos.begin(), infos.end(), sortFunc1);
	return infos;
}

void check4activity(vector<vector<int>> arr, int loopidx, int startStation, int numOfaroundActivity, int startTime)
{
	vector<vector<int>> shops;
	vector<vector<int>> parks;
	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < numOfaroundActivity; j++)
		{
			if (checkShopPPA(j, arr[i][0], arr[i][1], arr[i][2]))
			{
				vector<int> shop;
				int idx = shop_data->index[arr[i][0]][j];
				shop.push_back(idx);
				shop.push_back(shop_data->activitycoordx[idx]);
				shop.push_back(shop_data->activitycoordy[idx]);
				shop.push_back(arr[i][1]);
				shop.push_back(arr[i][2]);
				shops.push_back(shop);
			}
		}
	}

	for (int i = 0; i < arr.size(); i++)
	{
		for (int k = 0; k < numOfaroundActivity; k++)
		{
			if (checkParkPPA(k, arr[i][0], arr[i][1], arr[i][2]))
			{
				vector<int> park;
				int idx = park_data->index[arr[i][0]][k];
				park.push_back(idx);
				park.push_back(park_data->activitycoordx[idx]);
				park.push_back(park_data->activitycoordy[idx]);
				park.push_back(arr[i][1]);
				park.push_back(arr[i][2]);
				parks.push_back(park);
			}
		}
	}

	sort(shops.begin(), shops.end());
	if (shops.size() != 0)
	{
		output_data_shops(shops, loopidx, startStation, startStation, startTime);
	}

	sort(parks.begin(), parks.end());
	if (parks.size() != 0)
	{
		output_data_parks(parks, loopidx, startStation, startStation, startTime);
	}
}