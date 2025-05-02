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

int		INF_DIST4 = 2000000;

heap_sch_struct* sp_PT_heap_F;
heap_sch_struct* sp_PT_heap_B;
heap_sch_struct* sp_PV1_heap_F;
heap_sch_struct* sp_PV1_heap_B;
heap_sch_struct* sp_PT_heap_pre_time;
heap_sch_struct* sp_PV_heap_pre_planar_1;
heap_sch_struct* sp_PV_heap_pre_planar_2;
heap_sch_struct* sp_PV_heap_pre_original;
heap_sch_struct* sp_PV_heap_single_F;
heap_sch_struct* sp_PV_heap_single_B;
heap_sch_struct* sp_PT_heap_pre_transfer;

using namespace std;


int getNumStation(int netflag)
{
	int numOfStation = 0;

	switch (netflag)
	{
	case 0:
		numOfStation = 4; // walk
		break;
	case 1:
		numOfStation = 20; // bike
		break;
	case 2:
		numOfStation = transfer_station_data->staNum; //car
		break;
	case 3:
		numOfStation = transfer_station_data->staNum; //car
		break;
	}

	return numOfStation;
}

int getSpeed(int pvnetflag)
{
	// km/h
	int speed = 0;

	switch (pvnetflag)
	{
	case 0:
		speed = 5;
		break;
	case 1:
		speed = 15;
		break;
	case 2:
		speed = 50;
		break;
	case 3:
		speed = 90;
		break;
	}

	return speed;
}

int getSpeed(int pvflag, int roadflag, int areaflag)
{
	int speed = 0;
	int speedfactor = 1;

	if (areaflag == 1)
	{
		roadflag = 5;
		speedfactor = instance_stpap->densityfactor;
	}
		

	if (pvflag == 0)
		return speed_profile->speedofwalk[roadflag - 1];

	if(pvflag == 1)
		return speed_profile->speedofbike[roadflag - 1];

	if (pvflag == 2)
	{
		return (speed_profile->speedofcar[roadflag - 1] / speedfactor);
	}
		

}

int** getAroundStation(int nodeidx, int pvflag)
{
	int numOfstation = getNumStation(pvflag);
	int** stationNnode = generate_int_matrix(numOfstation, 2); // first station, second node

	if (pvflag == 2 || pvflag == 3)
	{
		for (int i = 0; i < numOfstation; i++)
		{
			stationNnode[i][0] = transfer_station_data->starowidx[i];
			stationNnode[i][1] = instance_pvpt_conn->n2Station[stationNnode[i][0]][0];
		}
	}
	else
	{
		for (int i = 0; i < numOfstation; i++)
		{
			stationNnode[i][0] = instance_pvpt_conn->node2Station[nodeidx][i];
			stationNnode[i][1] = instance_pvpt_conn->station2Node[nodeidx][i];
		}
	}

	return stationNnode;
}

void multimodal_search_checkrunningTime(int home, int destination, int startNode, int startStation, int startTime, int timeBudget, int pvflag1, int loopidx)
{
	int pvnet1 = 0;
	int ptnet1 = 0;
	int parkingtime = 0;
	int pikingtime = 0;


	if (pvflag1 == 2)
		parkingtime = 3;
	if (pvflag1 == 1)
		parkingtime = 1;
	if (pvflag1 == 2 || pvflag1 == 1)
		pikingtime = 1;


	// forward search
	//setPVEdgeDistU_pre(pvflag1);
	allocate_space_result_pv0_forward(1);
	sp_PV_tbs_1st(sp_PV1_heap_F, sp_PV1_heap_B, home, destination, 0, pvnet1, pvflag1);

	int stationTime1 = sp_PV1_heap_F->distRL[startNode] / 10000;
	if (stationTime1 > instance_stpap->timeBudget / 10000)
		return;

	allocate_space_result_pt0_forward(1);
	sp_PT_tbs_1st(sp_PT_heap_F, sp_PT_heap_B, startStation, stationTime1 + startTime + parkingtime, stationTime1, loopidx, 0);

	// backward search
	allocate_space_result_pv0_backward(1);
	sp_PV_tbs_2nd(sp_PV1_heap_F, sp_PV1_heap_B, destination, 0, 0);
	if (sp_PV1_heap_B->distRL[startNode] == INF_DIST4)
		return;

	allocate_space_result_pt0_backward(1);
	int startT = startTime + timeBudget - pikingtime - sp_PV1_heap_B->distRL[startNode]/10000;
	sp_PT_tbs_2nd(sp_PT_heap_F, sp_PT_heap_B, startStation, startT, pikingtime + sp_PV1_heap_B->distRL[startNode] / 10000, loopidx, 0, 0);
	delete_allocate_space(1, 1, 0, 0, 0, 0, 1, 1);
}

