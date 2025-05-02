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

using namespace std;

int		INF_DIST3 = 2000000;

heap_sch_struct* sp_PT_alt;
heap_sch_struct* sp_PV_alt;

void assignPTSearchNet(const vector<vector<int>>& data)
{
	instance_spnetpt->edgeNum = (int)data.size();
	instance_spnetpt->edgeIndex = generate_int_matrix(instance_spnetpt->nodeNum, 2); // changed
	instance_spnetpt->edgeFist = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->edgeList = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->edgeDisUFist = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->edgeDisUList = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->edgeDisU = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->Flag = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->edgeDisUTrs = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->edgeFistS = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->edgeListS = generate_int_arr(instance_spnetpt->edgeNum);

	instance_spnetpt->lineNum = generate_int_arr(instance_spnetpt->edgeNum);
	instance_spnetpt->routeNum = generate_int_arr(instance_spnetpt->edgeNum);
	for (int i = 0; i < instance_spnetpt->nodeNum; i++)
	{
		instance_spnetpt->edgeIndex[i][0] = -1;
		instance_spnetpt->edgeIndex[i][1] = -1;
	}

	for (int i = 0; i < instance_spnetpt->edgeNum; i++)
	{
		instance_spnetpt->edgeFist[i] = data[i][0];
		instance_spnetpt->edgeList[i] = data[i][1];
		instance_spnetpt->edgeDisUFist[i] = data[i][2];
		instance_spnetpt->edgeDisUList[i] = data[i][3];
		instance_spnetpt->edgeDisU[i] =data[i][3] - data[i][2];
		instance_spnetpt->Flag[i] = data[i][6];
		instance_spnetpt->edgeFistS[i] = data[i][7];
		instance_spnetpt->edgeListS[i] = data[i][8];
		instance_spnetpt->lineNum[i] = data[i][4];
		instance_spnetpt->routeNum[i] = data[i][5];

		if (data[i][6] == 4 || data[i][6] == 5)
		{
			instance_spnetpt->edgeDisUTrs[i] = 1;
		}
		else
		{
			instance_spnetpt->edgeDisUTrs[i] = 0;
		}

	}
	get_sp_index(instance_spnetpt->edgeFist, instance_spnetpt->edgeNum);
	cout << "Finish PT search network building" << endl;
}

void assignPTSearchNet_reverse(vector<vector<int>>& data)
{
	sort(data.begin(), data.end(), sortFunc1); // sort data according to end nodes
	instance_spnetpt_reverse->edgeNum = (int)data.size();
	instance_spnetpt_reverse->nodeNum = instance_spnetpt->nodeNum;
	instance_spnetpt_reverse->edgeIndex = generate_int_matrix(instance_spnetpt_reverse->nodeNum, 2); // changed
	instance_spnetpt_reverse->edgeFist = generate_int_arr(instance_spnetpt_reverse->edgeNum);
	instance_spnetpt_reverse->edgeList = generate_int_arr(instance_spnetpt_reverse->edgeNum);
	instance_spnetpt_reverse->edgeDisUFist = generate_int_arr(instance_spnetpt_reverse->edgeNum);
	instance_spnetpt_reverse->edgeDisUList = generate_int_arr(instance_spnetpt_reverse->edgeNum);
	instance_spnetpt_reverse->edgeDisU = generate_int_arr(instance_spnetpt_reverse->edgeNum);
	instance_spnetpt_reverse->Flag = generate_int_arr(instance_spnetpt_reverse->edgeNum);
	instance_spnetpt_reverse->edgeDisUTrs = generate_int_arr(instance_spnetpt_reverse->edgeNum);
	instance_spnetpt_reverse->edgeFistS = generate_int_arr(instance_spnetpt_reverse->edgeNum);
	instance_spnetpt_reverse->edgeListS = generate_int_arr(instance_spnetpt_reverse->edgeNum);

instance_spnetpt_reverse->lineNum = generate_int_arr(instance_spnetpt_reverse->edgeNum);
instance_spnetpt_reverse->routeNum = generate_int_arr(instance_spnetpt_reverse->edgeNum);
for (int i = 0; i < instance_spnetpt_reverse->nodeNum; i++)
{
	instance_spnetpt_reverse->edgeIndex[i][0] = -1;
	instance_spnetpt_reverse->edgeIndex[i][1] = -1;
}

for (int i = 0; i < instance_spnetpt_reverse->edgeNum; i++)
{
	instance_spnetpt_reverse->edgeFist[i] = data[i][1];
	instance_spnetpt_reverse->edgeList[i] = data[i][0];
	instance_spnetpt_reverse->edgeDisUFist[i] = data[i][3];
	instance_spnetpt_reverse->edgeDisUList[i] = data[i][2];
	instance_spnetpt_reverse->edgeDisU[i] = data[i][3] - data[i][2];
	instance_spnetpt_reverse->Flag[i] = data[i][6];
	instance_spnetpt_reverse->edgeFistS[i] = data[i][8];
	instance_spnetpt_reverse->edgeListS[i] = data[i][7];
	instance_spnetpt_reverse->lineNum[i] = data[i][4];
	instance_spnetpt_reverse->routeNum[i] = data[i][5];

	if (data[i][6] == 4 || data[i][6] == 5)
	{
		instance_spnetpt_reverse->edgeDisUTrs[i] = 1;
	}
	else
	{
		instance_spnetpt_reverse->edgeDisUTrs[i] = 0;
	}
}
get_sp_index_reverse(instance_spnetpt_reverse->edgeFist, instance_spnetpt_reverse->edgeNum);
cout << "Finish PT reversed search network building" << endl;
}

