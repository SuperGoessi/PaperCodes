#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <memory.h>
#include <algorithm>
#include <assert.h>
#include <vector>
#include <iostream>

#include "inout.h"
#include "checkFunc.h"
#include "timeexpanded.h"
#include "search.h"
#include "binaryheap.h"
#include "netbuild.h"
#include "test.h"
#include "sepsearch.h"
#include "deleteStation.h"

int		INF_DIST5 = 2000000;

using namespace std;

void init_space_stationSlection()
{
	pre_PT_selection->numOfStation = instance_ptstationcoords->stationNum;

	pre_PT_selection->timeFromA1 = generate_int_arr(pre_PT_selection->numOfStation);
	pre_PT_selection->timeFromA2 = generate_int_arr(pre_PT_selection->numOfStation);
	pre_PT_selection->timeFromStart = generate_int_arr(pre_PT_selection->numOfStation);
	pre_PT_selection->timeFromEnd = generate_int_arr(pre_PT_selection->numOfStation);
	pre_PT_selection->planarPPA_final = generate_int_arr(pre_PT_selection->numOfStation);
	pre_PT_selection->PC4 = generate_int_arr(pre_PT_selection->numOfStation);
	pre_PT_selection->triangle = generate_int_arr(pre_PT_selection->numOfStation);
	pre_PT_selection->routePerSta = generate_int_arr(pre_PT_selection->numOfStation); // route count per station

	pre_PT_selection->result = generate_int_arr(pre_PT_selection->numOfStation);
	pre_PT_selection->label = generate_int_arr(pre_PT_selection->numOfStation);

	for (int i = 0; i < pre_PT_selection->numOfStation; i++)
	{
		pre_PT_selection->timeFromA1[i] = INF_DIST5;
		pre_PT_selection->timeFromA2[i] = INF_DIST5;
		pre_PT_selection->timeFromStart[i] = INF_DIST5;
		pre_PT_selection->timeFromEnd[i] = INF_DIST5;
		pre_PT_selection->planarPPA_final[i] = 0;
		pre_PT_selection->PC4[i] = 0;
		pre_PT_selection->triangle[i] = 0;
		pre_PT_selection->result[i] = 0;
		pre_PT_selection->routePerSta[i] = 0;
		pre_PT_selection->label[i] = 0;
	}
}

// step 1
void sp_stp_1_planar(heap_sch_struct* heap_1, heap_sch_struct* heap_2, int pvidx, int psersonidx) //0-walk, 1-bike, 2-car
{
	//find_planar_space(sp_PV_heap_pre_original, pvidx);
	sp_sToALL_planar(instance_stpap->startNode, heap_1);
	sp_sToALL_planar(instance_stpap->endNode, heap_2);

	int count = 0;
	vector<vector<int>> outputs_pv;
	
	for (int i = 0; i< instance_pvcoords->roadNodeNum; i++)
	{

		int tmp1 = heap_1->dist[i];
		int tmp2 = heap_2->dist[i];

		if (tmp1 + tmp2 <= instance_stpap->timeBudget - instance_stpap->approxActivityTime)
		{
			vector<int> output;
			output.push_back(instance_pvcoords->roadNodeOldIdx[i]);
			output.push_back(instance_pvcoords->roadNodecoordx[i]);
			output.push_back(instance_pvcoords->roadNodecoordy[i]);
			outputs_pv.push_back(output);

			int staidx = findStation(i);
			if (staidx != INF_DIST5)
			{
				pre_PT_selection->planarPPA_final[staidx] = 1;
				pre_PT_selection->timeFromA1[staidx] = tmp1;
				pre_PT_selection->timeFromA2[staidx] = tmp2;
				count++;
			}

		}
	}

	output_data_pre_step1_pv(outputs_pv, psersonidx, pvidx);

	vector<vector<int>> outputs;
	for (int i=0; i< instance_ptstationcoords->stationNum; i++)
	{
		if (pre_PT_selection->planarPPA_final[i] == 1)
		{
			vector<int> output;
			output.push_back(i);
			output.push_back(instance_ptstationcoords->stationcoordx[i]);
			output.push_back(instance_ptstationcoords->stationcoordy[i]);
			outputs.push_back(output);
		}
	}
	output_data_pre_step1_pt(outputs, psersonidx, pvidx);

	resetHeap(instance_pvcoords->roadNodeNum, sp_PV_heap_pre_planar_1, 1);
	resetHeap(instance_pvcoords->roadNodeNum, sp_PV_heap_pre_planar_2, 1);
	//cout << "step 1:" << count << " station left" << endl;
}