void unimodal_search(int home, int destination, int startTime, int pvflag1, int loopidx)
{
	
	setPVEdgeDistU_pre(pvflag1);
	sp_PV_tbs_1st(sp_PV_heap_single_F, sp_PV_heap_single_B, home, destination, 0, 0, pvflag1);
	sp_PV_tbs_2nd(sp_PV_heap_single_F, sp_PV_heap_single_B, destination, 0, 0);
}

vector<vector<int>> multimodal_search_diff(int home, int destination, int startNode, int startStation, int endNode, int endStation, int startTime, int timeBudget, int pvflag1, int loopidx)
{
	int pvnet1 = 0;
	int ptnet1 = 0;
	int parkingtime = 0;
	int pikingtime = 0;


	if (pvflag1 == 2)
		parkingtime = 5;
	if (pvflag1 == 1)
		parkingtime = 2;

	if (pvflag1 == 2)
		pikingtime = 3;
	if (pvflag1 == 1)
		pikingtime = 1;

	setPVEdgeDistU_pre(pvflag1);
	allocate_space_result_pv0_forward(1);
	sp_PV_tbs_1st(sp_PV1_heap_F, sp_PV1_heap_B, home, destination, 0, pvnet1, pvflag1);
	for (int i = 0; i < sp_sepresult_pv0_forward->numOfNode; i++)
	{
		if (sp_PV1_heap_F->distRL[i] != INF_DIST4)
		{
			sp_sepresult_pv0_forward->searched[0][i] = 1;
			sp_sepresult_pv0_forward->parent[0][i] = sp_PV1_heap_F->parent[i];
			sp_sepresult_pv0_forward->cost[0][i] = sp_PV1_heap_F->distRL[i] / 10000;
		}
	}

	int stationTime1 = sp_sepresult_pv0_forward->cost[0][startNode];

	allocate_space_result_pt0_forward(1);
	sp_PT_tbs_1st(sp_PT_heap_F, sp_PT_heap_B, startStation, stationTime1 + startTime + parkingtime, stationTime1 + parkingtime, loopidx, 0);
	vector<vector<int>> stations;
	for (int i = 0; i < sp_sepresult_pt0_forward->numOfSta; i++)
	{
		if (sp_sepresult_pt0_forward->stationcost[ptnet1][i] <= instance_stpap->timeBudget / 10000)
		{
			vector<int> arr;
			arr.push_back(i);
			arr.push_back(sp_sepresult_pt0_forward->stationcost[ptnet1][i]);
			stations.push_back(arr);
		}
	}

	// backward search
	allocate_space_result_pv0_backward(1);
	sp_PV_tbs_2nd(sp_PV1_heap_F, sp_PV1_heap_B, destination, 0, 0);

	for (int j = 0; j < sp_sepresult_pv0_backward->numOfNode; j++)
	{
		if (sp_PV1_heap_B->distRL[j] != INF_DIST4)
		{
			sp_sepresult_pv0_backward->searched[0][j] = 1;
			sp_sepresult_pv0_backward->parent[0][j] = sp_PV1_heap_B->parent[j];
			sp_sepresult_pv0_backward->cost[0][j] = sp_PV1_heap_B->distRL[j] / 10000;
		}
	}

	// difference
	allocate_space_result_pt0_backward(1);
	int startT = startTime + timeBudget - pikingtime - sp_sepresult_pv0_backward->cost[0][endNode];
	sp_PT_tbs_2nd(sp_PT_heap_F, sp_PT_heap_B, endStation, startT, pikingtime + sp_sepresult_pv0_backward->cost[0][endNode], loopidx, 0, 0);
	vector<vector<int>> stations_back;
	for (int i = 0; i < stations.size(); i++)
	{
		int sta = stations[i][0];
		int time1 = stations[i][1];
		int time2 = sp_sepresult_pt0_backward->stationcost[ptnet1][sta];
		if (time1 + time2 <= instance_stpap->timeBudget / 10000)
		{
			vector<int> arr;
			arr.push_back(startStation);
			arr.push_back(endStation);
			arr.push_back(sta);
			arr.push_back(time1 + startTime);
			arr.push_back(startTime + instance_stpap->timeBudget / 10000 - time2);
			
			
			stations_back.push_back(arr);
		}

	}

	//if (stations_back.size() == 0)
		//return;

	//output_final_station(stations_back, loopidx, startStation);
	//output_data_ppa(loopidx, startStation, startTime);

	delete_allocate_space(1, 1, 0, 0, 0, 0, 1, 1);
	return stations_back;

}