int calDist(int stationidx, int nodeidx)
{
	int x1 = instance_ptstationcoords->stationcoordx[stationidx];
	int y1 = instance_ptstationcoords->stationcoordy[stationidx];
	int idx = findIdx(nodeidx);
	int x2 = instance_pvcoords->roadNodecoordx[idx];
	int y2 = instance_pvcoords->roadNodecoordy[idx];

	return get_euDist(x1, y1, x2, y2);
}

int get_euDist(int x1, int y1, int x2, int y2)
{
	float	a, b, c;
	a = (x1 - x2) / 1000.0f;
	b = (y1 - y2) / 1000.0f;

	c = sqrt(a * a + b * b);
	assert(c >= 0);
	return int(1210 * c);
}

void assignPVPTConnNet()
{
	instance_pvpt_conn->n2Station = generate_int_matrix(instance_ptstationcoords->stationNum, 2);
	instance_pvpt_conn->node2Station = generate_int_matrix(instance_pvcoords->roadNodeNum, instance_ptstationcoords->stationNum);
	instance_pvpt_conn->station2Node = generate_int_matrix(instance_pvcoords->roadNodeNum, instance_ptstationcoords->stationNum);

	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		instance_pvpt_conn->n2Station[i][0] = INF_DIST3;
		instance_pvpt_conn->n2Station[i][1] = INF_DIST3;
	}

	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		for (int j = 0; j < instance_ptstationcoords->stationNum; j++)
		{
			instance_pvpt_conn->node2Station[i][j] = INF_DIST3;
			instance_pvpt_conn->station2Node[i][j] = INF_DIST3;
		}

	}

	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		vector<vector<int>> distset;
		int temp_dist = INF_DIST3;
		for (int j = 0; j < instance_ptstationcoords->stationNum; j++)
		{
			vector<int> temp;
			int dist = calDist(j, i); // dist between station j and road node i
			temp.push_back(j);
			temp.push_back(dist);
			distset.push_back(temp);

			if (dist < instance_pvpt_conn->n2Station[j][1])
			{
				instance_pvpt_conn->n2Station[j][1] = dist;
				instance_pvpt_conn->n2Station[j][0] = i; // road node idx
			}
		}
		sort(distset.begin(), distset.end(), sortFunc1);

		for (int m = 0; m < instance_ptstationcoords->stationNum; m++)
		{
			instance_pvpt_conn->node2Station[i][m] = distset[m][0]; // node to station dist from min to max
		}
	}

	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		for (int j = 0; j < instance_ptstationcoords->stationNum; j++)
		{
			int station = instance_pvpt_conn->node2Station[i][j];
			instance_pvpt_conn->station2Node[i][j] = instance_pvpt_conn->n2Station[station][0]; // corresponding node for each station in a row
		}
	}

	cout << "Finish assign connections between road and station" << endl;
}

