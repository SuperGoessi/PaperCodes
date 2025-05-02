#pragma once
#include <vector>
#include "binaryheap.h"
using namespace std;

void assign_stp_ap_init(int startNode, int endNode, int starttime, int timebudget, int maxTransferTime, int approxATime, int densityfactor);

void allocate_space_result_pv0_forward(int num);
void allocate_space_result_pt0_forward(int num);
void allocate_space_result_pt1_forward(int num);
void allocate_space_result_pv1_forward(int num);

void allocate_space_result_pv1_backward(int num);
void allocate_space_result_pt1_backward(int num);
void allocate_space_result_pt0_backward(int num);
void allocate_space_result_pv0_backward(int num);

void init_space_result_pv0_forward(int num);
void init_space_result_pt0_forward(int num);
void init_space_result_pt1_forward(int num);
void init_space_result_pv1_forward(int num);

void init_space_result_pv1_backward(int num);
void init_space_result_pt1_backward(int num);
void init_space_result_pt0_backward(int num);
void init_space_result_pv0_backward(int num);
void delete_allocate_space(int pv0f, int pt0f, int pt1f, int pv1f, int pv1b, int pt1b, int pt0b, int pv0b);
void insert_val_to_sp_heap(heap_sch_struct* heap, int source, int topKey, int srcDist, int srcState, int hpSize);

int get_A_tvl_time(int node1, int node2, int pvflag);
int get_A_tv2_time(int node1, int* nodes, int numofnodes, int pvflag);
int findI_pv(int newIdx);

int get_A_station_time1(int station, int* endStations, int numofendststions);
int get_A_station_time2(int station1, int station2);

vector<vector<int>> findStartInfos(int station, int startTime, int pastCost, int netidx);
vector<vector<int>> findStartInfo(int startStation, int startTime, int pastCost);
vector<vector<int>> findStartInfos_mo(int station, int startTime, int pastCost, int loopidx2);

vector<vector<int>> findEndInfos(int endStation, int endTime, int pastCost, int loopidx2);
vector<vector<int>> findEndInfo(int endStation, int endTime, int pastCost);
vector<vector<int>> findEndInfos_mo(int endStation, int endTime, int pastCost, int loopidx2);

vector<vector<int>> findTransferInfo_forward(int Station, int Time, int pastCost);
vector<vector<int>> findTransferInfo_backward(int Station, int Time, int pastCost);

void delete_pre_selection_space();
void check4activity(vector<vector<int>> arr, int loopidx, int startStation, int numOfaroundActivity, int startTime);