void sp_sToALL_planar(int source, heap_sch_struct* heap)
{
	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		heap->dist[i] = INF_DIST5;
		heap->state[i] = 0;
	}
		
	memset(heap->state, 0, (instance_pvcoords->roadNodeNum) * sizeof(char));

	insert_val_to_sp_heap(heap, source, 0, 0, 2, 0);

	while (heap->size >= 0)
	{
		scan_node_planar(heap);
	}
	/*
	vector<vector<int>> result;
	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		if (heap->dist[i] == INF_DIST5)
		{
			vector<int> arr;
			arr.push_back(instance_pvcoords->roadNodeOldIdx[i]);
			arr.push_back(instance_pvcoords->roadNodecoordx[i]);
			arr.push_back(instance_pvcoords->roadNodecoordy[i]);
			result.push_back(arr);
		}
	}

	output_vector(result);*/

}

void scan_node_planar(heap_sch_struct* heap)
{
	int		tmp_node, tmp_distSV, stationidx;

	
	for (int i = instance_spnetpv->edgeIndex[heap->topNode][0]; i <= instance_spnetpv->edgeIndex[heap->topNode][1]; i++)
	{
		tmp_node = instance_spnetpv->edgeList[i];

		tmp_distSV = heap->topKey + instance_spnetpv->edgeDisU[i];

		if (tmp_distSV < heap->dist[tmp_node])
		{

			heap->dist[tmp_node] = tmp_distSV;
			if (heap->state[tmp_node] == 0)
				InsertElement(tmp_distSV, tmp_node, heap);
			else
				DecreaseKey(tmp_distSV, tmp_node, heap);
		}
	}
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}

int findStation(int nodeidx)
{
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		int roadnodeidx = instance_pvpt_conn->n2Station[i][0];
		if (roadnodeidx == nodeidx)
			return i;
	}
	return INF_DIST5;
}

