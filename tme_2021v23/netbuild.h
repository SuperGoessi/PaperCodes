#pragma once
void assignPVSearchNet();

int findNewIdx(int oldidx);
int findIdx(int newidx);

double costCal(int length, int flag);
void setPVEdgeDistU(int flag);

void get_sp_index(int a[], int edgeLen);
void get_sp_index_reverse(int a[], int edgeLen);
void get_sp_index_pv(int a[], int edgeLen);

void assignPTSearchNet(const vector<vector<int>>& data);

void assignPTSearchNet_reverse(vector<vector<int>>& data);

void assignPVSearchNet();

void assignPVPTConnNet();

void assignPV2Shop();

void assignSta2Shop(int numOfAroundSta);

void assignSta2Park(int numOfAroundSta);

int get_euDist(int x1, int y1, int x2, int y2);

vector<vector<int>> findPVlandmarks(int numOfLandmark, int startnode);

vector<vector<int>> findPTlandmarks(int numOfLandmarks, int startStation);

void allocate_space4ALTdata(int numOfPTmarks, int numOfPVmarks);

void assignALTvalues(int numofPTlandmarks, int startstation, int numofPVlandmarks, int startnode, int pvflag);

void sp_PV_alt_search(heap_sch_struct* heap, int nodeidx);
void scan_node_PV_alt(heap_sch_struct* heap, int nodeidx);

void sp_PT_alt_search(heap_sch_struct* heap, int startStation, int startTime);
void scan_node_PT_alt(heap_sch_struct* heap, int startStation);
vector<vector<int>> findStartInfo_ALT(int startStation, int startTime);

int costCal_pre(int length, int v);
void setPVEdgeDistU_pre(int flag);

void assignSpeedProfile(int typeOfroad);