void get_sp_index(int a[], int edgeLen)
{
	int value = a[0];
	instance_spnetpt->edgeIndex[value][0] = 0;

	for (int i = 1; i < edgeLen; i++)
	{
		if (a[i] == value)
			continue;
		else if (a[i] != value)
		{
			instance_spnetpt->edgeIndex[value][1] = i - 1;
			value = a[i];
			instance_spnetpt->edgeIndex[value][0] = i;
		}
	}
	instance_spnetpt->edgeIndex[value][1] = edgeLen - 1;
}

void get_sp_index_reverse(int a[], int edgeLen)
{
	int value = a[0];
	instance_spnetpt_reverse->edgeIndex[value][0] = 0;

	for (int i = 1; i < edgeLen; i++)
	{
		if (a[i] == value)
			continue;
		else if (a[i] != value)
		{
			instance_spnetpt_reverse->edgeIndex[value][1] = i - 1;
			value = a[i];
			instance_spnetpt_reverse->edgeIndex[value][0] = i;
		}
	}
	instance_spnetpt_reverse->edgeIndex[value][1] = edgeLen - 1;
}

void get_sp_index_pv(int a[], int edgeLen)
{
	int value = a[0];
	instance_spnetpv->edgeIndex[value][0] = 0;

	for (int i = 1; i < edgeLen; i++)
	{
		if (a[i] == value)
			continue;
		else if (a[i] != value)
		{
			instance_spnetpv->edgeIndex[value][1] = i - 1;
			value = a[i];
			instance_spnetpv->edgeIndex[value][0] = i;
		}
	}
	instance_spnetpv->edgeIndex[value][1] = edgeLen - 1;
}

void assignPVSearchNet()
{
	instance_spnetpv->nodeNum = instance_pvcoords->roadNodeNum;
	instance_spnetpv->oldIdx = generate_int_arr(instance_spnetpv->nodeNum);
	instance_spnetpv->newIdx = generate_int_arr(instance_spnetpv->nodeNum);

	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		instance_spnetpv->oldIdx[i] = instance_pvcoords->roadNodeOldIdx[i];

		instance_pvcoords->roadNodeNewIdx[i] = i;
		instance_spnetpv->newIdx[i] = i;
	}

	instance_spnetpv->edgeNum = instance_pvdata->edgeNum * 2; // add for/back edges
	instance_spnetpv->edgeFist = generate_int_arr(instance_spnetpv->edgeNum);
	instance_spnetpv->edgeList = generate_int_arr(instance_spnetpv->edgeNum);
	instance_spnetpv->edgeLength = generate_int_arr(instance_spnetpv->edgeNum);
	instance_spnetpv->Flag = generate_int_arr(instance_spnetpv->edgeNum);
	instance_spnetpv->iscity = generate_int_arr(instance_spnetpv->edgeNum);
	instance_spnetpv->edgeDisU = generate_int_arr(instance_spnetpv->edgeNum);
	instance_spnetpv->edgeDisUWalk = generate_int_arr(instance_spnetpv->edgeNum);
	instance_spnetpv->edgeIndex = generate_int_matrix(instance_spnetpv->nodeNum, 2);

	vector<vector<int>> temp_array;
	for (int j = 0; j < instance_pvdata->edgeNum; j++)
	{
		vector<int> arr;
		vector<int> brr;
		int firstNode = findNewIdx(instance_pvdata->edgeFistNode[j]);
		int secondNode = findNewIdx(instance_pvdata->edgeListNode[j]);

		arr.push_back(firstNode);
		arr.push_back(secondNode);
		arr.push_back(instance_pvdata->length[j]);
		arr.push_back(instance_pvdata->roadflag[j]);
		arr.push_back(instance_pvdata->cityflag[j]);

		brr.push_back(secondNode);
		brr.push_back(firstNode);
		brr.push_back(instance_pvdata->length[j]);
		brr.push_back(instance_pvdata->roadflag[j]);
		brr.push_back(instance_pvdata->cityflag[j]);

		temp_array.push_back(arr);
		temp_array.push_back(brr);
	}
	sort(temp_array.begin(), temp_array.end(), sortFunc0);
	for (int j = 0; j < temp_array.size(); j++)
	{
		instance_spnetpv->edgeFist[j] = temp_array[j][0];
		instance_spnetpv->edgeList[j] = temp_array[j][1];
		instance_spnetpv->edgeLength[j] = temp_array[j][2];
		instance_spnetpv->Flag[j] = temp_array[j][3];
		instance_spnetpv->iscity[j] = temp_array[j][4];
	}

	get_sp_index_pv(instance_spnetpv->edgeFist, instance_spnetpv->edgeNum);

	cout << "Finish PV search network building" << endl;
}

