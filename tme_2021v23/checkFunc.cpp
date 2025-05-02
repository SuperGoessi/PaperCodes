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
#include <string>
#include <sstream>
#include <fstream>

#include "inout.h"
#include "checkFunc.h"
#include "timeexpanded.h"
#include "search.h"
#include "binaryheap.h"
#include "netbuild.h"
#include "test.h"
#include "sepsearch.h"
#include "deleteStation.h"

bool checkShopPPA(int shopidx, int stationidx, int earliesttime, int latesttime)
{
	int timeonRoad = shop_data->cost[stationidx][shopidx] * 2;
	int timerange = latesttime - earliesttime;
	if (timerange >= timeonRoad + instance_stpap->approxActivityTime / 10000)
		return true;
	else
		return false;
}

bool checkParkPPA(int parkidx, int stationidx, int earliesttime, int latesttime)
{
	int timeonRoad = park_data->cost[stationidx][parkidx] * 2;
	int timerange = latesttime - earliesttime;
	if (timerange >= timeonRoad + instance_stpap->approxActivityTime / 10000)
		return true;
	else
		return false;
}

std::string fileNameGenerator(string const& str)
{
	string text = "Output";
	text += "_" + str;
	text += ".txt";
	return text;
}

std::string fileNameGenerator(string const& str, int netidx, int loopidx)
{
	string text = to_string(loopidx);
	text += "_Output";
	text += "_" + str;
	text += "_" + to_string(netidx);
	text += ".txt";
	return text;
}

int randomNumGenerator(int lower, int upper)
{
	int num = (rand() % (upper - lower + 1)) + lower;
	return num;
}

void output_data(const vector<vector<int>>& arr, int netflag, int startStation, int startTime, int netidx, int loopidx)
{
	string str;
	// PT: 0-normal forward, 1-normal backward, 2-transfer forward, 3-transfer backward
	// PV: 4-normal forward, 5-normal backward
	if (netflag == 0)
		str = "vector_PT_nf" + std::to_string(startStation) + "_" + std::to_string(startTime) + "_" + std::to_string(netidx) + "_" + std::to_string(loopidx);
	if (netflag == 1)
		str = "vector_PT_nb" + std::to_string(startStation) + "_" + std::to_string(startTime) + "_" + std::to_string(netidx) + "_" + std::to_string(loopidx);
	if (netflag == 2)
		str = "vector_PT_tf" + std::to_string(startStation) + "_" + std::to_string(startTime) + "_" + std::to_string(netidx) + "_" + std::to_string(loopidx);
	if (netflag == 3)
		str = "vector_PT_tb" + std::to_string(startStation) + "_" + std::to_string(startTime) + "_" + std::to_string(netidx) + "_" + std::to_string(loopidx);
	if (netflag == 4)
		str = "vector_PV_nf" + std::to_string(startStation) + "_" + std::to_string(startTime) + "_" + std::to_string(netidx) + "_" + std::to_string(loopidx);
	if (netflag == 5)
		str = "vector_PV_nb" + std::to_string(startStation) + "_" + std::to_string(startTime) + "_" + std::to_string(netidx) + "_" + std::to_string(loopidx);

	string filename = fileNameGenerator(str + "parent_line_route");
	std::ofstream file(filename);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}





void output_vector(const vector<vector<int>>& arr)
{
	string filename = fileNameGenerator("step1_pv");
	std::ofstream file(filename);

	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}

void output_vector_reverse(const vector<vector<int>>& arr)
{
	string filename = fileNameGenerator("step2");
	std::ofstream file(filename);

	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}

void output_idx()
{
	string filename = fileNameGenerator("pt_idx");
	std::ofstream file(filename);

	for (int i = 0; i < instance_spnetpt->nodeNum; i++)
	{
		file << i << " " << instance_spnetpt->edgeIndex[i][0] << " " << instance_spnetpt->edgeIndex[i][1] << endl;
	}
	file.close();
}

void output_idx_reverse()
{
	string filename = fileNameGenerator("pt_idx_reverse");
	std::ofstream file(filename);

	for (int i = 0; i < instance_spnetpt_reverse->nodeNum; i++)
	{
		file << i << " " << instance_spnetpt_reverse->edgeIndex[i][0] << " " << instance_spnetpt_reverse->edgeIndex[i][1] << endl;
	}
	file.close();
}

