#pragma once
using namespace std;

void init_space_stationSlection();

// step 1: planar PPA
void sp_stp_1_planar(heap_sch_struct* heap_1, heap_sch_struct* heap_2, int pvidx, int psersonidx);
void sp_sToALL_planar(int source, heap_sch_struct* heap);
void scan_node_planar(heap_sch_struct* heap);
void find_planar_space(heap_sch_struct* heap, int pvidx);
int get_time(int p1, int p2, int pvidx);
int get_velocity(int pvidx);
int findStation(int nodeidx);
//step 2
vector<int> interSelection(int pvflag, int psersonidx);

// step 3
void timeAndTrsSelection(const vector<int>& stations, int psersonidx, int pvflag);
// time cost
vector<vector<int>> computeAverageTime();
void assignSimplePTSearchNet(const vector<vector<int>>& data);
void get_sp_index_pre(int a[], int edgeLen);
int findLeastCost(int source, int destination, heap_sch_struct* heap);
void scan_node_flc(heap_sch_struct* heap);

// step 4
void distanceDelete(int pvflag);

// transfer cost
int countRouteNum();
void assignNewIdxRoute();
int findRouteNewIdx(int oldRouteIdx);
vector<vector<int>> findRouteRecord(int oldRouteIdx);
vector<vector<int>> replaceStaNode();
vector<vector<int>> computeGraphdata();
void assignSimplePTTrsSearchNet(const vector<vector<int>>& data);
void get_sp_index_pre_trs(int a[], int edgeLen);
int findTrsLeastCost(int source, int destination, heap_sch_struct* heap);
void scan_node_ftlc(heap_sch_struct* heap, int arr[]);
vector<int> findStartNode(int startStation);
bool transferTime(int s1, int s2);

// in one function
vector<vector<int>> stationPreSelection(int pvflag, int personidx);
vector<vector<int>> stationPreSelection_without(int pvflag, int personidx);

// walk stations
vector<vector<int>> findNerestStations(int numOfStation, int startNode);