int findNewIdx(int oldidx)
{
	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		if (oldidx == instance_spnetpv->oldIdx[i])
			return instance_spnetpv->newIdx[i];
	}
}

int findIdx(int newidx)
{
	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		if (newidx == instance_spnetpv->newIdx[i])
		{
			return i;
		}
	}
}

double costCal(int length, int v)
{
	double vv = v * 1000 / 60;
	double time = (length / vv) * 10000; // in min
	//if (time == 0)
		//time = 1;
	return time;
}

int costCal_pre(int length, int v)
{
	double vv = v * 1000 / 60;
	double time = (length / vv)*10000; // in min
	//if (time == 0)
		//time = 1;
	return time;
}

void setPVEdgeDistU_pre(int flag)
{
	for (int i = 0; i < instance_spnetpv->edgeNum; i++)
	{
		instance_spnetpv->edgeDisU[i] = 0;
	}

	for (int i = 0; i < instance_spnetpv->edgeNum; i++)
	{
		int roadflag = instance_spnetpv->Flag[i];
		int areaflag = instance_spnetpv->iscity[i];
		int v = getSpeed(flag, roadflag, areaflag);
		if (v == 0)
		{
			instance_spnetpv->edgeDisU[i] = INF_DIST3;
		}
		else
		{
			int time = costCal_pre(instance_spnetpv->edgeLength[i], v);
			instance_spnetpv->edgeDisU[i] = time;
		}


	}

	//cout << "Already set pre PV net disutility!" << endl;
}

void setPVEdgeDistU(int flag)
{
	for (int i = 0; i < instance_spnetpv->edgeNum; i++)
	{
		instance_spnetpv->edgeDisUWalk[i] = 0;
	}

	for (int i = 0; i < instance_spnetpv->edgeNum; i++)
	{
		int roadflag = instance_spnetpv->Flag[i];
		int areaflag = instance_spnetpv->iscity[i];
		int v = getSpeed(flag, roadflag, areaflag);
		if (v == 0)
		{
			instance_spnetpv->edgeDisUWalk[i] = INF_DIST3; // change
		}
		else
		{
			int time = costCal(instance_spnetpv->edgeLength[i], v); // real time
			if(time == 0)
				instance_spnetpv->edgeDisUWalk[i] = 10000;
			else
				instance_spnetpv->edgeDisUWalk[i] = time;
		}


	}

	cout << "Already set PV net disutility for walk!" << endl;
}

void assignPV2Shop()
{
	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		for (int j = 0; j < shop_data->numOfactivity; j++)
		{
			if (instance_pvcoords->roadNodeOldIdx[i] == shop_data->activitynodeidx[j])
				instance_pvcoords->isShop[i] = 1;
		}
	}
	
	
	cout << "Finish assign shop locations in PV" << endl;
}