void output_pv_idx()
{
	string filename = fileNameGenerator("pv_idx");
	std::ofstream file(filename);

	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		file << i << " " << instance_spnetpv->edgeIndex[i][0] << " " << instance_spnetpv->edgeIndex[i][1] << endl;
	}
	file.close();
}

void output_pv_data_all()
{
	string filename = fileNameGenerator("pv_all_data");
	std::ofstream file(filename);

	file << instance_spnetpv->edgeNum << endl;
	for (int i = 0; i < instance_spnetpv->edgeNum; i++)
	{
		file << instance_spnetpv->edgeFist[i] << " " << instance_spnetpv->edgeList[i] << " " << instance_spnetpv->edgeDisU[i] << endl;
	}
	file.close();
}

void output_pv_data_old()
{
	string filename = fileNameGenerator("pv_old_coords_data");
	std::ofstream file(filename);

	file << instance_spnetpv->nodeNum << endl;
	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		file << instance_pvcoords->roadNodeNewIdx[i] << " " << instance_pvcoords->roadNodecoordx[i] << " " << instance_pvcoords->roadNodecoordy[i] << endl;
	}
	file.close();
}


int findold_idx(int newIdx)
{
	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		if (instance_pvcoords->roadNodeNewIdx[i] == newIdx)
			return instance_pvcoords->roadNodeOldIdx[i];
	}
}

int findindex(int oldindex)
{
	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		if (instance_pvcoords->roadNodeOldIdx[i] == oldindex)
			return i;
	}
}