vector<vector<int>> multimodal_search(int home, int destination, int startNode, int startStation, int startTime, int timeBudget, int pvflag1, int loopidx)
{
	int pvnet1 = 0;
	int ptnet1 = 0;
	int parkingtime = 0;
	int pikingtime = 0;

	
	if (pvflag1 == 2)
		parkingtime = 5;
	if (pvflag1 == 1)
		parkingtime = 2;

	if (pvflag1 == 2)
		pikingtime = 3;
	if (pvflag1 == 1)
		pikingtime = 1;


	// forward search
	setPVEdgeDistU_pre(pvflag1);
	allocate_space_result_pv0_forward(1);
	sp_PV_tbs_1st(sp_PV1_heap_F, sp_PV1_heap_B, home, destination, 0, pvnet1, pvflag1);
	for (int i = 0; i < sp_sepresult_pv0_forward->numOfNode; i++)
	{
		if (sp_PV1_heap_F->distRL[i] != INF_DIST4)
		{
			sp_sepresult_pv0_forward->searched[0][i] = 1;
			sp_sepresult_pv0_forward->parent[0][i] = sp_PV1_heap_F->parent[i];
			sp_sepresult_pv0_forward->cost[0][i] = sp_PV1_heap_F->distRL[i]/10000;
		}
	}

	int stationTime1 = sp_sepresult_pv0_forward->cost[0][startNode];
	//if (stationTime1 > instance_stpap->timeBudget / 10000)
		//return;

	allocate_space_result_pt0_forward(1);
	sp_PT_tbs_1st(sp_PT_heap_F, sp_PT_heap_B, startStation, stationTime1 + startTime + parkingtime, stationTime1 + parkingtime, loopidx, 0);
	vector<vector<int>> stations;
	for (int i = 0; i < sp_sepresult_pt0_forward->numOfSta; i++)
	{
		if (sp_sepresult_pt0_forward->stationcost[ptnet1][i] <= instance_stpap->timeBudget / 10000)
		{
			vector<int> arr;
			arr.push_back(i);
			arr.push_back(sp_sepresult_pt0_forward->stationcost[ptnet1][i]);
			stations.push_back(arr);
		}
	}
	//if (stations.size() == 0)
		//return;
	
	// backward search
	allocate_space_result_pv0_backward(1);
	sp_PV_tbs_2nd(sp_PV1_heap_F, sp_PV1_heap_B, destination, 0, 0);

	for (int j = 0; j < sp_sepresult_pv0_backward->numOfNode; j++)
	{
		if (sp_PV1_heap_B->distRL[j] != INF_DIST4)
		{
			sp_sepresult_pv0_backward->searched[0][j] = 1;
			sp_sepresult_pv0_backward->parent[0][j] = sp_PV1_heap_B->parent[j];
			sp_sepresult_pv0_backward->cost[0][j] = sp_PV1_heap_B->distRL[j] / 10000;
		}	
	}

	//if (sp_sepresult_pv0_backward->cost[0][startNode] == INF_DIST4)
		//return;
	
	allocate_space_result_pt0_backward(1);
	int startT = startTime + timeBudget - pikingtime - sp_sepresult_pv0_backward->cost[0][startNode];
	sp_PT_tbs_2nd(sp_PT_heap_F, sp_PT_heap_B, startStation, startT, pikingtime + sp_sepresult_pv0_backward->cost[0][startNode], loopidx, 0, 0);
	vector<vector<int>> stations_back;	
	for (int i = 0; i < stations.size(); i++)
	{
		int sta = stations[i][0];
		int time1 = stations[i][1];
		int time2 = sp_sepresult_pt0_backward->stationcost[ptnet1][sta];
		if (time1 + time2 <= instance_stpap->timeBudget / 10000)
		{
			vector<int> arr;
			arr.push_back(startStation);
			arr.push_back(sta);
			arr.push_back(time1 + startTime);
			arr.push_back(startTime + instance_stpap->timeBudget/10000 - time2);
			stations_back.push_back(arr);
		}

	}

	//if (stations_back.size() == 0)
		//return;

	//output_final_station(stations_back, loopidx, startStation);
	//output_data_ppa(loopidx, startStation, startTime);

	delete_allocate_space(1, 1, 0, 0, 0, 0, 1, 1);
	return stations_back;
}

