#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
//#include "stdafx.h"
#include <string.h>
#include <memory.h>
#include <algorithm>
#include <assert.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include <iostream>
#include <cctype>
#include <random>



#include "inout.h"
#include "checkFunc.h"
#include "timeexpanded.h"
#include "search.h"
#include "binaryheap.h"
#include "netbuild.h"
#include "test.h"
#include "test.h"
#include "sepsearch.h"
#include "deleteStation.h"

int		INF_DIST2 = 2000000;

using namespace std;
using u32 = uint_least32_t;
using engine = std::mt19937;

int generateRandomNum(int lowerRange, int higherRange)
{
	std::random_device os_seed;
	const u32 seed = os_seed();

	engine generator(seed);
	std::uniform_int_distribution< u32 > distribute(lowerRange, higherRange);

	for (int repetition = 0; repetition < 10; ++repetition)
		return distribute(generator);
}

void generateRandomData(int numofData)
{
	string filename = fileNameGenerator("start_information");
	std::ofstream file(filename);

	file << numofData << endl;
	for (int i = 0; i < numofData; i++)
	{
		file << generateRandomNum(0, 25710) << " " << generateRandomNum(0, 25710) << " " << generateRandomNum(360, 1350) << " " << generateRandomNum(20, 180) << " " << generateRandomNum(1, 3) << endl;
	}
	file.close();
}



int findI_pv2(int newIdx)
{
	for (int i = 0; i < instance_pvcoords->roadNodeNum; i++)
	{
		if (instance_pvcoords->roadNodeNewIdx[i] == newIdx)
			return i;
	}
}


int get_A_tvl_time2(int node1, int node2)
{
	int i = findI_pv2(node1);
	int j = findI_pv2(node2);
	int x1 = instance_pvcoords->roadNodecoordx[i];
	int y1 = instance_pvcoords->roadNodecoordy[i];
	int x2 = instance_pvcoords->roadNodecoordx[j];
	int y2 = instance_pvcoords->roadNodecoordy[j];

	return int(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) / (90 * 1000 / 3600)); // in second
}
/*
void sp_PV_tbs_test(heap_sch_struct* heap, int startNode, int endNode, int netidx)
{
	int inttmp;

	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		heap->dist[i] = INF_DIST2;
		heap->distRL[i] = INF_DIST2;
	}

	memset(heap->state, 0, (instance_spnetpv->nodeNum) * sizeof(char));

	inttmp = get_A_tvl_time2(startNode, endNode);
	heap->distRL[startNode] = 0;
	insert_val_to_sp_heap(heap, startNode, inttmp, inttmp, 2, 0);

	while (heap->size >= 0)
	{
		scan_PV_forward(heap, netidx, endNode);
	}
	cout << "Finish forward search" << endl;

}

void scan_PV_forward(heap_sch_struct* heap, int netidx, int endNode)
{
	int		tmp_node, tmp_distSV, inttmp;

	for (int i = instance_spnetpv->edgeIndex[heap->topNode][0]; i <= instance_spnetpv->edgeIndex[heap->topNode][1]; i++)
	{
		tmp_node = instance_spnetpv->edgeList[i];
		inttmp = heap->distRL[heap->topNode] + instance_spnetpv->edgeDisU[i];
		tmp_distSV = inttmp + get_A_tvl_time2(tmp_node, endNode);

		if (tmp_distSV <= instance_stpap->timeBudget_pv && tmp_distSV < heap->dist[tmp_node])
		{
			heap->dist[tmp_node] = tmp_distSV; // estimated travel time
			heap->distRL[tmp_node] = inttmp; // real travel time

			//instance_spresult->pv_parent[netidx][tmp_node] = heap->topNode;
			//instance_spresult->pv_searched[netidx][tmp_node] = 1;
			instance_spresult->pv_cost[netidx][tmp_node] = inttmp; // used for backward search

			if (heap->state[tmp_node] == 0)
				InsertElement(tmp_distSV, tmp_node, heap);
			else
				DecreaseKey(tmp_distSV, tmp_node, heap);
		}
	}
	cout << heap->size << endl;
	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}

void sp_PV_tbs_test_2nd(heap_sch_struct* heap, int startNode, int endNode, int netidx)
{
	int inttmp;

	for (int i = 0; i < instance_spnetpv->nodeNum; i++)
	{
		heap->dist[i] = INF_DIST2;
	}

	memset(heap->state, 0, (instance_spnetpv->nodeNum) * sizeof(char));

	insert_val_to_sp_heap(heap, endNode, 0, 0, 2, 0);

	while (heap->size >= 0)
	{
		scan_PV_backward(heap, netidx);
	}
	cout << "Finish backward search" << endl;
}

void scan_PV_backward(heap_sch_struct* heap, int netidx)
{
	int		tmp_node, tmp_distSV;
	cout << heap->topNode << endl;
	for (int i = instance_spnetpv->edgeIndex[heap->topNode][0]; i < instance_spnetpv->edgeIndex[heap->topNode][1]; i++)
	{
		tmp_node = instance_spnetpv->edgeList[i];
		
		if (instance_spresult->pv_cost[netidx][tmp_node] != -1 && instance_spresult->pv_cost[netidx][tmp_node] + heap->topKey <= instance_stpap->timeBudget_pv)
		{
			tmp_distSV = heap->topKey + instance_spnetpv->edgeDisU[i];

			if (tmp_distSV < heap->dist[tmp_node])
			{
				heap->dist[tmp_node] = tmp_distSV;

				//instance_spresult->pv_parent_reverse[netidx][tmp_node] = heap->topNode;
				//instance_spresult->pv_searched_reverse[netidx][tmp_node] = 1;
				instance_spresult->pv_cost_reverse[netidx][tmp_node] = tmp_distSV;

				if (heap->state[tmp_node] == 0)
					InsertElement(tmp_distSV, tmp_node, heap);
				else
					DecreaseKey(tmp_distSV, tmp_node, heap);
			}
		}
	}
	cout << heap->size << endl;

	DeleteAndExtractMin(heap);
	heap->state[heap->topNode] = 2;
}*/