void output_vector(const vector<vector<int>>& arr, int startstation, int endstation, int loopidx)
{
	string str = "step3_" + std::to_string(startstation) + "_" + std::to_string(endstation) + "_" + std::to_string(loopidx) + ".txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}

void output_data_pre_step1_pv(const vector<vector<int>>& arr, int psersonidx, int pvflag)
{
	string str = std::to_string(psersonidx) + "_" + std::to_string(pvflag) + "_pre_step1_pv.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}
void output_data_pre_step1_pt(const vector<vector<int>>& arr, int psersonidx, int pvflag)
{
	string str = std::to_string(psersonidx) + "_" + std::to_string(pvflag) + "_pre_step1_pt.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}
void output_data_pre_step2(const vector<vector<int>>& arr, int psersonidx, int pvflag)
{
	string str = std::to_string(psersonidx) + "_" + std::to_string(pvflag) + "_pre_step2.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}
void output_data_pre_step3(const vector<vector<int>>& arr, int psersonidx, int pvflag)
{

	string str = std::to_string(psersonidx) + "_" + std::to_string(pvflag) + "_pre_step3.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}

void output_data_pre_step3_trianglecompare(const vector<vector<int>>& arr, int psersonidx, int pvflag)
{
	string str = std::to_string(psersonidx) + "_" + std::to_string(pvflag) + "_pre_step3_tricompare.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}

void output_data_pre_step2_distcompare(const vector<vector<int>>& arr, int psersonidx, int pvflag)
{
	string str = std::to_string(psersonidx) + "_" + std::to_string(pvflag) + "_pre_step2_distcompare.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}

void output_data_pre_step4_distcompare(const vector<vector<int>>& arr, int psersonidx, int pvflag)
{
	string str = std::to_string(psersonidx) + "_" + std::to_string(pvflag) + "_pre_step4.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}

void output_data_ptforward(const vector<vector<int>>& arr, int loopidx, int startstation)
{
	string str = std::to_string(loopidx) +"_ptforward_" + std::to_string(startstation) + ".txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}
void output_data_ptbackward(const vector<vector<int>>& arr, int loopidx)
{
	string str =std::to_string(loopidx) + "_ptbackward.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}

void output_final_station(const vector<vector<int>>& arr, int psersonidx, int startStation)
{
	string str = std::to_string(psersonidx) + "_" + std::to_string(startStation) + "_finalstation.txt";
	std::ofstream file(str);
	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
}

void output_data_ppa(int personidx, int startStation, int startTime)
{
	output_data_pv0f(personidx, startStation, startStation, 0, startTime);
	output_data_pt0f(personidx, startStation, startStation, 0, startTime);
	output_data_pt0b(personidx, startStation, startStation, 0, startTime);
	output_data_pv0b(personidx, startStation, startStation, 0, startTime);
}

void output_shop_data()
{
	string str = "time_from_shops2stations.txt";
	std::ofstream file(str);

	for (int i = 0; i < shop_data->numOfactivity; i++)
	{
		for (int j = 0; j < instance_ptstationcoords->stationNum; j++)
		{
			
			file << shop_data->cost[i][j] << " ";

		}
		file << endl;
	}
	file.close();
}

void output_data_shops(const vector<vector<int>>& arr, int personidx, int startStation, int endStation, int startTime)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_" + std::to_string(startTime) + "_shops.txt";
	std::ofstream file(str);

	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}

void output_data_parks(const vector<vector<int>>& arr, int personidx, int startStation, int endStation, int startTime)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_" + std::to_string(startTime) + "_parks.txt";
	std::ofstream file(str);

	for (auto i : arr)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}

void output_data(int personidx, int startStation, int endStation, int pv0fidx, int pt0fidx, int pt1fidx, int pv1fidx, int pv1bidx, int pt1bidx, int pt0bidx, int pv0bidx)
{
	output_data_pv0f(personidx, startStation, endStation, pv0fidx, 0);
	output_data_pt0f(personidx, startStation, endStation, pt0fidx, 0);
	output_data_pt1f(personidx, startStation, endStation, pt1fidx);
	output_data_pv1f(personidx, startStation, endStation, pv1fidx);
	output_data_pv1b(personidx, startStation, endStation, pv1bidx);
	output_data_pt1b(personidx, startStation, endStation, pt1bidx);
	output_data_pt0b(personidx, startStation, endStation, pt0bidx, 0);
	output_data_pv0b(personidx, startStation, endStation, pv0bidx, 0);
}

void output_data_pv0f(int personidx, int startStation, int endStation, int pv0fidx, int startTime)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_" + std::to_string(startTime) + "_pv0_f.txt";
	std::ofstream file(str);
	for (int i = 0; i < sp_sepresult_pv0_forward->numOfNode; i++)
	{
		if (sp_sepresult_pv0_forward->cost[pv0fidx][i] != 2000000)
		{
			file << i << " " << sp_sepresult_pv0_forward->cost[pv0fidx][i] << " " << instance_pvcoords->roadNodecoordx[i] << " " << instance_pvcoords->roadNodecoordy[i] << endl;
		}
	}
	file.close();
}
void output_data_pt0f(int personidx, int startStation, int endStation, int pt0fidx, int startTime)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_" + std::to_string(startTime) + "_pt0_f.txt";
	std::ofstream file(str);
	for (int i = 0; i < sp_sepresult_pt0_forward->numOfSta; i++)
	{
		if (sp_sepresult_pt0_forward->stationcost[pt0fidx][i] != 2000000)
		{
			file << i << " " << sp_sepresult_pt0_forward->stationcost[pt0fidx][i] << " " << instance_ptstationcoords->stationcoordx[i] << " " << instance_ptstationcoords->stationcoordy[i] << endl;
		}
	}
	file.close();
}
void output_data_pt1f(int personidx, int startStation, int endStation, int pt1fidx)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_pt1_f.txt";
	std::ofstream file(str);
	for (int i = 0; i < sp_sepresult_pt1_forward->numOfSta; i++)
	{
		if (sp_sepresult_pt1_forward->stationcost[pt1fidx][i] != 2000000)
		{
			file << i << " " << sp_sepresult_pt1_forward->stationcost[pt1fidx][i] << " " << instance_ptstationcoords->stationcoordx[i] << " " << instance_ptstationcoords->stationcoordy[i] << endl;
		}
	}
	file.close();
}
void output_data_pv1f(int personidx, int startStation, int endStation, int pv1fidx)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_pv1_f.txt";
	std::ofstream file(str);
	for (int i = 0; i < sp_sepresult_pv1_forward->numOfNode; i++)
	{
		if (sp_sepresult_pv1_forward->cost[pv1fidx][i] != 2000000)
		{
			file << i << " " << sp_sepresult_pv1_forward->cost[pv1fidx][i] << " " << instance_pvcoords->roadNodecoordx[i] << " " << instance_pvcoords->roadNodecoordy[i] << endl;
		}
	}
	file.close();
}
void output_data_pv1b(int personidx, int startStation, int endStation, int pv1bidx)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_pv1_b.txt";
	std::ofstream file(str);
	for (int i = 0; i < sp_sepresult_pv1_backward->numOfNode; i++)
	{
		if (sp_sepresult_pv1_backward->cost[pv1bidx][i] != 2000000)
		{
			file << i << " " << sp_sepresult_pv1_backward->cost[pv1bidx][i] << " " << instance_pvcoords->roadNodecoordx[i] << " " << instance_pvcoords->roadNodecoordy[i] << endl;
		}
	}
	file.close();
}
void output_data_pt1b(int personidx, int startStation, int endStation, int pt1bidx)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_pt1_b.txt";
	std::ofstream file(str);
	for (int i = 0; i < sp_sepresult_pt1_backward->numOfSta; i++)
	{
		if (sp_sepresult_pt1_backward->stationcost[pt1bidx][i] != 2000000)
		{
			file << i << " " << sp_sepresult_pt1_backward->stationcost[pt1bidx][i] << " " << instance_ptstationcoords->stationcoordx[i] << " " << instance_ptstationcoords->stationcoordy[i] << endl;
		}
	}
	file.close();
}
void output_data_pt0b(int personidx, int startStation, int endStation, int pt0bidx, int startTime)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_" + std::to_string(startTime) + "_pt0_b.txt";
	std::ofstream file(str);
	for (int i = 0; i < sp_sepresult_pt0_backward->numOfSta; i++)
	{
		if (sp_sepresult_pt0_backward->stationcost[pt0bidx][i] != 2000000)
		{
			file << i << " " << sp_sepresult_pt0_backward->stationcost[pt0bidx][i] << " " << instance_ptstationcoords->stationcoordx[i] << " " << instance_ptstationcoords->stationcoordy[i] << endl;
		}
	}
	file.close();
}
void output_data_pv0b(int personidx, int startStation, int endStation, int pv0bidx, int startTime)
{
	string str = std::to_string(personidx) + "_" + std::to_string(startStation) + "_" + std::to_string(endStation) + "_" + std::to_string(startTime) + "_pv0_b.txt";
	std::ofstream file(str);
	for (int i = 0; i < sp_sepresult_pv0_backward->numOfNode; i++)
	{
		if (sp_sepresult_pv0_backward->cost[pv0bidx][i] != 2000000)
		{
			file << i << " " << sp_sepresult_pv0_backward->cost[pv0bidx][i] << " " << instance_pvcoords->roadNodecoordx[i] << " " << instance_pvcoords->roadNodecoordy[i] << endl;
		}
	}
	file.close();
}

vector<vector<int>> check_shop_single_v(int personidx, int pvfalg)
{
	int timeBudget = instance_stpap->timeBudget / 10000;
	int activityTime = instance_stpap->approxActivityTime / 10000;
	int startTime = instance_stpap->startTime;
	vector<vector<int>> output_single_shops;

	int parkingtime = 0;
	int pikingtime = 0;

	if (pvfalg == 2)
		parkingtime = 5;
	if (pvfalg == 1)
		parkingtime = 2;

	if (pvfalg == 2)
		pikingtime = 3;
	if (pvfalg == 1)
		pikingtime = 1;

	for (int i = 0; i < shop_data->numOfactivity; i++)
	{
		int idx = findNewIdx(shop_data->activitynodeidx[i]);
		if (sp_PV_heap_single_F->distRL[idx] != 2000000 && sp_PV_heap_single_B->distRL[idx] != 2000000)
		{
			int time1 = sp_PV_heap_single_F->distRL[idx] / 10000;
			int time2 = sp_PV_heap_single_B->distRL[idx] / 10000;

			if (parkingtime + pikingtime + activityTime + time1 + time2 <= timeBudget)
			{
				vector<int> arr;
				arr.push_back(i);
				arr.push_back(shop_data->activitycoordx[i]);
				arr.push_back(shop_data->activitycoordy[i]);
				arr.push_back(startTime + time1 + parkingtime);
				arr.push_back(startTime + timeBudget - time2 - pikingtime);
				output_single_shops.push_back(arr);
			}
		}
	}

	return output_single_shops;
}

void check_shop_single(int personidx, int pvfalg)
{
	int timeBudget = instance_stpap->timeBudget/10000;
	int activityTime = instance_stpap->approxActivityTime/10000;
	int startTime = instance_stpap->startTime;
	vector<vector<int>> output_single_shops;

	int parkingtime = 0;
	int pikingtime = 0;

	if (pvfalg == 2)
		parkingtime = 5;
	if (pvfalg == 1)
		parkingtime = 2;

	if (pvfalg == 2)
		pikingtime = 3;
	if (pvfalg == 1)
		pikingtime = 1;

	for (int i = 0; i < shop_data->numOfactivity; i++)
	{
		int idx = findNewIdx(shop_data->activitynodeidx[i]);
		if (sp_PV_heap_single_F->distRL[idx] != 2000000 && sp_PV_heap_single_B->distRL[idx] != 2000000)
		{
			int time1 = sp_PV_heap_single_F->distRL[idx] / 10000;
			int time2 = sp_PV_heap_single_B->distRL[idx] / 10000;

			if (parkingtime + pikingtime + activityTime + time1 + time2 <= timeBudget)
			{
				vector<int> arr;
				arr.push_back(i);
				arr.push_back(shop_data->activitycoordx[i]);
				arr.push_back(shop_data->activitycoordy[i]);
				arr.push_back(startTime + time1 + parkingtime);
				arr.push_back(startTime + timeBudget - time2 - pikingtime);
				output_single_shops.push_back(arr);
			}
		}
	}

	string str =  std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_single_shop.txt";
	std::ofstream file(str);
	for (auto i : output_single_shops)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}
void check_park_single(int personidx, int pvfalg)
{
	int timeBudget = instance_stpap->timeBudget / 10000;
	int activityTime = instance_stpap->approxActivityTime / 10000;
	int startTime = instance_stpap->startTime;
	vector<vector<int>> output_single_park;

	int parkingtime = 0;
	int pikingtime = 0;

	if (pvfalg == 2)
		parkingtime = 5;
	if (pvfalg == 1)
		parkingtime = 2;

	if (pvfalg == 2)
		pikingtime = 3;
	if (pvfalg == 1)
		pikingtime = 1;

	for (int i = 0; i < park_data->numOfactivity; i++)
	{
		int idx = findNewIdx(park_data->activitynodeidx[i]);
		if (sp_PV_heap_single_F->distRL[idx] != 2000000 && sp_PV_heap_single_B->distRL[idx] != 2000000)
		{
			int time1 = sp_PV_heap_single_F->distRL[idx] / 10000;
			int time2 = sp_PV_heap_single_B->distRL[idx] / 10000;

			if (activityTime + time1 + time2 <= timeBudget)
			{
				vector<int> arr;
				arr.push_back(i);
				arr.push_back(park_data->activitycoordx[i]);
				arr.push_back(park_data->activitycoordy[i]);
				arr.push_back(startTime + time1 + parkingtime);
				arr.push_back(startTime + timeBudget - time2 - pikingtime);
				output_single_park.push_back(arr);
			}
		}
	}

	string str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_single_park.txt";
	std::ofstream file(str);
	for (auto i : output_single_park)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}

vector<vector<int>> check_shop_v(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg)
{
	vector<vector<int>> shops;

	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{

			if (checkShopPPA(j, arr[i][1], arr[i][2], arr[i][3]))
			{
				int idx = shop_data->index[arr[i][1]][j];
				int timeonRoad = shop_data->cost[arr[i][1]][j];

				vector<int> shop;
				shop.push_back(idx);
				shop.push_back(shop_data->activitycoordx[idx]);
				shop.push_back(shop_data->activitycoordy[idx]);
				shop.push_back(arr[i][0]);
				shop.push_back(arr[i][1]);
				shop.push_back(arr[i][2] + timeonRoad);
				shop.push_back(arr[i][3] - timeonRoad);
				shops.push_back(shop);
			}
		}
	}

	return shops;
}