void sp_PT_tbs_1st_mo(heap_sch_struct* heap_1, heap_sch_struct* heap_2, int startStation, int arriveTime, int pastCost, int netidx, int loopidx1, int loopidx2)
{
	// from multiple stations to one stations, only the store area changes
	for (int i = 0; i < instance_spnetpt->nodeNum; i++)
	{
		heap_1->dist[i] = INF_DIST4;
		heap_1->distRL[i] = INF_DIST4;
		heap_2->dist[i] = INF_DIST4;
		heap_1->state[i] = 0;
		heap_2->state[i] = 0;
	}
	memset(heap_1->state, 0, (instance_spnetpt->nodeNum) * sizeof(char));
	memset(heap_2->state, 0, (instance_spnetpt->nodeNum) * sizeof(char));
	
	sp_sepresult_pt1_forward->stationcost[loopidx2][startStation] = pastCost;
	sp_sepresult_pt1_forward->searched[loopidx2][startStation] = 1;

	vector<vector<int>> startinfo = findStartInfos_mo(startStation, arriveTime, pastCost, loopidx2);
	if (startinfo.size() == 0)
		return;
	insert_val_to_sp_heap(heap_1, startinfo[0][0], startinfo[0][1], startinfo[0][1], 2, 0);
	heap_1->distRL[startinfo[0][0]] = startinfo[0][1];
	for (int i = 1; i < startinfo.size(); i++)
	{
		InsertElement(startinfo[i][1], startinfo[i][0], heap_1);
	}

	while (heap_1->size >= 0)
	{
		scan_node_PT_1st_mo(heap_1, loopidx2);

	}
}

void scan_node_PT_1st_mo(heap_sch_struct* heap, int loopidx2)
{
	int	tmp_node;
	int inttmp;
	int tmp_distSV;
	int station;
	int parentstation;
	int transferTimes;

	for (int i = instance_spnetpt->edgeIndex[heap->topNode][0]; i <= instance_spnetpt->edgeIndex[heap->topNode][1]; i++)
	{
		if (i != -1)
		{
			tmp_node = instance_spnetpt->edgeList[i]; // next node
			station = instance_spnetpt->edgeListS[i];
			parentstation = instance_spnetpt->edgeFistS[i];
			inttmp = heap->distRL[heap->topNode] + instance_spnetpt->edgeDisU[i];
			tmp_distSV = inttmp;
			transferTimes = sp_sepresult_pt1_forward->trscost[loopidx2][heap->topNode] + instance_spnetpt->edgeDisUTrs[i];

			if (tmp_distSV <= instance_stpap->timeBudget / 10000 && tmp_distSV < heap->dist[tmp_node] && transferTimes <= instance_stpap->maxTrasferTime)
			{
				heap->dist[tmp_node] = tmp_distSV;
				heap->distRL[tmp_node] = inttmp;
				sp_sepresult_pt1_forward->searched[loopidx2][station] = 1;
				sp_sepresult_pt1_forward->parent[loopidx2][tmp_node] = heap->topNode;
				sp_sepresult_pt1_forward->cost[loopidx2][tmp_node] = inttmp;
				sp_sepresult_pt1_forward->trscost[loopidx2][tmp_node] = transferTimes;

				if (inttmp < sp_sepresult_pt1_forward->stationcost[loopidx2][station])
				{
					sp_sepresult_pt1_forward->stationcost[loopidx2][station] = inttmp;
				}

				if (heap->state[tmp_node] == 0)
					InsertElement(inttmp, tmp_node, heap);
				else
					DecreaseKey(inttmp, tmp_node, heap);
			}
		}
	}
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}

void sp_PT_tbs_2nd_mo(heap_sch_struct* heap_2, int endstation, int arriveTime, int pastCost, int netidx, int loopidx1, int loopidx2, int loopidx3)
{
	// loopidx3 to find forward data in stations
	for (int i = 0; i < instance_spnetpt->nodeNum; i++)
	{
		heap_2->distRL[i] = INF_DIST4;
		heap_2->dist[i] = INF_DIST4;
		heap_2->state[i] = 0;
	}
	sp_sepresult_pt1_backward->stationcost[loopidx2][endstation] = pastCost;
	sp_sepresult_pt1_backward->searched[loopidx2][endstation] = 1;

	vector<vector<int>> endinfos = findEndInfos_mo(endstation, arriveTime, pastCost, loopidx2);
	if (endinfos.size() == 0)
		return;
	insert_val_to_sp_heap(heap_2, endinfos[0][0], endinfos[0][1], endinfos[0][1], 2, 0);
	heap_2->distRL[endinfos[0][0]] = endinfos[0][1];

	for (int i = 1; i < endinfos.size(); i++)
	{
		InsertElement(endinfos[i][1], endinfos[i][0], heap_2);
	}

	while (heap_2->size >= 0)
	{
		scan_node_PT_2nd_mo(heap_2, loopidx2, loopidx3);
	}
}

