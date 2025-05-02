#pragma once
#include <vector>
using namespace std;

typedef struct {
	int numOfroads;

	int* speedofwalk;
	int* speedofbike;
	int* speedofcar;
} speed_strcut;

typedef struct {
	int numOfStation;

	int* timeFromA1; 
	int* timeFromA2; // step 1

	int* timeFromStart;
	int* timeFromEnd;
	int* planarPPA_final;
	int* PC4; // step 2

	int* triangle; // step 3
	int* routePerSta;
	int* result;
	int* label;
} pre_PT_slection_struct;

typedef struct {
	int numberOfConn;

	int* startStation;
	int* endStation;
	int* routeNum;
} pre_PT_connection_struct;

typedef struct {
	int numOfRoute;

	int* oldrouteNum;
	int* newrouteNum;
	int* countOfsta; // number of stations in each route with new route index
} pre_PT_conn_computed_struct;

typedef struct {
	int numOfStation; //PT station
	int numOflandmark_PT;

	int numOfNodes; // PV nodes
	int numOflandmark_PV;

	//PT
	int** landmarkPT; // id, x coord, y coord
	int** cost2LandmarkPT; // station num * landmarks

	//PV
	int** landmarkPV; // id, x coord, y coord
	int** cost2LandmarkPV; // node number * landmarks
} ALT_data_struct;

typedef struct {
	int numOfactivity;

	int* activitynodeidx;
	int* activitycoordx;
	int* activitycoordy;
	int* activityOpenTime;
	int* activityCloseTime;

	int** cost;
	int** index;
} activity_data_struct;

typedef struct {
	int staNum;
	int* staIdx;
	int* stacoordx;
	int* stacoordy;
	int* starowidx;
} transfer_station_data_struct;

typedef struct {
	int testDataNum;

	int* idx;
	int* coordx;
	int* coordy;
	int* PC4;
	int* startTime;
	int* timeBudget;
	int* activityTime;
	int* transferTime;
	int* densityFactor;
} test_data_struct;

typedef struct {
	int stationNum;
	int* stationidx; // diff from PT_station_route_struct
	int* stationcoordx;
	int* stationcoordy;
	int* district;
	int* routeNum;
	
	int* isHub; // within author defined range
	int* isPC4; // each PC4 chose one station
	int* isTAZ; // each TAZ chose one station

	int** fourNearestStation; // num of station * 4
	int** distNearestStation; // num of station * 4, one to one as above
} pt_coords_struct;

typedef struct {
	int roadNodeNum;
	int* roadNodeOldIdx;
	int* roadNodeNewIdx;
	int* roadNodecoordx;
	int* roadNodecoordy;
	int* isShop;
} pv_coords_struct;

typedef struct {
	int edgeNum;
	int* edgeFistNode;
	int* edgeListNode;
	int* length; // in meter
	int* roadflag; // 1 for highway, 2 for orange road, 3 for yellow road, 4 for others
	int* cityflag; // 1 for city center, 2 for others
} pv_route_data_struct;

typedef struct {
	int stationRecord;
	int** routesData;
	int* recordNum4Station; // count records for each station
} pt_route_data_struct;

typedef struct {
	int** node2Station; // from one node to several station
	int** station2Node; // from one node to node near station
	int** n2Station;
} pv_pt_conn_struct;

typedef struct {
	int	nodeNum; // sum of stationNodeNum array
	int	edgeNum; // num of all edges

	int* oldIdx;
	int* newIdx;

	int** edgeIndex; // length = nodeNum+1 for PT
	int* edgeList; // end node of each edge
	int* edgeFist; // start point of each edge
	int* edgeDisUFist;
	int* edgeDisUList;
	int* edgeDisU; // cost of each edge
	int* edgeDisUTrs; // cost of each edge for transfer search
	int* edgeDisUWalk; // cost of each edge for walk
	int* edgeLength;

	//int* stationNodeNum; // record for each station, the number of nodes
	int* Flag; // edge type flag
	int* iscity; // flag for city area
	int* parent;
	int* edgeListS; // real station number of the end node
	int* edgeFistS; // real station number of the start node

	int* lineNum;
	int* routeNum;
} sp_net_struct;

typedef struct {
	int startTime;
	int timeBudget;
	int maxTrasferTime;
	int startNode;
	int endNode;
	int pvflag;
	int approxActivityTime;
	int counts;
	double densityfactor; // to adjust network speed for car
	/*
	// PT
	int	startStation;
	int endStation;
	int startTime;
	int timeBudget;
	int maxTrasferTime;

	// road
	int startNode;
	int endNode;
	int startTime_pv;
	int timeBudget_pv;*/
} stp_ap_struct; // to record personlized information

typedef struct {
	int numOfNode;
	int numOfSta;

	int** searched;
	int** parent;
	int** cost;
	int** trscost;

	int** stationcost;
	
} sp_sepresult_struct;

extern stp_ap_struct* instance_stpap;

extern pt_route_data_struct* instance_ptroutedata;

extern pt_coords_struct* instance_ptstationcoords;

extern sp_net_struct* instance_spnetpt;

extern sp_net_struct* instance_spnetpt_reverse;

extern pv_route_data_struct* instance_pvdata;

extern pv_coords_struct* instance_pvcoords;

extern sp_net_struct* instance_spnetpv;

extern pv_pt_conn_struct* instance_pvpt_conn;

extern test_data_struct* test_data;

extern activity_data_struct* shop_data;

extern activity_data_struct* park_data;

extern transfer_station_data_struct* transfer_station_data;

extern ALT_data_struct* ALT_data;

extern sp_net_struct* instance_preptnet;

extern sp_net_struct* instance_preptnet_trs;

extern pre_PT_connection_struct* pre_PT_conn;

extern pre_PT_conn_computed_struct* pre_PT_conn_computed;

extern pre_PT_slection_struct* pre_PT_selection;

extern speed_strcut* speed_profile;

extern sp_sepresult_struct* sp_sepresult_pv0_forward;
extern sp_sepresult_struct* sp_sepresult_pt0_forward;
extern sp_sepresult_struct* sp_sepresult_pt1_forward;
extern sp_sepresult_struct* sp_sepresult_pv1_forward;

extern sp_sepresult_struct* sp_sepresult_pv1_backward;
extern sp_sepresult_struct* sp_sepresult_pt1_backward;
extern sp_sepresult_struct* sp_sepresult_pt0_backward;
extern sp_sepresult_struct* sp_sepresult_pv0_backward;

void allocate_instances();

void init_heaps();

void rest_heaps();

void alloc_space4_pnter_of_instances();

void alloc_space4_pnter_of_instances4result();

int* generate_int_arr(int n);

char* generate_char_arr(int n);

int** generate_int_matrix(int n, int m);

void read_pre_PT_data();

void read_PT_data();

void read_PV_data();

void test_data_pre();

void allocate_space4_test_data();

void read_test_data();

void read_shop_data();

void read_park_data();

void read_transfer_station_data();