void check_shop(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg)
{
	vector<vector<int>> shops;

	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{

			if (checkShopPPA(j, arr[i][1], arr[i][2], arr[i][3]))
			{
				int idx = shop_data->index[arr[i][1]][j];
				int timeonRoad = shop_data->cost[arr[i][1]][j];

				vector<int> shop;
				shop.push_back(idx);
				shop.push_back(shop_data->activitycoordx[idx]);
				shop.push_back(shop_data->activitycoordy[idx]);
				shop.push_back(arr[i][0]);
				shop.push_back(arr[i][1]);
				shop.push_back(arr[i][2] + timeonRoad);
				shop.push_back(arr[i][3] - timeonRoad);
				shops.push_back(shop);
			}
		}
	}

	string str;
	if(typeidx==0)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_without_shop.txt";
	if(typeidx==1)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_with_shop.txt";
	if (typeidx==2)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_walk_shop.txt";

	std::ofstream file(str);
	for (auto i : shops)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}
void check_park(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg)
{
	vector<vector<int>> parks;

	for (int i = 0; i < arr.size(); i++)
	{
		for (int k = 0; k < 4; k++)
		{

			if (checkParkPPA(k, arr[i][1], arr[i][2], arr[i][3]))
			{
				int idx = park_data->index[arr[i][1]][k];
				int timeonRoad = park_data->cost[arr[i][1]][k];

				vector<int> park;
				park.push_back(idx);
				park.push_back(park_data->activitycoordx[idx]);
				park.push_back(park_data->activitycoordy[idx]);
				park.push_back(arr[i][0]);
				park.push_back(arr[i][1]);
				park.push_back(arr[i][2] + timeonRoad);
				park.push_back(arr[i][3] - timeonRoad);
				parks.push_back(park);
			}
		}
	}

	string str;
	if (typeidx == 0)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_without_park.txt";
	if (typeidx == 1)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_with_park.txt";
	if (typeidx == 2)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_walk_park.txt";

	std::ofstream file(str);
	for (auto i : parks)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}