void scan_node_PT_2nd_mo(heap_sch_struct* heap, int loopidx2, int loopidx3)
{
	int	tmp_node;
	int inttmp;
	int station;
	int parentstation;
	int transferTimes;


	for (int i = instance_spnetpt_reverse->edgeIndex[heap->topNode][0]; i <= instance_spnetpt_reverse->edgeIndex[heap->topNode][1]; i++)
	{
		if (i != -1)
		{
			tmp_node = instance_spnetpt_reverse->edgeList[i]; // next node
			station = instance_spnetpt_reverse->edgeListS[i];
			parentstation = instance_spnetpt_reverse->edgeFistS[i];

			if(sp_sepresult_pt1_forward->searched[loopidx3][station] == 1 && sp_sepresult_pt1_forward->stationcost[loopidx3][station] + heap->topKey <= instance_stpap->timeBudget / 10000 && instance_spnetpt_reverse->edgeDisUFist[i] <= instance_stpap->timeBudget / 10000 + instance_stpap->startTime)
			{
				inttmp = heap->topKey + instance_spnetpt_reverse->edgeDisU[i];
				transferTimes = sp_sepresult_pt1_backward->trscost[loopidx2][heap->topNode] + instance_spnetpt_reverse->edgeDisUTrs[i];

				if (inttmp <= instance_stpap->timeBudget / 10000 && inttmp < heap->distRL[tmp_node] && transferTimes <= instance_stpap->maxTrasferTime)
				{
					heap->distRL[tmp_node] = inttmp;
					sp_sepresult_pt1_backward->searched[loopidx2][station] = 1;
					sp_sepresult_pt1_backward->parent[loopidx2][tmp_node] = heap->topNode;
					sp_sepresult_pt1_backward->cost[loopidx2][tmp_node] = inttmp;
					sp_sepresult_pt1_backward->trscost[loopidx2][tmp_node] = transferTimes;

					if (inttmp < sp_sepresult_pt1_backward->stationcost[loopidx2][station])
					{
						sp_sepresult_pt1_backward->stationcost[loopidx2][station] = inttmp;
					}


					if (heap->state[tmp_node] == 0)
						InsertElement(inttmp, tmp_node, heap);
					else
						DecreaseKey(inttmp, tmp_node, heap);
				}
			}
		}
	}
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}

void sp_PT_tbs_1st(heap_sch_struct* heap_1, heap_sch_struct* heap_2, int startStation, int arriveTime, int pastCost, int loopidx1, int loopidx2)
{
	for (int i = 0; i < instance_spnetpt->nodeNum; i++)
	{
		heap_1->dist[i] = INF_DIST4;
		heap_1->distRL[i] = INF_DIST4;
		heap_2->dist[i] = INF_DIST4;
		heap_1->state[i] = 0;
		heap_2->state[i] = 0;
	}

	sp_sepresult_pt0_forward->stationcost[loopidx2][startStation] = pastCost;
	sp_sepresult_pt0_forward->searched[loopidx2][startStation] = 1;

	memset(heap_1->state, 0, (instance_spnetpt->nodeNum) * sizeof(char));
	memset(heap_2->state, 0, (instance_spnetpt->nodeNum) * sizeof(char));

	vector<vector<int>> startinfo = findStartInfos(startStation, arriveTime, pastCost, loopidx2);
	if (startinfo.size() == 0)
		return;
	insert_val_to_sp_heap(heap_1, startinfo[0][0], startinfo[0][1], startinfo[0][1], 2, 0);
	heap_1->distRL[startinfo[0][0]] = startinfo[0][1];
	for (int i = 1; i < startinfo.size(); i++)
	{
		InsertElement(startinfo[i][1], startinfo[i][0], heap_1);
	}

	while (heap_1->size >= 0)
	{
		scan_node_PT_1st(heap_1, loopidx2);

	}

}