void assignSta2Shop(int numOfAroundSta)
{
	setPVEdgeDistU_pre(0);
	shop_data->cost = generate_int_matrix(instance_ptstationcoords->stationNum, numOfAroundSta);
	shop_data->index = generate_int_matrix(instance_ptstationcoords->stationNum, numOfAroundSta);

	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		for (int j = 0; j < numOfAroundSta; j++)
		{
			shop_data->cost[i][j] = 0;
			shop_data->index[i][j] = 0;
		}
			
	}

	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		int stainpv = instance_pvpt_conn->n2Station[i][0];
		int idx = findIdx(stainpv);
		int x1 = instance_pvcoords->roadNodecoordx[idx];
		int y1 = instance_pvcoords->roadNodecoordy[idx];

		vector<vector<int>> costs;
		for (int j = 0; j < shop_data->numOfactivity; j++)
		{
			int x2 = shop_data->activitycoordx[j];
			int y2 = shop_data->activitycoordy[j];
			int length = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) * 1.4;
			int time = costCal_pre(length, 5);

			if (time/10000 > 15)
			{
				vector<int> temp;
				temp.push_back(j);
				temp.push_back(INF_DIST3);
				costs.push_back(temp);
			}
			else
			{
				vector<int> temp;
				temp.push_back(j);
				temp.push_back(time/10000);
				costs.push_back(temp);
			}
			
		}
		sort(costs.begin(), costs.end(), sortFunc1);

		for (int k = 0; k < numOfAroundSta; k++)
		{
			shop_data->cost[i][k] = costs[k][1];
			shop_data->index[i][k] = costs[k][0];
		}
	}

	cout << "Finish assign shop data" << endl;
}

void assignSta2Park(int numOfAroundSta)
{
	setPVEdgeDistU_pre(0);
	park_data->cost = generate_int_matrix(instance_ptstationcoords->stationNum, numOfAroundSta);
	park_data->index = generate_int_matrix(instance_ptstationcoords->stationNum, numOfAroundSta);

	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		for (int j = 0; j < numOfAroundSta; j++)
		{
			park_data->cost[i][j] = 0;
			park_data->index[i][j] = 0;
		}
			
	}

	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		int stainpv = instance_pvpt_conn->n2Station[i][0];
		int idx = findIdx(stainpv);
		int x1 = instance_pvcoords->roadNodecoordx[idx];
		int y1 = instance_pvcoords->roadNodecoordy[idx];

		vector<vector<int>> costs;
		for (int j = 0; j < park_data->numOfactivity; j++)
		{
			int x2 = park_data->activitycoordx[j];
			int y2 = park_data->activitycoordy[j];
			int length = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) * 1.4;
			int time = costCal_pre(length, 5);

			if (time / 10000 > 15)
			{
				vector<int> temp;
				temp.push_back(j);
				temp.push_back(INF_DIST3);
				costs.push_back(temp);
			}
			else
			{
				vector<int> temp;
				temp.push_back(j);
				temp.push_back(time / 10000);
				costs.push_back(temp);
			}

		}
	
		sort(costs.begin(), costs.end(), sortFunc1);

		for (int k = 0; k < numOfAroundSta; k++)
		{
			park_data->cost[i][k] = costs[k][1];
			park_data->index[i][k] = costs[k][0];
		}
	}
	cout << " Finish assign park data" << endl;
}

vector<vector<int>> findPVlandmarks(int numOfLandmark, int startnodeid)
{
	vector<vector<int>> landmarks;
	int startx = instance_pvcoords->roadNodecoordx[startnodeid];
	int starty = instance_pvcoords->roadNodecoordy[startnodeid];
	int* ifselected = generate_int_arr(instance_pvcoords->roadNodeNum);

	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		ifselected[i] = 0;
	}

	// first land mark
	int largestDist = 0;
	int id_temp = 0;
	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		int nodex = instance_pvcoords->roadNodecoordx[i];
		int nodey = instance_pvcoords->roadNodecoordy[i];
			
		int temp_dist = get_euDist(nodex, nodey, startx, starty);

		if (temp_dist > largestDist)
		{
			largestDist = temp_dist;
			id_temp = i;
		}
	}

	vector<int> arr;
	arr.push_back(instance_pvcoords->roadNodeNewIdx[id_temp]);
	arr.push_back(instance_pvcoords->roadNodecoordx[id_temp]);
	arr.push_back(instance_pvcoords->roadNodecoordy[id_temp]);
	landmarks.push_back(arr);
	ifselected[id_temp] = 1;

	largestDist = 0;
	id_temp = 0;
	while (landmarks.size() != numOfLandmark)
	{
		largestDist = 0;
		id_temp = 0;
		for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
		{
			if (ifselected[i] != 1)
			{
				int totalDist_temp = 0;
				int nodex = instance_pvcoords->roadNodecoordx[i];
				int nodey = instance_pvcoords->roadNodecoordy[i];
				bool ifjump = false;

				for (int j = 0; j < landmarks.size(); j++)
				{
					int temp_dist = get_euDist(nodex, nodey, landmarks[j][1], landmarks[j][2]);
					if (temp_dist < 9000)
					{
						ifjump = true;
						break;
					}
					totalDist_temp += temp_dist;
				}

				if (totalDist_temp > largestDist && ifjump != true)
				{
					largestDist = totalDist_temp;
					id_temp = i;
				}
			}	
		}

		vector<int> arr;
		arr.push_back(instance_pvcoords->roadNodeNewIdx[id_temp]);
		arr.push_back(instance_pvcoords->roadNodecoordx[id_temp]);
		arr.push_back(instance_pvcoords->roadNodecoordy[id_temp]);
		ifselected[id_temp] = 1;
		landmarks.push_back(arr);
	}

	return landmarks;
}