vector<vector<int>> check_shop_diff_v(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg)
{
	vector<vector<int>> shops;

	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{

			if (checkShopPPA(j, arr[i][2], arr[i][3], arr[i][4]))
			{
				int idx = shop_data->index[arr[i][2]][j];
				int timeonRoad = shop_data->cost[arr[i][2]][j];

				vector<int> shop;
				shop.push_back(idx);
				shop.push_back(shop_data->activitycoordx[idx]);
				shop.push_back(shop_data->activitycoordy[idx]);
				shop.push_back(arr[i][0]);
				shop.push_back(arr[i][1]);
				shop.push_back(arr[i][2]);
				shop.push_back(arr[i][3] + timeonRoad);
				shop.push_back(arr[i][4] - timeonRoad);
				shops.push_back(shop);
			}
		}
	}

	return shops;
}

void check_shop_diff(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg)
{
	vector<vector<int>> shops;

	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{

			if (checkShopPPA(j, arr[i][2], arr[i][3], arr[i][4]))
			{
				int idx = shop_data->index[arr[i][2]][j];
				int timeonRoad = shop_data->cost[arr[i][2]][j];

				vector<int> shop;
				shop.push_back(idx);
				shop.push_back(shop_data->activitycoordx[idx]);
				shop.push_back(shop_data->activitycoordy[idx]);
				shop.push_back(arr[i][0]);
				shop.push_back(arr[i][1]);
				shop.push_back(arr[i][2]);
				shop.push_back(arr[i][3] + timeonRoad);
				shop.push_back(arr[i][4] - timeonRoad);
				shops.push_back(shop);
			}
		}
	}

	string str;
	if (typeidx == 0)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_without_shop.txt";
	if (typeidx == 1)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_with_shop.txt";
	if (typeidx == 2)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_walk_shop.txt";

	std::ofstream file(str);
	for (auto i : shops)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}

