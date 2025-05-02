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
pt_route_data_struct* instance_ptroutedata;

pt_coords_struct* instance_ptstationcoords;

sp_net_struct* instance_spnetpt;

sp_net_struct* instance_spnetpt_reverse;

stp_ap_struct* instance_stpap;

pv_route_data_struct* instance_pvdata;

pv_coords_struct* instance_pvcoords;

sp_net_struct* instance_spnetpv;

pv_pt_conn_struct* instance_pvpt_conn;

test_data_struct* test_data;

activity_data_struct* shop_data;

activity_data_struct* park_data;

transfer_station_data_struct* transfer_station_data;

ALT_data_struct* ALT_data;

sp_net_struct* instance_preptnet;

sp_net_struct* instance_preptnet_trs;

pre_PT_connection_struct* pre_PT_conn;

pre_PT_conn_computed_struct* pre_PT_conn_computed;

pre_PT_slection_struct* pre_PT_selection;

speed_strcut* speed_profile;

sp_sepresult_struct* sp_sepresult_pv0_forward;
sp_sepresult_struct* sp_sepresult_pt0_forward;
sp_sepresult_struct* sp_sepresult_pt1_forward;
sp_sepresult_struct* sp_sepresult_pv1_forward;

sp_sepresult_struct* sp_sepresult_pv1_backward;
sp_sepresult_struct* sp_sepresult_pt1_backward;
sp_sepresult_struct* sp_sepresult_pt0_backward;
sp_sepresult_struct* sp_sepresult_pv0_backward;

void test_data_pre()
{
	allocate_space4_test_data();
	read_test_data();
	read_shop_data();
	read_park_data();
	//read_transfer_station_data();
}

void alloc_space4_pnter_of_instances4result()
{
	sp_sepresult_pv0_forward = (sp_sepresult_struct*)malloc(sizeof(sp_sepresult_struct));
	if (sp_sepresult_pv0_forward == NULL)
	{
		printf("Out of space: sp_sepresult_pv0_forward\n");
		getchar();
		exit(1);
	}

	sp_sepresult_pt0_forward = (sp_sepresult_struct*)malloc(sizeof(sp_sepresult_struct));
	if (sp_sepresult_pt0_forward == NULL)
	{
		printf("Out of space: sp_sepresult_pt0_forward\n");
		getchar();
		exit(1);
	}

	sp_sepresult_pt1_forward = (sp_sepresult_struct*)malloc(sizeof(sp_sepresult_struct));
	if (sp_sepresult_pt1_forward == NULL)
	{
		printf("Out of space: sp_sepresult_pt1_forward\n");
		getchar();
		exit(1);
	}

	sp_sepresult_pv1_forward = (sp_sepresult_struct*)malloc(sizeof(sp_sepresult_struct));
	if (sp_sepresult_pv1_forward == NULL)
	{
		printf("Out of space: sp_sepresult_pv1_forward\n");
		getchar();
		exit(1);
	}

	sp_sepresult_pv1_backward = (sp_sepresult_struct*)malloc(sizeof(sp_sepresult_struct));
	if (sp_sepresult_pv1_backward == NULL)
	{
		printf("Out of space: sp_sepresult_pv1_backward\n");
		getchar();
		exit(1);
	}

	sp_sepresult_pt1_backward = (sp_sepresult_struct*)malloc(sizeof(sp_sepresult_struct));
	if (sp_sepresult_pt1_backward == NULL)
	{
		printf("Out of space: sp_sepresult_pt1_backward\n");
		getchar();
		exit(1);
	}

	sp_sepresult_pt0_backward = (sp_sepresult_struct*)malloc(sizeof(sp_sepresult_struct));
	if (sp_sepresult_pt0_backward == NULL)
	{
		printf("Out of space: sp_sepresult_pt0_backward\n");
		getchar();
		exit(1);
	}

	sp_sepresult_pv0_backward = (sp_sepresult_struct*)malloc(sizeof(sp_sepresult_struct));
	if (sp_sepresult_pv0_backward == NULL)
	{
		printf("Out of space: sp_sepresult_pv0_backward\n");
		getchar();
		exit(1);
	}

}