vector<vector<int>> findPTlandmarks(int numOfLandmarks, int startStation)
{
	vector<vector<int>> landmarks;
	int stationx = instance_ptstationcoords->stationcoordx[startStation];
	int stationy = instance_ptstationcoords->stationcoordy[startStation];
	int* ifselected = generate_int_arr(instance_ptstationcoords->stationNum);

	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		ifselected[i] = 0;
	}

	// first land mark
	int largestDist = 0;
	int id_temp = 0;
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		int nodex = instance_ptstationcoords->stationcoordx[i];
		int nodey = instance_ptstationcoords->stationcoordy[i];

		int temp_dist = get_euDist(nodex, nodey, stationx, stationy);

		if (temp_dist > largestDist)
		{
			largestDist = temp_dist;
			id_temp = i;
		}
	}

	vector<int> arr;
	arr.push_back(id_temp);
	arr.push_back(instance_ptstationcoords->stationcoordx[id_temp]);
	arr.push_back(instance_ptstationcoords->stationcoordy[id_temp]);
	landmarks.push_back(arr);
	ifselected[id_temp] = 1;

	while (landmarks.size() != numOfLandmarks)
	{
		largestDist = 0;
		id_temp = 0;
		for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
		{
			if (ifselected[i] != 1)
			{
				int totalDist_temp = 0;
				int nodex = instance_ptstationcoords->stationcoordx[i];
				int nodey = instance_ptstationcoords->stationcoordy[i];
				bool ifjump = false;

				for (int j = 0; j < landmarks.size(); j++)
				{
					int temp_dist = get_euDist(nodex, nodey, landmarks[j][1], landmarks[j][2]);
					if (temp_dist < 9000)
					{
						ifjump = true;
						break;
					}
					totalDist_temp += temp_dist;
				}

				if (totalDist_temp > largestDist && ifjump != true)
				{
					largestDist = totalDist_temp;
					id_temp = i;
				}
			}
		}

		vector<int> arr;
		arr.push_back(id_temp);
		arr.push_back(instance_ptstationcoords->stationcoordx[id_temp]);
		arr.push_back(instance_ptstationcoords->stationcoordy[id_temp]);
		ifselected[id_temp] = 1;
		landmarks.push_back(arr);
	}

	return landmarks;
}