void check_park_diff(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg)
{
	vector<vector<int>> parks;

	for (int i = 0; i < arr.size(); i++)
	{
		for (int k = 0; k < 4; k++)
		{

			if (checkParkPPA(k, arr[i][2], arr[i][3], arr[i][4]))
			{
				int idx = park_data->index[arr[i][2]][k];
				int timeonRoad = park_data->cost[arr[i][2]][k];

				vector<int> park;
				park.push_back(idx);
				park.push_back(park_data->activitycoordx[idx]);
				park.push_back(park_data->activitycoordy[idx]);
				park.push_back(arr[i][0]);
				park.push_back(arr[i][1]);
				park.push_back(arr[i][2]);
				park.push_back(arr[i][3] + timeonRoad);
				park.push_back(arr[i][4] - timeonRoad);
				parks.push_back(park);
			}
		}
	}

	string str;
	if (typeidx == 0)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_without_park.txt";
	if (typeidx == 1)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_with_park.txt";
	if (typeidx == 2)
		str = std::to_string(personidx) + "_" + std::to_string(pvfalg) + "_walk_park.txt";

	std::ofstream file(str);
	for (auto i : parks)
	{
		for (auto j : i)
		{
			file << j << " ";
		}
		file << endl;
	}
	file.close();
}

void combineResult(vector<vector<int>>& singleModeResult, vector<vector<int>>& multiModeResult, vector<vector<int>>& simplifiedResult)
{
	for (auto result : singleModeResult)
	{
		int id = result[0];
		int time = result[4] - result[3];

		vector<int> temp1;
		temp1.push_back(id);
		temp1.push_back(time - instance_stpap->approxActivityTime/10000);
		simplifiedResult.push_back(temp1);
	}

	
	sort(multiModeResult.begin(), multiModeResult.end(), sortFunc_03);
	vector<vector<int>> idwithTime = findMaxTimeRange(findTimes4Shop(multiModeResult));

	simplifiedResult.reserve(simplifiedResult.size() + idwithTime.size());
	simplifiedResult.insert(simplifiedResult.end(), idwithTime.begin(), idwithTime.end());
	idwithTime.clear();

}