void scan_node_PT_1st(heap_sch_struct* heap, int loopidx2)
{
	int	tmp_node;
	int inttmp;
	int tmp_distSV;
	int station;
	int parentstation;
	int transferTimes;

	for (int i = instance_spnetpt->edgeIndex[heap->topNode][0]; i <= instance_spnetpt->edgeIndex[heap->topNode][1]; i++)
	{
		if (i != -1)
		{
			// cout << "i is " << i << endl;

			tmp_node = instance_spnetpt->edgeList[i]; // next node
			station = instance_spnetpt->edgeListS[i];
			parentstation = instance_spnetpt->edgeFistS[i];
			inttmp = heap->distRL[heap->topNode] + instance_spnetpt->edgeDisU[i];
			tmp_distSV = inttmp;
			transferTimes = sp_sepresult_pt0_forward->trscost[loopidx2][heap->topNode] + instance_spnetpt->edgeDisUTrs[i];

			if (tmp_distSV <= instance_stpap->timeBudget/10000 && tmp_distSV < heap->dist[tmp_node] && transferTimes <= instance_stpap->maxTrasferTime)
			{
				heap->dist[tmp_node] = tmp_distSV;
				heap->distRL[tmp_node] = inttmp;
				sp_sepresult_pt0_forward->searched[loopidx2][station] = 1;
				sp_sepresult_pt0_forward->parent[loopidx2][tmp_node] = heap->topNode;
				sp_sepresult_pt0_forward->cost[loopidx2][tmp_node] = inttmp;
				sp_sepresult_pt0_forward->trscost[loopidx2][tmp_node] = transferTimes;

				if (inttmp < sp_sepresult_pt0_forward->stationcost[loopidx2][station])
				{
					sp_sepresult_pt0_forward->stationcost[loopidx2][station] = inttmp;
				}

				if (heap->state[tmp_node] == 0)
					InsertElement(inttmp, tmp_node, heap);
				else
					DecreaseKey(inttmp, tmp_node, heap);
			}
		}
	}
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}



void sp_PT_tbs_2nd(heap_sch_struct* heap_1, heap_sch_struct* heap_2, int endstation, int arriveTime, int pastCost, int loopidx1, int loopidx2, int loopidx3)
{
	for (int i = 0; i < instance_spnetpt->nodeNum; i++)
	{
		heap_2->distRL[i] = INF_DIST4;
		heap_2->dist[i] = INF_DIST4;
		heap_2->state[i] = 0;
	}
	sp_sepresult_pt0_backward->stationcost[loopidx2][endstation] = pastCost;
	sp_sepresult_pt0_backward->searched[loopidx2][endstation] = 1;

	vector<vector<int>> endinfos = findEndInfos(endstation, arriveTime, pastCost, loopidx2);
	if (endinfos.size() == 0)
		return;
	insert_val_to_sp_heap(heap_2, endinfos[0][0], endinfos[0][1], endinfos[0][1], 2, 0);
	heap_2->distRL[endinfos[0][0]] = endinfos[0][1];
	for (int i = 1; i < endinfos.size(); i++)
	{
		InsertElement(endinfos[i][1], endinfos[i][0], heap_2);
	}


	while (heap_2->size >= 0)
	{
		scan_node_PT_2nd(heap_2, loopidx2, loopidx3);
	}

}