void allocate_space4ALTdata(int numOfPTmarks, int numOfPVmarks)
{
	//int numOfStation; //PT station
	//int numOflandmark_PT;

	//int numOfNodes; // PV nodes
	//int numOflandmark_PV;

	//PT
	//int** landmarkPT; // id, x coord, y coord
	//int** cost2LandmarkPT; // station num * landmarks

	//PV
	//int** landmarkPV; // id, x coord, y coord
	//int** cost2LandmarkPV; // node number * landmarks

	ALT_data->numOfStation = instance_ptstationcoords->stationNum;
	ALT_data->numOfNodes = instance_pvcoords->roadNodeNum;
	ALT_data->numOflandmark_PT = numOfPTmarks;
	ALT_data->numOflandmark_PV = numOfPVmarks;

	ALT_data->landmarkPT = generate_int_matrix(ALT_data->numOflandmark_PT, 3);
	ALT_data->cost2LandmarkPT = generate_int_matrix(numOfPTmarks, ALT_data->numOfStation);

	ALT_data->landmarkPV = generate_int_matrix(ALT_data->numOflandmark_PV, 3);
	ALT_data->cost2LandmarkPV = generate_int_matrix(numOfPVmarks, ALT_data->numOfNodes);

	for (int i = 0; i < ALT_data->numOfStation; i++)
	{
		for (int j = 0; j < ALT_data->numOfStation; j++)
		{
			ALT_data->cost2LandmarkPT[i][j] = INF_DIST3;
		}
	}

	for (int i = 0; i < ALT_data->numOfNodes; i++)
	{
		for (int j = 0; j < ALT_data->numOfNodes; j++)
		{
			ALT_data->cost2LandmarkPV[i][j] = INF_DIST3;
		}
	}
}

void assignALTvalues(int numofPTlandmarks, int startstation, int numofPVlandmarks, int startnode, int pvflag)
{
	vector<vector<int>> PT_landmarks = findPTlandmarks(numofPTlandmarks, startstation);
	vector<vector<int>> PV_landmarks = findPVlandmarks(numofPVlandmarks, startnode);

	allocate_space4ALTdata(numofPTlandmarks, numofPVlandmarks);

	for (int i = 0; i < numofPTlandmarks; i++)
	{
		ALT_data->landmarkPT[i][0] = PT_landmarks[i][0];
		ALT_data->landmarkPT[i][1] = PT_landmarks[i][1];
		ALT_data->landmarkPT[i][2] = PT_landmarks[i][2];
	}

	for (int j = 0; j < numofPVlandmarks; j++)
	{
		ALT_data->landmarkPV[j][0] = PV_landmarks[j][0];
		ALT_data->landmarkPV[j][1] = PV_landmarks[j][1];
		ALT_data->landmarkPV[j][2] = PV_landmarks[j][2];
	}

	sp_PT_alt = Init_heap(instance_spnetpt->nodeNum, 1);
	sp_PV_alt = Init_heap(instance_spnetpv->nodeNum, 1);

	setPVEdgeDistU(pvflag);
	for (int i = 0; i < numofPVlandmarks; i++)
	{
		sp_PV_alt_search(sp_PV_alt, i);
		resetHeap(instance_spnetpv->nodeNum, sp_PV_alt, 1);
	}
	
	cout << "Finish PV ALT assign" << endl;
	
	for (int i = 0; i < numofPTlandmarks; i++)
	{
		sp_PT_alt_search(sp_PT_alt, i, 420);
		resetHeap(instance_spnetpt->nodeNum, sp_PT_alt, 1);
	}

	cout << "Finish PT ALT search" << endl;
}

void sp_PV_alt_search(heap_sch_struct* heap, int landmarkidx)
{
	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		heap->dist[i] = INF_DIST3;
		heap->state[i] = 0;
	}

	memset(heap->state, 0, (instance_spnetpv->nodeNum) * sizeof(char));
	int nodeidx = ALT_data->landmarkPV[landmarkidx][0];
	ALT_data->cost2LandmarkPV[landmarkidx][nodeidx] = 0;
	insert_val_to_sp_heap(heap, nodeidx, 0, 0, 2, 0);
	while (heap->size >= 0)
	{
		scan_node_PV_alt(heap, landmarkidx);
	}
}

void scan_node_PV_alt(heap_sch_struct* heap, int landmarkidx)
{
	int		tmp_node,  inttmp;

	for (int i = instance_spnetpv->edgeIndex[heap->topNode][0]; i <= instance_spnetpv->edgeIndex[heap->topNode][1]; i++)
	{
		tmp_node = instance_spnetpv->edgeList[i];
		inttmp = heap->dist[heap->topNode] + instance_spnetpv->edgeDisU[i];

		if (inttmp < heap->dist[tmp_node])
		{
			heap->dist[tmp_node] = inttmp;
			ALT_data->cost2LandmarkPV[landmarkidx][tmp_node] = inttmp;
			if (heap->state[tmp_node] == 0)
				InsertElement(inttmp, tmp_node, heap);
			else
				DecreaseKey(inttmp, tmp_node, heap);
		}
	}
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}