vector<vector<int>> findTimes4Shop(const vector<vector<int>>& multiModeResult)
{
	int idx = multiModeResult[0][0];
	int startStation = multiModeResult[0][3];
	int time1 = multiModeResult[0][5];
	int time2 = multiModeResult[0][6];

	vector<vector<int>> result;
	for (int i = 0; i < multiModeResult.size(); i++)
	{
		if (idx == multiModeResult[i][0] && startStation == multiModeResult[i][3])
		{
			int sTime = multiModeResult[i][5];
			int eTime = multiModeResult[i][6];

			if (time1 > sTime)
				time1 = sTime;

			if (time2 < eTime)
				time2 = eTime;
		}
		else
		{
			vector<int> temp;
			temp.push_back(idx);
			temp.push_back(time1);
			temp.push_back(time2);
			result.push_back(temp);

			idx = multiModeResult[i][0];
			startStation = multiModeResult[i][3];
			time1 = multiModeResult[i][5];
			time2 = multiModeResult[i][6];
		}
	}

	vector<int> temp;
	temp.push_back(idx);
	temp.push_back(time1);
	temp.push_back(time2);
	result.push_back(temp);

	return result;
}

vector<vector<int>> findMaxTimeRange(const vector<vector<int>>& arr)
{
	vector<vector<int>> result;

	int idx = arr[0][0];
	int time1 = arr[0][1];
	int time2 = arr[0][2];
	int timerange = time2 - time1;

	for (int i = 0; i < arr.size(); i++)
	{
		if (idx == arr[i][0])
		{
			int time = arr[i][2] - arr[i][1];
			if (time > timerange)
			{
				time1 = arr[i][1];
				time2 = arr[i][2];
				timerange = time;
			}

		}
		else
		{
			vector<int> re;
			re.push_back(idx);
			re.push_back(time1);
			re.push_back(time2);
			result.push_back(re);

			idx = arr[i][0];
			time1 = arr[i][1];
			time2 = arr[i][2];
			timerange = time2 - time1;
		}

	}

	vector<int> re;
	re.push_back(idx);
	re.push_back(time1);
	re.push_back(time2);
	result.push_back(re);


	// id with time
	vector<vector<int>> final_result;
	for (auto i : result)
	{
		vector<int> temp;
		temp.push_back(i[0]);
		temp.push_back(i[2] - i[1] - instance_stpap->approxActivityTime/10000);
		final_result.push_back(temp);
	}

	return final_result;
}

