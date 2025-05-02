#pragma once
void multimodal_search_checkrunningTime(int home, int destination, int startNode, int startStation, int startTime, int timeBudget, int pvflag1, int loopidx);
vector<vector<int>> multimodal_search(int home, int destination, int startNode, int startStation, int startTime, int timeBudget, int pvflag1, int loopidx);

vector<vector<int>> multimodal_search_diff(int home, int destination, int startNode, int startStation, int endNode, int endStation, int startTime, int timeBudget, int pvflag1, int loopidx); // diff station

void unimodal_search(int home, int destination, int startTime, int pvflag1, int loopidx);

void sp_PV_tbs_1st(heap_sch_struct* heap1, heap_sch_struct* heap2, int startNode, int endNode, int pastCost, int netidx, int pvflag);
void scan_node_PV_1st(heap_sch_struct* heap, int netidx, int endNode, int pvflag);


void sp_PT_tbs_1st(heap_sch_struct* heap_1, heap_sch_struct* heap_2, int startStation, int arriveTime, int pastCost, int loopidx1, int loopidx2);
void scan_node_PT_1st(heap_sch_struct* heap, int loopidx2);


int getNumStation(int netflag);

int** getAroundStation(int nodeidx, int pvflag);

int getSpeed(int pvnetflag);

int getSpeed(int pvflag, int roadflag, int areaflag);

void sp_PT_tbs_1st_mo(heap_sch_struct* heap_1, heap_sch_struct* heap_2, int startStation, int arriveTime, int pastCost, int netidx, int loopidx1, int loopidx2);
void scan_node_PT_1st_mo(heap_sch_struct* heap, int loopidx2);
void sp_PT_tbs_2nd_mo(heap_sch_struct* heap_2, int endstation, int arriveTime, int pastCost, int netidx, int loopidx1, int loopidx2, int loopidx3);
void scan_node_PT_2nd_mo(heap_sch_struct* heap, int loopidx2, int loopidx3);
void sp_PT_tbs_2nd(heap_sch_struct* heap_1, heap_sch_struct* heap_2, int endstation, int arriveTime, int pastCost, int loopidx1, int loopidx2, int loopidx3);
void scan_node_PT_2nd(heap_sch_struct* heap, int loopidx2, int loopidx3);
void sp_PV_tbs_2nd(heap_sch_struct* heap1, heap_sch_struct* heap2, int startNode, int pastCost, int loopidx2);
void scan_node_PV_2nd(heap_sch_struct* heap, heap_sch_struct* heap_opp, int loopidx2);
void sp_PV_tbs_2nd_final(heap_sch_struct* heap1, heap_sch_struct* heap2, int startNode, int pastCost, int loopidx1, int loopidx2, int loopidx3);
void scan_node_PV_2nd_final(heap_sch_struct* heap, int loopidx2, int loopidx3);

void sp_PV_one2all(heap_sch_struct* heap, int startNode);
void scan_node_PV_one2all(heap_sch_struct* heap);