void scan_node_PT_2nd(heap_sch_struct* heap, int loopidx2, int loopidx3)
{
	int	tmp_node;
	int inttmp;
	int station;
	int parentstation;
	int transferTimes;


	for (int i = instance_spnetpt_reverse->edgeIndex[heap->topNode][0]; i <= instance_spnetpt_reverse->edgeIndex[heap->topNode][1]; i++)
	{
		if (i != -1)
		{
			tmp_node = instance_spnetpt_reverse->edgeList[i]; // next node
			station = instance_spnetpt_reverse->edgeListS[i];
			parentstation = instance_spnetpt_reverse->edgeFistS[i];
			// if(sp_sepresult_pt0_forward->searched[loopidx3][station] == 1 && sp_sepresult_pt0_forward->stationcost[loopidx3][station] + heap->topKey <= instance_stpap->timeBudget / 10000 && instance_spnetpt_reverse->edgeDisUFist[i] <= instance_stpap->timeBudget / 10000 + instance_stpap->startTime)
			if(sp_sepresult_pt0_forward->searched[loopidx3][station] == 1 && sp_sepresult_pt0_forward->stationcost[loopidx3][station] + heap->topKey <= instance_stpap->timeBudget / 10000 && instance_spnetpt_reverse->edgeDisUFist[i] <= instance_stpap->timeBudget / 10000 + instance_stpap->startTime)
			{
				inttmp = heap->topKey + instance_spnetpt_reverse->edgeDisU[i];
				transferTimes = sp_sepresult_pt0_backward->trscost[loopidx2][heap->topNode] + instance_spnetpt_reverse->edgeDisUTrs[i];

				if (inttmp <= instance_stpap->timeBudget / 10000 && inttmp < heap->dist[tmp_node] && transferTimes <= instance_stpap->maxTrasferTime)
				{
					heap->dist[tmp_node] = inttmp;
					sp_sepresult_pt0_backward->searched[loopidx2][station] = 1;
					sp_sepresult_pt0_backward->parent[loopidx2][tmp_node] = heap->topNode;
					sp_sepresult_pt0_backward->cost[loopidx2][tmp_node] = inttmp;
					sp_sepresult_pt0_backward->trscost[loopidx2][tmp_node] = transferTimes;

					if (inttmp < sp_sepresult_pt0_backward->stationcost[loopidx2][station])
					{
						sp_sepresult_pt0_backward->stationcost[loopidx2][station] = inttmp;
					}


					if (heap->state[tmp_node] == 0)
						InsertElement(inttmp, tmp_node, heap);
					else
						DecreaseKey(inttmp, tmp_node, heap);
				}
			}
		}
	}
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}



void sp_PV_tbs_1st(heap_sch_struct* heap1, heap_sch_struct* heap2, int startNode, int endNode, int pastCost, int netidx, int pvflag)
{
	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		heap1->dist[i] = INF_DIST4;
		heap1->distRL[i] = INF_DIST4;
		heap2->dist[i] = INF_DIST4;
		heap2->distRL[i] = INF_DIST4;
		heap1->state[i] = 0;
		heap2->state[i] = 0;
	}

	memset(heap1->state, 0, (instance_spnetpv->nodeNum) * sizeof(char));
	memset(heap2->state, 0, (instance_spnetpv->nodeNum) * sizeof(char));

	pastCost = pastCost * 10000;
	int inttmp = get_A_tvl_time(startNode, endNode, pvflag);
	heap1->distRL[startNode] = pastCost;
	insert_val_to_sp_heap(heap1, startNode, inttmp + pastCost, inttmp + pastCost, 2, 0);
	while (heap1->size >= 0)
	{
		scan_node_PV_1st(heap1, netidx, endNode, pvflag);
	}

}



void scan_node_PV_1st(heap_sch_struct* heap, int netidx, int endNode, int pvflag)
{
	int		tmp_node, tmp_distSV, inttmp;

	for (int i = instance_spnetpv->edgeIndex[heap->topNode][0]; i <= instance_spnetpv->edgeIndex[heap->topNode][1]; i++)
	{
		tmp_node = instance_spnetpv->edgeList[i];
		inttmp = heap->distRL[heap->topNode] + instance_spnetpv->edgeDisU[i];
		tmp_distSV = inttmp + get_A_tvl_time(tmp_node, endNode, pvflag);
		if (tmp_distSV <= instance_stpap->timeBudget && tmp_distSV < heap->dist[tmp_node])
		{
			heap->dist[tmp_node] = tmp_distSV; // estimated travel time
			if (inttmp < heap->distRL[tmp_node])
			{
				heap->distRL[tmp_node] = inttmp; // real travel time
				heap->parent[tmp_node] = heap->topNode;
			}
				
			if (heap->state[tmp_node] == 0)
				InsertElement(tmp_distSV, tmp_node, heap);
			else
				DecreaseKey(tmp_distSV, tmp_node, heap);
		}
	}
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}
void sp_PV_tbs_2nd_final(heap_sch_struct* heap1, heap_sch_struct* heap2, int startNode, int pastCost, int loopidx1, int loopidx2, int loopidx3)
{
	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		heap1->dist[i] = INF_DIST4;
		heap1->distRL[i] = INF_DIST4;
		heap2->dist[i] = INF_DIST4;
		heap2->distRL[i] = INF_DIST4;
		heap1->state[i] = 0;
		heap2->state[i] = 0;
	}

	pastCost = pastCost * 10000;
	insert_val_to_sp_heap(heap2, startNode, pastCost, pastCost, 2, 0);
	heap2->distRL[startNode] = pastCost;
	heap2->parent[startNode] = startNode;
	while (heap2->size >= 0)
	{
		scan_node_PV_2nd_final(heap2, loopidx2, loopidx3);
	}
}