vector<vector<int>> Result_walk(const vector<vector<int>>& walkModeResult)
{
	int id = walkModeResult[0][0];
	int ss = walkModeResult[0][3];
	int es = walkModeResult[0][4];
	int time1 = walkModeResult[0][6];
	int time2 = walkModeResult[0][7];

	// find max time range for shops
	vector<vector<int>> result;
	for (int i = 0; i < walkModeResult.size(); i++)
	{
		if (id == walkModeResult[i][0] && ss == walkModeResult[i][3] && es == walkModeResult[i][4])
		{
			int sTime = walkModeResult[i][6];
			int eTime = walkModeResult[i][7];

			if (time1 > sTime)
				time1 = sTime;

			if (time2 < eTime)
				time2 = eTime;
		}
		else
		{
			vector<int> temp;
			temp.push_back(id);
			temp.push_back(time2 - time1 -instance_stpap->approxActivityTime / 10000);
			result.push_back(temp);

			id = walkModeResult[i][0];
			ss = walkModeResult[i][3];
			es = walkModeResult[i][4];
			time1 = walkModeResult[i][6]; 
			time2 = walkModeResult[i][7];
		}
	}

	vector<int> temp;
	temp.push_back(id);
	temp.push_back(time2-time1-instance_stpap->approxActivityTime/10000);
	result.push_back(temp);

	return result;
}

vector<vector<int>> deleteDuplicate(const vector<vector<int>>& arr)
{
	vector<vector<int>> result;
	int id = arr[0][0];
	int time = arr[0][1];

	for (int i = 1; i < arr.size(); i++)
	{
		int idx = arr[i][0];
		int tempTime = arr[i][1];
		if (idx == id)
		{
			time = tempTime;
		}
		else
		{
			vector<int> re;
			re.push_back(id);
			re.push_back(time);
			result.push_back(re);

			id = idx;
			time = tempTime;
		}
	}
	vector<int> re;
	re.push_back(id);
	re.push_back(time);
	result.push_back(re);

	return result;

}

vector<int> sumup(const vector<int>& walk, const vector<vector<int>>& bike, const vector<vector<int>>& car, int personidx)
{

	int size_bike = bike.size();
	int size_car = car.size();

	int count_bike = 0;
	int count_car = 0;


	for (int i = 0; i < size_bike; i++)
	{
		count_bike += bike[i][1];
	}
	for (int j = 0; j < size_car; j++)
	{
		count_car += car[j][1];
	}

	vector<int> temp;
	temp.push_back(personidx);
	temp.push_back(walk[0]);
	temp.push_back(walk[1]);
	temp.push_back(size_bike);
	temp.push_back(count_bike);
	temp.push_back(size_car);
	temp.push_back(count_car);
	return temp;
}

vector<int> get_walksta(const vector<vector<int>>& walkmode)
{
	vector<int> walk_sta;

	if (walkmode.size() != 0)
	{
		vector<vector<int>> simplifiedResult_walk = Result_walk(walkmode);
		sort(simplifiedResult_walk.begin(), simplifiedResult_walk.end(), sortFunc_01);
		vector<vector<int>> simplifiedResult_walk_final = deleteDuplicate(simplifiedResult_walk);

		int size_walk = simplifiedResult_walk_final.size();
		int count_walk = 0;
		for (int i = 0; i < size_walk; i++)
		{
			count_walk += simplifiedResult_walk_final[i][1];
		}

		walk_sta.push_back(size_walk);
		walk_sta.push_back(count_walk);
	}

	

	return walk_sta;
}