void sp_PT_alt_search(heap_sch_struct* heap, int landmarkidx, int startTime)
{
	for (int i = 0; i < instance_spnetpt->nodeNum; i++)
	{
		heap->dist[i] = INF_DIST3;
		heap->distRL[i] = INF_DIST3;
		heap->state[i] = 0;
	}

	memset(heap->state, 0, (instance_spnetpt->nodeNum) * sizeof(char));

	int startStation = ALT_data->landmarkPT[landmarkidx][0];
	vector<vector<int>> startinfo = findStartInfo_ALT(startStation, startTime);
	if (startinfo.size() == 0)
		return;
	insert_val_to_sp_heap(heap, startinfo[0][0], startinfo[0][1], startinfo[0][1], 2, 0);
	heap->distRL[startinfo[0][0]] = startinfo[0][1];
	ALT_data->cost2LandmarkPT[landmarkidx][startStation] = 0;
	for (int i = 1; i < startinfo.size(); i++)
	{
		InsertElement(startinfo[i][1], startinfo[i][0], heap);
	}


	while (heap->size >= 0)
	{
		scan_node_PT_alt(heap, landmarkidx);
	}
}

void scan_node_PT_alt(heap_sch_struct* heap, int landmarkidx)
{
	int	tmp_node;
	int inttmp;
	int tmp_distSV;
	int station;
	int parentstation;

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

			if (tmp_distSV < heap->dist[tmp_node])
			{
				heap->dist[tmp_node] = tmp_distSV;
				heap->distRL[tmp_node] = inttmp;
				
				if (inttmp < ALT_data->cost2LandmarkPT[landmarkidx][station])
					ALT_data->cost2LandmarkPT[landmarkidx][station] = inttmp;

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

vector<vector<int>> findStartInfo_ALT(int startStation, int startTime)
{
	vector<vector<int>> temp_arr;


	for (int i = 0; i < instance_spnetpt->edgeNum; i++)
	{
		vector<int> info;
		int station = instance_spnetpt->edgeFistS[i];
		int time = instance_spnetpt->edgeDisUFist[i];
		int flag = instance_spnetpt->Flag[i];
		if (station == startStation && time - startTime >= 0)
		{
			info.push_back(instance_spnetpt->edgeFist[i]); // start point
			info.push_back(time - startTime); // already past time
			info.push_back(time - startTime); 
			info.push_back(station); // station
			info.push_back(instance_spnetpt->lineNum[i]);
			info.push_back(instance_spnetpt->routeNum[i]);
			temp_arr.push_back(info);
		}
	}
	sort(temp_arr.begin(), temp_arr.end(), sortFunc1);
	return temp_arr;
}

void assignSpeedProfile(int typeOfroad)
{
	speed_profile->numOfroads = typeOfroad;

	speed_profile->speedofwalk = generate_int_arr(speed_profile->numOfroads);
	speed_profile->speedofbike = generate_int_arr(speed_profile->numOfroads);
	speed_profile->speedofcar = generate_int_arr(speed_profile->numOfroads);

	//walk
	speed_profile->speedofwalk[0] = 0;
	speed_profile->speedofwalk[1] = 5;
	speed_profile->speedofwalk[2] = 5;
	speed_profile->speedofwalk[3] = 5;
	speed_profile->speedofwalk[4] = 5;

	//bike
	speed_profile->speedofbike[0] = 0;
	speed_profile->speedofbike[1] = 20;
	speed_profile->speedofbike[2] = 20;
	speed_profile->speedofbike[3] = 18;
	speed_profile->speedofbike[4] = 15;

	//car
	speed_profile->speedofcar[0] = 100;
	speed_profile->speedofcar[1] = 70;
	speed_profile->speedofcar[2] = 50;
	speed_profile->speedofcar[3] = 30;
	speed_profile->speedofcar[4] = 20;
}