void scan_node_PV_2nd_final(heap_sch_struct* heap, int loopidx2, int loopidx3)
{
	int		tmp_node, tmp_distSV;
	for (int i = instance_spnetpv->edgeIndex[heap->topNode][0]; i <= instance_spnetpv->edgeIndex[heap->topNode][1]; i++)
	{
		tmp_node = instance_spnetpv->edgeList[i];

		if (sp_sepresult_pv0_forward->cost[loopidx3][heap->topNode] * 10000 + heap->topKey <= instance_stpap->timeBudget && sp_sepresult_pv0_forward->searched[0][tmp_node] == 1)
		{

			tmp_distSV = heap->topKey + instance_spnetpv->edgeDisU[i];
			if (tmp_distSV <= heap->distRL[tmp_node])
			{
				heap->distRL[tmp_node] = tmp_distSV;
				heap->parent[tmp_node] = heap->topNode;

				if (heap->state[tmp_node] == 0)
					InsertElement(tmp_distSV, tmp_node, heap);
				else
					DecreaseKey(tmp_distSV, tmp_node, heap);
			}
		}
	}

	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}

void sp_PV_tbs_2nd(heap_sch_struct* heap1, heap_sch_struct* heap2, int startNode, int pastCost, int loopidx2)
{
	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		heap2->dist[i] = INF_DIST4;
		heap2->distRL[i] = INF_DIST4;
		heap2->state[i] = 0;
	}

	pastCost = pastCost * 10000;
	insert_val_to_sp_heap(heap2, startNode, pastCost, pastCost, 2, 0);
	heap2->distRL[startNode] = pastCost;
	heap2->parent[startNode] = startNode;
	while (heap2->size >= 0)
	{
		scan_node_PV_2nd(heap2, heap1, loopidx2);
	}
}

void scan_node_PV_2nd(heap_sch_struct* heap, heap_sch_struct* heap_opp, int loopidx2)
{
	int		tmp_node, tmp_distSV;
	for (int i = instance_spnetpv->edgeIndex[heap->topNode][0]; i <= instance_spnetpv->edgeIndex[heap->topNode][1]; i++)
	{
		tmp_node = instance_spnetpv->edgeList[i];

		if (heap_opp->distRL[heap->topNode] + heap->topKey <= instance_stpap->timeBudget)
		{
			
			tmp_distSV = heap->topKey + instance_spnetpv->edgeDisU[i];
			if (tmp_distSV <= heap->distRL[tmp_node])
			{
				heap->distRL[tmp_node] = tmp_distSV;
				heap->parent[tmp_node] = heap->topNode;

				if (heap->state[tmp_node] == 0)
					InsertElement(tmp_distSV, tmp_node, heap);
				else
					DecreaseKey(tmp_distSV, tmp_node, heap);
			}
		}
	}

	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}

void sp_PV_one2all(heap_sch_struct* heap, int startNode)
{
	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		heap->dist[i] = INF_DIST4;
		heap->distRL[i] = INF_DIST4;
		heap->state[i] = 0;
	}

	memset(heap->state, 0, (instance_spnetpv->nodeNum) * sizeof(char));

	heap->distRL[startNode] = 0;
	insert_val_to_sp_heap(heap, startNode, 0, 0, 2, 0);
	while (heap->size >= 0)
	{
		scan_node_PV_one2all(heap);
	}
}

void scan_node_PV_one2all(heap_sch_struct* heap)
{
	int		tmp_node, tmp_distSV, inttmp;

	for (int i = instance_spnetpv->edgeIndex[heap->topNode][0]; i <= instance_spnetpv->edgeIndex[heap->topNode][1]; i++)
	{
		tmp_node = instance_spnetpv->edgeList[i];
		inttmp = heap->distRL[heap->topNode] + instance_spnetpv->edgeDisU[i];
		tmp_distSV = inttmp;

		if (tmp_distSV <= instance_stpap->timeBudget && tmp_distSV < heap->dist[tmp_node])
		{
			heap->dist[tmp_node] = tmp_distSV; // estimated travel time
			if (inttmp < heap->distRL[tmp_node])
			{
				heap->distRL[tmp_node] = inttmp; // real travel time
				heap->parent[tmp_node] = heap->topNode;
			}

			if (heap->state[tmp_node] == 0)
				InsertElement(tmp_distSV, tmp_node, heap);
			else
				DecreaseKey(tmp_distSV, tmp_node, heap);
		}
	}
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}