void find_planar_space(heap_sch_struct* heap, int pvidx)
{
	int i, tmp1, tmp2;
	int newidx;

	heap->schCnt = 0;
	memset(heap->schFlag, 0, (instance_pvcoords->roadNodeNum) * sizeof(char));

	for (i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{

		tmp1 = get_time(i, instance_stpap->startNode, pvidx) * 10000;
		tmp2 = get_time(i, instance_stpap->endNode, pvidx) * 10000;

		if (tmp1 + tmp2 <= instance_stpap->timeBudget - instance_stpap->approxActivityTime)
		{
			heap->schFlag[i] = 1;
			heap->schCnt++;
				
		}
	}
	//cout << "cnt:" << heap->schCnt << endl;
}

int get_time(int p1, int p2, int pvidx)
{
	int p1_x = instance_pvcoords->roadNodecoordx[p1];
	int p1_y = instance_pvcoords->roadNodecoordy[p1];
	int p2_x = instance_pvcoords->roadNodecoordx[p2];
	int p2_y = instance_pvcoords->roadNodecoordy[p2];

	int v = get_velocity(pvidx) * 1000 / 60;

	return  int(sqrt((p1_x - p2_x) * (p1_x - p2_x) + (p1_y - p2_y) * (p1_y - p2_y))/ v);
}

int get_velocity(int pvidx)
{
	int v = 0;

	switch (pvidx)
	{
	case 0:
		v = 5; // walk
		break;
	case 1:
		v = 15; // bike
		break;
	case 2:
		v = 20; //car
		break;
	}

	return v;
}

// step 2
vector<int> interSelection(int pvflag, int psersonidx)
{
	vector<int> stations;
	if (pvflag == 1)
	{
		for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
		{
			if (pre_PT_selection->planarPPA_final[i] == 1 && instance_ptstationcoords->isPC4[i] == 1)
				stations.push_back(i);
			//if (pre_PT_selection->planarPPA_final[i] == 1 && instance_ptstationcoords->isHub[i] == 1 && instance_ptstationcoords->isPC4[i] == 1)
				//stations.push_back(i);
		}
	}

	if (pvflag == 2)
	{
		for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
		{
			if (pre_PT_selection->planarPPA_final[i] == 1 && instance_ptstationcoords->isHub[i] == 1 && instance_ptstationcoords->isTAZ[i] == 1)
				stations.push_back(i);
		}
	}

	for (int i = 0; i < stations.size(); i++)
	{
		pre_PT_selection->result[stations[i]] = 1;
	}

	// 1.5 km comparison
	int lengthRange = 0;
	if (pvflag == 1)
		lengthRange = 500;
	if (pvflag == 2)
		lengthRange = 5000;
	vector<vector<int>> distcom;
	for (int i : stations)
	{
		for (int j : stations)
		{
			if (i == j)
				continue;
			int dist = get_stop_euDist(i, j);
			if (dist <= lengthRange && pre_PT_selection->result[j] == 1 && pre_PT_selection->result[i] == 1)
			{
				int routecounti = pre_PT_selection->routePerSta[i];
				int routecountj = pre_PT_selection->routePerSta[j];

				if (routecounti > routecountj && pre_PT_selection->label[j] != 1)
				{
					pre_PT_selection->result[j] = 0;
					pre_PT_selection->label[i] = 1;
				}

				vector<int> arr;
				arr.push_back(i);
				arr.push_back(j);
				arr.push_back(instance_ptstationcoords->stationcoordx[i]);
				arr.push_back(instance_ptstationcoords->stationcoordy[i]);
				arr.push_back(instance_ptstationcoords->stationcoordx[j]);
				arr.push_back(instance_ptstationcoords->stationcoordy[j]);
				arr.push_back(pre_PT_selection->routePerSta[i]);
				arr.push_back(pre_PT_selection->routePerSta[j]);
				distcom.push_back(arr);
			}
		}
	}
	//output_data_pre_step2_distcompare(distcom, psersonidx);

	vector<int> result;
	for (int i = 0; i < pre_PT_selection->numOfStation; i++)
	{
		if (pre_PT_selection->result[i] == 1)
		{
			result.push_back(i);
		}
	}

	vector<vector<int>> outputs;
	for (int i : result)
	{
		vector<int> output;
		output.push_back(i);
		output.push_back(instance_ptstationcoords->stationcoordx[i]);
		output.push_back(instance_ptstationcoords->stationcoordy[i]);
		outputs.push_back(output);
	}

	output_data_pre_step2(outputs, psersonidx, pvflag);
	
	//cout << "step 2: " << result.size() << " station left" << endl;

	return result;
}

// step 3
vector<vector<int>> computeAverageTime()
{
	int lastStartStation = 0;
	int lastEndStation = 0;
	int totaltime = 0;
	int count = 0;
	int currentStartStation = 0;
	int currentEndStation = 0;
	int currenttime = 0;

	// sort data
	vector<vector<int>> arr;
	for (int i = 0; i < instance_ptroutedata->stationRecord; i++)
	{
		vector<int> temp;
		temp.push_back(instance_ptroutedata->routesData[i][0]);
		temp.push_back(instance_ptroutedata->routesData[i][1]);
		temp.push_back((instance_ptroutedata->routesData[i][3] - instance_ptroutedata->routesData[i][2]));
		arr.push_back(temp);
	}

	vector<vector<int>> records;

	sort(arr.begin(), arr.end(), sortFunc_01);
	lastStartStation = arr[0][0];
	lastEndStation = arr[0][1];
	totaltime += arr[0][2];
	count = 1;

	for (int i = 1; i < instance_ptroutedata->stationRecord; i++)
	{
		currentStartStation = arr[i][0];
		currentEndStation = arr[i][1];
		currenttime = arr[i][2];

		if (lastStartStation == currentStartStation && lastEndStation == currentEndStation)
		{
			lastStartStation = currentStartStation;
			lastEndStation = currentEndStation;
			totaltime += currenttime;
			count += 1;

			if (i == instance_ptroutedata->stationRecord - 1)
			{
				vector<int> record;
				record.push_back(currentStartStation);
				record.push_back(currentEndStation);
				record.push_back(round(totaltime / count));
				records.push_back(record);
			}
		}
		else
		{
			vector<int> record;
			record.push_back(lastStartStation);
			record.push_back(lastEndStation);
			record.push_back(round(totaltime/count));
			records.push_back(record);

			lastStartStation = currentStartStation;
			lastEndStation = currentEndStation;
			totaltime = currenttime;
			count = 1;
		}
	}
	sort(records.begin(), records.end(), sortFunc0);
	//cout << "Pt time cost counts: " << records.size() << endl;
	return records;
}

void assignSimplePTSearchNet(const vector<vector<int>>& data)
{
	instance_preptnet->edgeNum = (int)data.size();

	instance_preptnet->edgeIndex = generate_int_matrix(instance_ptstationcoords->stationNum, 2); // changed
	instance_preptnet->edgeFist = generate_int_arr(instance_preptnet->edgeNum);
	instance_preptnet->edgeList = generate_int_arr(instance_preptnet->edgeNum);
	instance_preptnet->edgeDisU = generate_int_arr(instance_preptnet->edgeNum);
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		instance_preptnet->edgeIndex[i][0] = -1;
		instance_preptnet->edgeIndex[i][1] = -1;
	}

	for (int i = 0; i < instance_preptnet->edgeNum; i++)
	{
		instance_preptnet->edgeFist[i] = data[i][0];
		instance_preptnet->edgeList[i] = data[i][1];
		instance_preptnet->edgeDisU[i] = data[i][2];
	}
	get_sp_index_pre(instance_preptnet->edgeFist, instance_preptnet->edgeNum);
	cout << "Finish pre PT search network building" << endl;
}