void allocate_space4_test_data()
{
	test_data = (test_data_struct*)malloc(sizeof(test_data_struct));
	if (test_data == NULL)
	{
		printf("Out of space: test data\n");
		getchar();
		exit(1);
	}

	shop_data = (activity_data_struct*)malloc(sizeof(activity_data_struct));
	if (shop_data == NULL)
	{
		printf("Out of space: shop data\n");
		getchar();
		exit(1);
	}

	park_data = (activity_data_struct*)malloc(sizeof(activity_data_struct));
	if (park_data == NULL)
	{
		printf("Out of space: park data\n");
		getchar();
		exit(1);
	}

	transfer_station_data = (transfer_station_data_struct*)malloc(sizeof(transfer_station_data_struct));
	if (transfer_station_data == NULL)
	{
		printf("Out of space: transfer_station_data\n");
		getchar();
		exit(1);
	}
}

void read_test_data()
{
	FILE* graphPtr;
	errno_t		err;

	if ((err = fopen_s(&graphPtr, "test_population.txt", "r")) != 0)
	{
		printf("test_population.txt was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(test_data->testDataNum));
	test_data->idx = generate_int_arr(test_data->testDataNum);
	test_data->coordx = generate_int_arr(test_data->testDataNum);
	test_data->coordy = generate_int_arr(test_data->testDataNum);
	test_data->PC4 = generate_int_arr(test_data->testDataNum);
	test_data->startTime = generate_int_arr(test_data->testDataNum);
	test_data->timeBudget = generate_int_arr(test_data->testDataNum);
	test_data->activityTime = generate_int_arr(test_data->testDataNum);
	test_data->transferTime = generate_int_arr(test_data->testDataNum);
	test_data->densityFactor = generate_int_arr(test_data->testDataNum);

	// read station index, x coordinate, y coordinate
	for (int i = 0; i < test_data->testDataNum; i++)
	{
		fscanf_s(graphPtr, "%d %d %d %d %d %d %d %d %d", &(test_data->idx[i]), &(test_data->coordx[i]), &(test_data->coordy[i]), &(test_data->PC4[i]), &(test_data->startTime[i]), &(test_data->timeBudget[i]), &(test_data->activityTime[i]), &(test_data->transferTime[i]), &(test_data->densityFactor[i]));
	}
	fclose(graphPtr);

	printf("Finish reading test data!\n");
}

void allocate_instances()
{
	alloc_space4_pnter_of_instances();
	alloc_space4_pnter_of_instances4result();
	read_PT_data();
	read_pre_PT_data();
	read_PV_data();
	assigndist2Station(); // for walk from station to station
}

void init_heaps()
{
	sp_PT_heap_F = Init_heap(instance_spnetpt->nodeNum, 1);
	sp_PT_heap_B = Init_heap(instance_spnetpt->nodeNum, 1);
	sp_PV1_heap_F = Init_heap(instance_spnetpv->nodeNum, 1);
	sp_PV1_heap_B = Init_heap(instance_spnetpv->nodeNum, 1);

	sp_PV_heap_pre_original = Init_heap(instance_spnetpv->nodeNum, 1);
	sp_PV_heap_pre_planar_1 = Init_heap(instance_pvcoords->roadNodeNum, 1);
	sp_PV_heap_pre_planar_2 = Init_heap(instance_pvcoords->roadNodeNum, 1);
	sp_PT_heap_pre_time = Init_heap(instance_ptstationcoords->stationNum, 1);

	sp_PV_heap_single_F = Init_heap(instance_spnetpv->nodeNum, 1);
	sp_PV_heap_single_B = Init_heap(instance_spnetpv->nodeNum, 1);
	
	cout << "Finish heaps initilization" << endl;
}

void rest_heaps()
{
	resetHeap(instance_spnetpt->nodeNum, sp_PT_heap_F, 1);
	resetHeap(instance_spnetpt->nodeNum, sp_PT_heap_B, 1);

	resetHeap(instance_spnetpv->nodeNum, sp_PV1_heap_F, 1);
	resetHeap(instance_spnetpv->nodeNum, sp_PV1_heap_B, 1);
}

int* generate_int_arr(int n)
{
	int* new_array;

	if ((new_array = (int*)malloc(sizeof(int) * n)) == NULL) {
		getchar();
		printf_s("Out of memory, exit.");
		exit(1);
	}
	return new_array;
}

double* generate_double_arr(int n)
{
	double* new_array;

	if ((new_array = (double*)malloc(sizeof(double) * n)) == NULL) {
		getchar();
		printf_s("Out of memory, exit.");
		exit(1);
	}
	return new_array;
}

char* generate_char_arr(int n)
{
	char* new_array;
	if ((new_array = (char*)malloc(sizeof(char) * n)) == NULL)
	{
		printf_s("Out of memory, exit.");
		getchar();
		exit(1);
	}
	return new_array;
}

int** generate_int_matrix(int n, int m) // n is the number of rows, m is the number of columns
{
	int** new_array = new int* [n];
	for (int i = 0; i < n; ++i)
	{
		new_array[i] = new int[m];
	}

	return new_array;
}

void alloc_space4_pnter_of_instances()
{
	instance_pvpt_conn = (pv_pt_conn_struct*)malloc(sizeof(pv_pt_conn_struct));
	if (instance_pvpt_conn == NULL)
	{
		printf("Out of space: instance_pvpt_conn\n");
		getchar();
		exit(1);
	}

	instance_ptstationcoords = (pt_coords_struct*)malloc(sizeof(pt_coords_struct));
	if (instance_ptstationcoords == NULL)
	{
		printf("Out of space: instance_ptstation_coords\n");
		getchar();
		exit(1);
	}

	instance_ptroutedata = (pt_route_data_struct*)malloc(sizeof(pt_route_data_struct));
	if (instance_ptroutedata == NULL)
	{
		printf("Out of space: instance_ptroute_data\n");
		getchar();
		exit(1);
	}

	instance_spnetpt = (sp_net_struct*)malloc(sizeof(sp_net_struct));
	if (instance_spnetpt == NULL)
	{
		printf("Out of space: instance_sp_net_pt\n");
		getchar();
		exit(1);
	}

	instance_spnetpt_reverse = (sp_net_struct*)malloc(sizeof(sp_net_struct));
	if (instance_spnetpt_reverse == NULL)
	{
		printf("Out of space: instance_sp_net_pt_reverse\n");
		getchar();
		exit(1);
	}

	instance_stpap = (stp_ap_struct*)malloc(sizeof(stp_ap_struct));
	if (instance_stpap == NULL)
	{
		printf("Out of space: instance_stp_ap\n");
		getchar();
		exit(1);
	}

	instance_pvdata = (pv_route_data_struct*)malloc(sizeof(pv_route_data_struct));
	if (instance_pvdata == NULL)
	{
		printf("Out of space: instance_pvdata\n");
		getchar();
		exit(1);
	}

	instance_pvcoords = (pv_coords_struct*)malloc(sizeof(pv_coords_struct));
	if (instance_pvcoords == NULL)
	{
		printf("Out of space: instance_pvcoords\n");
		getchar();
		exit(1);
	}

	instance_spnetpv = (sp_net_struct*)malloc(sizeof(sp_net_struct));
	if (instance_spnetpv == NULL)
	{
		printf("Out of space: instance_spnetpv\n");
		getchar();
		exit(1);
	}

	ALT_data = (ALT_data_struct*)malloc(sizeof(ALT_data_struct));
	if (ALT_data == NULL)
	{
		printf("Out of space: ALT_data\n");
		getchar();
		exit(1);
	}

	instance_preptnet = (sp_net_struct*)malloc(sizeof(sp_net_struct));
	if (instance_preptnet == NULL)
	{
		printf("Out of space: instance_preptnet\n");
		getchar();
		exit(1);
	}

	instance_preptnet_trs = (sp_net_struct*)malloc(sizeof(sp_net_struct));
	if (instance_preptnet_trs == NULL)
	{
		printf("Out of space: instance_preptnet\n");
		getchar();
		exit(1);
	}

	pre_PT_conn = (pre_PT_connection_struct*)malloc(sizeof(pre_PT_connection_struct));
	if (pre_PT_conn == NULL)
	{
		printf("out of space: pre_PT_conn\n");
		getchar();
		exit(1);
	}

	pre_PT_conn_computed = (pre_PT_conn_computed_struct*)malloc(sizeof(pre_PT_conn_computed_struct));
	if (pre_PT_conn_computed == NULL)
	{
		printf("out of space: pre_PT_conn_computed\n");
		getchar();
		exit(1);
	}

	pre_PT_selection = (pre_PT_slection_struct*)malloc(sizeof(pre_PT_slection_struct));
	if (pre_PT_selection == NULL)
	{
		printf("out of space: pre_PT_selection\n");
		getchar();
		exit(1);
	}

	speed_profile = (speed_strcut*)malloc(sizeof(speed_strcut));
	if (speed_profile == NULL)
	{
		printf("out of space: speed_profile\n");
		getchar();
		exit(1);
	}

}

void read_pre_PT_data()
{
	FILE* graphPtr;
	errno_t		err;

	if ((err = fopen_s(&graphPtr, "test_pre_PT_connection.txt", "r")) != 0)
	{
		printf("test_pre_PT_connection.txt was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(pre_PT_conn->numberOfConn));

	pre_PT_conn->startStation = generate_int_arr(pre_PT_conn->numberOfConn);
	pre_PT_conn->endStation = generate_int_arr(pre_PT_conn->numberOfConn);
	pre_PT_conn->routeNum = generate_int_arr(pre_PT_conn->numberOfConn);

	for (int i = 0; i < pre_PT_conn->numberOfConn; i++)
	{
		fscanf_s(graphPtr, "%d %d %d", &(pre_PT_conn->startStation[i]), &(pre_PT_conn->endStation[i]), &(pre_PT_conn->routeNum[i]));
	}
	fclose(graphPtr);
}

void read_PT_data()
{
	FILE* graphPtr;
	errno_t		err;

	if ((err = fopen_s(&graphPtr, "test_basic_stationsv5.txt", "r")) != 0)
	{
		printf("test_basic_stations.dat was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(instance_ptstationcoords->stationNum));	//read stop/station numbers

	instance_ptstationcoords->stationidx = generate_int_arr(instance_ptstationcoords->stationNum);
	instance_ptstationcoords->stationcoordx = generate_int_arr(instance_ptstationcoords->stationNum);
	instance_ptstationcoords->stationcoordy = generate_int_arr(instance_ptstationcoords->stationNum);
	instance_ptstationcoords->routeNum = generate_int_arr(instance_ptstationcoords->stationNum);

	instance_ptstationcoords->isHub = generate_int_arr(instance_ptstationcoords->stationNum);
	instance_ptstationcoords->isPC4 = generate_int_arr(instance_ptstationcoords->stationNum);
	instance_ptstationcoords->isTAZ = generate_int_arr(instance_ptstationcoords->stationNum);
	
	instance_ptstationcoords->fourNearestStation = generate_int_matrix(instance_ptstationcoords->stationNum, 4);
	instance_ptstationcoords->distNearestStation = generate_int_matrix(instance_ptstationcoords->stationNum, 4);

	// read station index, x coordinate, y coordinate
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		fscanf_s(graphPtr, "%d %d %d %d %d %d %d", &(instance_ptstationcoords->stationidx[i]), &(instance_ptstationcoords->stationcoordx[i]), &(instance_ptstationcoords->stationcoordy[i]), &(instance_ptstationcoords->routeNum[i]), &(instance_ptstationcoords->isHub[i]), &(instance_ptstationcoords->isPC4[i]), &(instance_ptstationcoords->isTAZ[i]));
	}
	fclose(graphPtr);

	if ((err = fopen_s(&graphPtr, "test_basic_connections.dat", "r")) != 0)
	{
		printf("test_basic_connections.dat was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(instance_ptroutedata->stationRecord));
	// cout << "records number is: " << instance_ptroutedata->stationRecord << endl;
	instance_ptroutedata->routesData = generate_int_matrix(instance_ptroutedata->stationRecord, 6);
	instance_ptroutedata->recordNum4Station = generate_int_arr(instance_ptstationcoords->stationNum);

	for (int m = 0; m < instance_ptstationcoords->stationNum; m++)
	{
		instance_ptroutedata->recordNum4Station[m] = 0;
	}

	// read the route data
	// start station, end station, start time, end time, link index, route index
	for (int i = 0; i < instance_ptroutedata->stationRecord; i++)
	{
		fscanf_s(graphPtr, "%d	%d	%d	%d	%d	%d",
			&(instance_ptroutedata->routesData[i][0]),
			&(instance_ptroutedata->routesData[i][1]),
			&(instance_ptroutedata->routesData[i][2]),
			&(instance_ptroutedata->routesData[i][3]),
			&(instance_ptroutedata->routesData[i][4]),
			&(instance_ptroutedata->routesData[i][5]));
	}
	fclose(graphPtr);

	printf("Finish reading PT network!\n");
}

void read_PV_data()
{
	FILE* graphPtr;
	errno_t		err;

	if ((err = fopen_s(&graphPtr, "test_road_node.txt", "r")) != 0)
	{
		printf("test_road_node.txt was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(instance_pvcoords->roadNodeNum));

	instance_pvcoords->roadNodeOldIdx = generate_int_arr(instance_pvcoords->roadNodeNum);
	instance_pvcoords->roadNodeNewIdx = generate_int_arr(instance_pvcoords->roadNodeNum);
	instance_pvcoords->roadNodecoordx = generate_int_arr(instance_pvcoords->roadNodeNum);
	instance_pvcoords->roadNodecoordy = generate_int_arr(instance_pvcoords->roadNodeNum);
	instance_pvcoords->isShop = generate_int_arr(instance_pvcoords->roadNodeNum);

	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		fscanf_s(graphPtr, "%d %d %d", &(instance_pvcoords->roadNodeOldIdx[i]), &(instance_pvcoords->roadNodecoordx[i]), &(instance_pvcoords->roadNodecoordy[i]));
	}
	fclose(graphPtr);
	
	if ((err = fopen_s(&graphPtr, "test_road_edge.txt", "r")) != 0)
	{
		printf("test_road_edge.txt. was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(instance_pvdata->edgeNum));

	instance_pvdata->edgeFistNode = generate_int_arr(instance_pvdata->edgeNum);
	instance_pvdata->edgeListNode = generate_int_arr(instance_pvdata->edgeNum);
	instance_pvdata->length = generate_int_arr(instance_pvdata->edgeNum);
	instance_pvdata->roadflag = generate_int_arr(instance_pvdata->edgeNum);
	instance_pvdata->cityflag = generate_int_arr(instance_pvdata->edgeNum);

	for (int i = 0; i < instance_pvdata->edgeNum; i++)
	{
		fscanf_s(graphPtr, "%d %d %d %d %d", &(instance_pvdata->edgeFistNode[i]), &(instance_pvdata->edgeListNode[i]), &(instance_pvdata->length[i]), &(instance_pvdata->roadflag[i]), &(instance_pvdata->cityflag[i]));
	}
	fclose(graphPtr);

	printf("Finish reading PV network!\n");
}


void read_shop_data()
{
	FILE* graphPtr;
	errno_t		err;

	if ((err = fopen_s(&graphPtr, "test_shop_inpv.txt", "r")) != 0)
	{
		printf("test_shop.txt was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(shop_data->numOfactivity));

	shop_data->activitynodeidx = generate_int_arr(shop_data->numOfactivity);
	shop_data->activitycoordx = generate_int_arr(shop_data->numOfactivity);
	shop_data->activitycoordy = generate_int_arr(shop_data->numOfactivity);
	shop_data->activityOpenTime = generate_int_arr(shop_data->numOfactivity);
	shop_data->activityCloseTime = generate_int_arr(shop_data->numOfactivity);

	for (int i = 0; i < shop_data->numOfactivity; i++)
	{
		fscanf_s(graphPtr, "%d %d %d %d %d", &(shop_data->activitynodeidx[i]), &(shop_data->activitycoordx[i]), &(shop_data->activitycoordy[i]), &(shop_data->activityOpenTime[i]), &(shop_data->activityCloseTime[i]));
	}
	fclose(graphPtr);

	printf("Finish reading shop data!\n");
}

void read_park_data()
{
	FILE* graphPtr;
	errno_t		err;

	if ((err = fopen_s(&graphPtr, "test_park_inpv.txt", "r")) != 0)
	{
		printf("test_park.txt was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(park_data->numOfactivity));


	park_data->activitynodeidx = generate_int_arr(park_data->numOfactivity);
	park_data->activitycoordx = generate_int_arr(park_data->numOfactivity);
	park_data->activitycoordy = generate_int_arr(park_data->numOfactivity);

	for (int i = 0; i < park_data->numOfactivity; i++)
	{
		fscanf_s(graphPtr, "%d %d %d", &(park_data->activitynodeidx[i]), &(park_data->activitycoordx[i]), &(park_data->activitycoordy[i]));
	}
	fclose(graphPtr);

	printf("Finish reading shop data!\n");

}

void read_transfer_station_data()
{
	FILE* graphPtr;
	errno_t		err;

	if ((err = fopen_s(&graphPtr, "test_transfer_station.txt", "r")) != 0)
	{
		printf("test_transfer_station.txt was not opened\n");
		getchar();
		exit(1);
	}
	fscanf_s(graphPtr, "%d", &(transfer_station_data->staNum));

	transfer_station_data->staIdx = generate_int_arr(transfer_station_data->staNum);
	transfer_station_data->stacoordx = generate_int_arr(transfer_station_data->staNum);
	transfer_station_data->stacoordy = generate_int_arr(transfer_station_data->staNum);
	transfer_station_data->starowidx = generate_int_arr(transfer_station_data->staNum);
	for (int i = 0; i < transfer_station_data->staNum; i++)
	{
		fscanf_s(graphPtr, "%d %d %d %d", &(transfer_station_data->staIdx[i]), &(transfer_station_data->stacoordx[i]), &(transfer_station_data->stacoordy[i]), &(transfer_station_data->starowidx[i]));
	}
	fclose(graphPtr);

	printf("Finish reading transfer station data!\n");
}