void get_sp_index_pre(int a[], int edgeLen)
{
	int value = a[0];
	instance_preptnet->edgeIndex[value][0] = 0;

	for (int i = 1; i < edgeLen; i++)
	{
		if (a[i] == value)
			continue;
		else if (a[i] != value)
		{
			instance_preptnet->edgeIndex[value][1] = i - 1;
			value = a[i];
			instance_preptnet->edgeIndex[value][0] = i;
		}
	}
	instance_preptnet->edgeIndex[value][1] = edgeLen - 1;
}

int findLeastCost(int source, int destination, heap_sch_struct* heap)
{
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		heap->dist[i] = INF_DIST5;
		heap->state[i] = 0;
	}
		
	memset(heap->state, 0, (instance_ptstationcoords->stationNum) * sizeof(char));

	insert_val_to_sp_heap(heap, source, 0, 0, 2, 0);

	while (heap->size >= 0)
	{
		scan_node_flc(heap);
	}

	return heap->dist[destination];
}

void scan_node_flc(heap_sch_struct* heap)
{
	int	tmp_node;
	int inttmp;

	for (int i = instance_preptnet->edgeIndex[heap->topNode][0]; i <= instance_preptnet->edgeIndex[heap->topNode][1]; i++)
	{
		if (i != -1)
		{
			tmp_node = instance_preptnet->edgeList[i]; // next node
			inttmp = heap->dist[heap->topNode] + instance_preptnet->edgeDisU[i];

			if (inttmp < heap->dist[tmp_node])
			{
				heap->dist[tmp_node] = inttmp;

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

int countRouteNum()
{
	int count = 1;
	int temp_route = pre_PT_conn->routeNum[0];
	for (int i = 1; i < pre_PT_conn->numberOfConn; i++)
	{
		if (pre_PT_conn->routeNum[i] != temp_route)
		{
			count++;
			temp_route = pre_PT_conn->routeNum[i];
		}
	}
	return count;
}

void assignNewIdxRoute()
{
	pre_PT_conn_computed->numOfRoute = countRouteNum();

	pre_PT_conn_computed->oldrouteNum = generate_int_arr(pre_PT_conn_computed->numOfRoute);
	pre_PT_conn_computed->newrouteNum = generate_int_arr(pre_PT_conn_computed->numOfRoute);
	pre_PT_conn_computed->countOfsta = generate_int_arr(pre_PT_conn_computed->numOfRoute);

	int staCount = 0;
	int routidx = 0;
	pre_PT_conn_computed->oldrouteNum[routidx] = pre_PT_conn->routeNum[0];
	pre_PT_conn_computed->newrouteNum[routidx] = routidx;
	for (int i = 1; i < pre_PT_conn->numberOfConn; i++)
	{
		if (pre_PT_conn->routeNum[i] != pre_PT_conn_computed->oldrouteNum[routidx])
		{
			routidx++;
			pre_PT_conn_computed->oldrouteNum[routidx] = pre_PT_conn->routeNum[i];
			pre_PT_conn_computed->newrouteNum[routidx] = routidx;
		}
	}

	for (int i = 0; i < pre_PT_conn_computed->numOfRoute; i++)
	{
		int temp_route = pre_PT_conn_computed->oldrouteNum[i];

		vector<int> temp_sta;
		for (int j = 0; j < pre_PT_conn->numberOfConn; j++)
		{
			
			if (pre_PT_conn->routeNum[j] == temp_route)
			{
				temp_sta.push_back(pre_PT_conn->startStation[j]);
				temp_sta.push_back(pre_PT_conn->endStation[j]);
			}
		}

		std::sort(temp_sta.begin(), temp_sta.end());
		pre_PT_conn_computed->countOfsta[i] = std::unique(temp_sta.begin(), temp_sta.end()) - temp_sta.begin();
	}
}

int findRouteNewIdx(int oldRouteIdx)
{
	for (int i = 0; i < pre_PT_conn_computed->numOfRoute; i++)
	{
		if (pre_PT_conn_computed->oldrouteNum[i] == oldRouteIdx)
			return i;
	}
}

vector<vector<int>> findRouteRecord(int oldRouteIdx)
{
	vector<vector<int>> result;
	for (int i = 0; i < pre_PT_conn->numberOfConn; i++)
	{
		if (pre_PT_conn->routeNum[i] == oldRouteIdx)
		{
			vector<int> arr;
			arr.push_back(pre_PT_conn->startStation[i]);
			arr.push_back(pre_PT_conn->endStation[i]);
			arr.push_back(findRouteNewIdx(oldRouteIdx));
			arr.push_back(pre_PT_conn->routeNum[i]);
			result.push_back(arr);
		}
	}
	//cout << pre_PT_conn_computed->countOfsta[findRouteNewIdx(oldRouteIdx)] << endl;
	return result;
}

vector<vector<int>> replaceStaNode()
{
	vector<vector<int>> result;
	int total_count = 0;

	for (int i = 0; i < pre_PT_conn_computed->numOfRoute; i++)
	{
		int oldRouteIdx = pre_PT_conn_computed->oldrouteNum[i];
		int numOfsta = pre_PT_conn_computed->countOfsta[i];

		vector<vector<int>> data = findRouteRecord(oldRouteIdx); // all records for one route idx

		vector<int> station;
		for (int j = 0; j < data.size(); j++)
		{
			station.push_back(data[j][0]); // start station
			station.push_back(data[j][1]); // end station
		}

		sort(station.begin(), station.end());
		station.erase(unique(station.begin(), station.end()), station.end());

		int** replaceMa = generate_int_matrix(station.size(), 2);
		for (int k = 0; k < numOfsta; k++)
		{
			replaceMa[k][0] = station[k];
			replaceMa[k][1] = total_count + k;
		}

		for (int j = 0; j < data.size(); j++)
		{
			int station1 = data[j][0];
			int station2 = data[j][1];
			int idx1 = 0;
			int idx2 = 0;

			for (int k = 0; k < numOfsta; k++)
			{
				if (replaceMa[k][0] == station1)
					idx1 = replaceMa[k][1];
				if (replaceMa[k][0] == station2)
					idx2 = replaceMa[k][1];
			}

			//forward
			vector<int> temp;
			temp.push_back(idx1);
			temp.push_back(idx2);
			temp.push_back(station1);
			temp.push_back(station2);
			temp.push_back(data[j][2]);
			temp.push_back(data[j][3]);
			result.push_back(temp);

			//backward
			vector<int> temp1;
			temp1.push_back(idx2);
			temp1.push_back(idx1);
			temp1.push_back(station2);
			temp1.push_back(station1);
			temp1.push_back(data[j][2]);
			temp1.push_back(data[j][3]);
			result.push_back(temp1);
		}


		total_count += numOfsta;
	}

	instance_preptnet_trs->nodeNum = total_count;

	sort(result.begin(), result.end());
	result.erase(unique(result.begin(), result.end()), result.end());
	return result;
}

vector<vector<int>> computeGraphdata()
{
	vector<vector<int>> realConn = replaceStaNode();
	vector<vector<int>> trsConn;
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		vector<int> nodeASta;
		for (int j = 0; j < realConn.size(); j++)
		{
			if (realConn[j][2] == i)
				nodeASta.push_back(realConn[j][0]);
			if (realConn[j][3] == i)
				nodeASta.push_back(realConn[j][1]);

			
		}
		sort(nodeASta.begin(), nodeASta.end());
		nodeASta.erase(unique(nodeASta.begin(), nodeASta.end()), nodeASta.end());
		pre_PT_selection->routePerSta[i] = nodeASta.size();

		// tansfer links
		for (int m = 0; m < nodeASta.size(); m++)
		{
			for (int n = 0; n < nodeASta.size(); n++)
			{
				if (m == n)
					continue;

				vector<int> aTrsConn;
				aTrsConn.push_back(nodeASta[m]);
				aTrsConn.push_back(nodeASta[n]);
				aTrsConn.push_back(i);
				aTrsConn.push_back(i);
				aTrsConn.push_back(100000);
				trsConn.push_back(aTrsConn);
			}
		}
	}

	// real links
	for (int j = 0; j < realConn.size(); j++)
	{
		vector<int> temp;

		temp.push_back(realConn[j][0]);
		temp.push_back(realConn[j][1]);
		temp.push_back(realConn[j][2]);
		temp.push_back(realConn[j][3]);
		temp.push_back(1);
		trsConn.push_back(temp);
	}

	sort(trsConn.begin(), trsConn.end(), sortFunc0);
	//cout << trsConn.size() << endl;
	return trsConn;
}

void assignSimplePTTrsSearchNet(const vector<vector<int>>& data)
{
	instance_preptnet_trs->edgeNum = (int)data.size();

	instance_preptnet_trs->edgeIndex = generate_int_matrix(instance_preptnet_trs->nodeNum, 2); // changed
	instance_preptnet_trs->edgeFist = generate_int_arr(instance_preptnet_trs->edgeNum);
	instance_preptnet_trs->edgeList = generate_int_arr(instance_preptnet_trs->edgeNum);
	instance_preptnet_trs->edgeFistS = generate_int_arr(instance_preptnet_trs->edgeNum);
	instance_preptnet_trs->edgeListS = generate_int_arr(instance_preptnet_trs->edgeNum);
	instance_preptnet_trs->edgeDisU = generate_int_arr(instance_preptnet_trs->edgeNum);
	for (int i = 0; i < instance_preptnet_trs->nodeNum; i++)
	{
		instance_preptnet_trs->edgeIndex[i][0] = -1;
		instance_preptnet_trs->edgeIndex[i][1] = -1;
	}

	for (int i = 0; i < instance_preptnet_trs->edgeNum; i++)
	{
		instance_preptnet_trs->edgeFist[i] = data[i][0];
		instance_preptnet_trs->edgeList[i] = data[i][1];
		instance_preptnet_trs->edgeFistS[i] = data[i][2];
		instance_preptnet_trs->edgeListS[i] = data[i][3];
		instance_preptnet_trs->edgeDisU[i] = data[i][4];
	}
	get_sp_index_pre_trs(instance_preptnet_trs->edgeFist, instance_preptnet_trs->edgeNum);

	//for (int i = 0; i < instance_preptnet_trs->nodeNum; i++)
		//cout << instance_preptnet_trs->edgeIndex[i][0] << " " << instance_preptnet_trs->edgeIndex[i][1] << endl;
	//cout << "PT transfer network: " << instance_preptnet_trs->nodeNum << " " << instance_preptnet_trs->edgeNum << endl;
	cout << "Finish pre PT transfer search network building" << endl;
}

void get_sp_index_pre_trs(int a[], int edgeLen)
{
	int value = a[0];
	instance_preptnet_trs->edgeIndex[value][0] = 0;

	for (int i = 1; i < edgeLen; i++)
	{
		if (a[i] == value)
			continue;
		else if (a[i] != value)
		{
			instance_preptnet_trs->edgeIndex[value][1] = i - 1;
			value = a[i];
			instance_preptnet_trs->edgeIndex[value][0] = i;
		}
	}
	instance_preptnet_trs->edgeIndex[value][1] = edgeLen - 1;
}

vector<int> findStartNode(int startStation)
{
	vector<int> result;
	for (int i = 0; i < instance_preptnet_trs->edgeNum; i++)
	{
		if (instance_preptnet_trs->edgeFistS[i] == startStation && instance_preptnet_trs->edgeDisU[i] == 1)
			result.push_back(instance_preptnet_trs->edgeFist[i]);
	}
	return result;
}

int findTrsLeastCost(int source, int destination, heap_sch_struct* heap)
{
	for (int i = 0; i < instance_preptnet_trs->nodeNum; i++)
	{
		heap->dist[i] = INF_DIST5;
		heap->state[i] = 0;
	}
	memset(heap->state, 0, (instance_preptnet_trs->nodeNum) * sizeof(char));

	vector<int> info = findStartNode(source);
	int* arr = generate_int_arr(instance_ptstationcoords->stationNum);
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
		arr[i] = INF_DIST5;
	arr[source] = 0;

	insert_val_to_sp_heap(heap, info[0], 0, 0, 2, 0);
	heap->dist[info[0]] = 0;

	for (int i = 1; i < info.size(); i++)
	{
		InsertElement(0, info[i], heap);
	}

	while (heap->size >= 0)
	{
		scan_node_ftlc(heap, arr);
	}

	return arr[destination];
}

void scan_node_ftlc(heap_sch_struct* heap, int arr[])
{
	int	tmp_node;
	int inttmp;
	int station;

	for (int i = instance_preptnet_trs->edgeIndex[heap->topNode][0]; i <= instance_preptnet_trs->edgeIndex[heap->topNode][1]; i++)
	{
		if (i != -1)
		{
 			tmp_node = instance_preptnet_trs->edgeList[i]; // next node
			inttmp = heap->dist[heap->topNode] + instance_preptnet_trs->edgeDisU[i];
			station = instance_preptnet_trs->edgeListS[i];

			if (inttmp < heap->dist[tmp_node])
			{
				heap->dist[tmp_node] = inttmp;

				if (inttmp <= arr[station])
					arr[station] = inttmp;

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

bool transferTime(int s1, int s2)
{
	resetHeap(instance_preptnet_trs->nodeNum, sp_PT_heap_pre_transfer, 1);
	int num1 = findTrsLeastCost(s1, s2, sp_PT_heap_pre_transfer)/ 100000;
	//cout << "num1: " << num1 << endl;

	resetHeap(instance_preptnet_trs->nodeNum, sp_PT_heap_pre_transfer, 1);
	int num2 = findTrsLeastCost(s2, s1, sp_PT_heap_pre_transfer)/ 100000;
	//cout << "num2: " << num2 << endl;

	if (num1 <= num2)
		return true;
	else
		return false;
}

void timeAndTrsSelection(const vector<int>& stations, int psersonidx, int pvflag)
{
	int pttime = 0;
	int time1 = 0;
	int time2 = 0;
	int time3 = 0;
	int time4 = 0;
	vector<vector<int>> compareResult;
	for (int i : stations)
	{
		for (int j : stations)
		{
			if (pre_PT_selection->result[i] != 0 && pre_PT_selection->result[j] != 0)
			{
				if (i == j)
					continue;
				else
				{
					pttime = findLeastCost(i, j, sp_PT_heap_pre_time) * 10000;
					time1 = pre_PT_selection->timeFromA1[i]; // time*10000
					time2 = pre_PT_selection->timeFromA1[j];
					time3 = pre_PT_selection->timeFromA2[i];
					time4 = pre_PT_selection->timeFromA2[j];

					resetHeap(instance_ptstationcoords->stationNum, sp_PT_heap_pre_time, 1);

					if (time1 + pttime <= time2 && time3 + pttime <= time4)
					{
						pre_PT_selection->result[j] = 0;

						vector<int> result;
						result.push_back(i);
						result.push_back(j);
						result.push_back(instance_ptstationcoords->stationcoordx[i]);
						result.push_back(instance_ptstationcoords->stationcoordy[i]);
						result.push_back(instance_ptstationcoords->stationcoordx[j]);
						result.push_back(instance_ptstationcoords->stationcoordy[j]);
						result.push_back(pttime);
						result.push_back(time1);
						result.push_back(time2);
						result.push_back(time3);
						result.push_back(time4);
						compareResult.push_back(result);
					}

					/*
					if ((time1 <= time2) && transferTime(i, j))
					{
						//pre_PT_selection->result[i] = 1;
						pre_PT_selection->result[j] = 0;
					}
					*/
				}
			}
		}
	}
	//output_data_pre_step3_trianglecompare(compareResult, psersonidx);

	vector<vector<int>> outputs;
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		if (pre_PT_selection->result[i] == 1)
		{
			vector<int> output;
			output.push_back(i);
			output.push_back(instance_ptstationcoords->stationcoordx[i]);
			output.push_back(instance_ptstationcoords->stationcoordy[i]);
			outputs.push_back(output);
		}
	}
	output_data_pre_step3(outputs, psersonidx, pvflag);
}

// nearest stations
vector<vector<int>> findNerestStations(int numOfStation, int startNode)
{
	int x1 = instance_pvcoords->roadNodecoordx[startNode];
	int y1 = instance_pvcoords->roadNodecoordy[startNode];

	vector<vector<int>> result;
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		int x2 = instance_ptstationcoords->stationcoordx[i];
		int y2 = instance_ptstationcoords->stationcoordy[i];
		int length = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

		vector<int> staLen;
		staLen.push_back(i);
		staLen.push_back(length);
		result.push_back(staLen);
	}
	sort(result.begin(), result.end(), sortFunc1);
	//output_data_ptforward(result, startNode, startNode);
	
	vector<vector<int>> outputs;
	for (int i = 0; i < numOfStation; i++)
	{
		vector<int> output;
		output.push_back(result[i][0]);
		output.push_back(instance_pvpt_conn->n2Station[result[i][0]][0]);
		outputs.push_back(output);
	}

	return outputs;
}


// in all
vector<vector<int>> stationPreSelection_without(int pvflag, int personidx)
{
	// initialization
	setPVEdgeDistU_pre(pvflag); // time*10000
	init_space_stationSlection();

	assignNewIdxRoute();
	assignSimplePTTrsSearchNet(computeGraphdata());

	// step 1: planar PPA
	sp_stp_1_planar(sp_PV_heap_pre_planar_1, sp_PV_heap_pre_planar_2, pvflag, personidx);
	vector<vector<int>> outputs;
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		if (pre_PT_selection->planarPPA_final[i] == 1)
		{
			vector<int> output;
			output.push_back(i);
			output.push_back(instance_pvpt_conn->n2Station[i][0]);
			outputs.push_back(output);
		}
	}
	delete_pre_selection_space();
	cout << "without stations: " << outputs.size() << endl;
	return outputs;
}
vector<vector<int>> stationPreSelection(int pvflag, int personidx)
{
	// initialization
	setPVEdgeDistU_pre(pvflag); // time*10000
	init_space_stationSlection();
	
	
	assignNewIdxRoute();
	assignSimplePTTrsSearchNet(computeGraphdata());

	// step 1: planar PPA
	sp_stp_1_planar(sp_PV_heap_pre_planar_1, sp_PV_heap_pre_planar_2, pvflag, personidx);
	vector<vector<int>> outputs;
	
	// step 2: PC4
	// check the read in dataset, instance_ptstationcoords->isPC4 for bike, instance_ptstationcoords->isTAZ for car
	vector<int> stations = interSelection(pvflag, personidx);

	// step 3: PT time cost and transfer
	timeAndTrsSelection(stations, personidx, pvflag);

	// step 4: distance delete
	if (pvflag == 1)
	{
		distanceDelete(pvflag);
	}
	

	vector<vector<int>> outputs2;
	int count = 0;
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		if (pre_PT_selection->result[i] == 1)
		{
			//cout << "station: " << i << endl;
			count++;

			vector<int> output;
			output.push_back(i);
			output.push_back(instance_pvpt_conn->n2Station[i][0]);
			outputs2.push_back(output);
		}
	}
	output_data_pre_step4_distcompare(outputs2, personidx, pvflag);
	delete_pre_selection_space();
	cout << "without stations: " << outputs2.size() << endl;
	return outputs2;
}

void distanceDelete(int pvflag)
{
	int startNode = instance_stpap->startNode;
	int endNode = instance_stpap->endNode;
	int timeBudget = instance_stpap->timeBudget / 10000;
	int activityTime = instance_stpap->approxActivityTime / 10000;
	int spareTime = timeBudget - activityTime;

	int i = findI_pv(startNode);
	int j = findI_pv(endNode);
	int x1 = instance_pvcoords->roadNodecoordx[i];
	int y1 = instance_pvcoords->roadNodecoordy[i];
	int x2 = instance_pvcoords->roadNodecoordx[j];
	int y2 = instance_pvcoords->roadNodecoordy[j];

	int x = (x1 + x2) / 2;
	int y = (y1 + y2) / 2;
	cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x << " " << y << endl;
	int speed = getSpeed(pvflag);
	cout << speed << endl;
	cout << spareTime << endl;
	int length = (speed * 1000 * spareTime) / 120;
	cout << "length is " << length << endl;
	/*
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		if (pre_PT_selection->result[i] == 1)
		{
			int sx = instance_ptstationcoords->stationcoordx[i];
			int sy = instance_ptstationcoords->stationcoordy[i];

			int len = sqrt((sx - x) * (sx - x) + (sy - y) * (sy - y));

			if (len > length)
				pre_PT_selection->result[i] = 0;
		}
	}*/
}