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

bool sortFunc_01(vector<int> const& s1, vector<int> const& s2)
{
	if (s1[0] != s2[0])
	{
		return s1[0] < s2[0];
	}

	return s1[1] < s2[1];
}

bool sortFunc_03(vector<int> const& s1, vector<int> const& s2)
{
	if (s1[0] != s2[0])
	{
		return s1[0] < s2[0];
	}

	return s1[3] < s2[3];
}

bool sortFunc_13(vector<int> const& s1, vector<int> const& s2)
{
	// If the values of the first column are not equal,
	// just use them to order s1 and s2.
	if (s1[1] != s2[1])
	{
		return s1[1] < s2[1];
	}

	// If the values of the first column are equal,
	// use the values of the second column to order s1 and s2.
	return s1[3] < s2[3];
}

bool sortFunc8(const vector<int>& p1, const vector<int>& p2)
{
	return p1[8] < p2[8];
}

bool sortFunc0(const vector<int>& p1, const vector<int>& p2)
{
	return p1[0] < p2[0];
}

bool sortFunc1(const vector<int>& p1, const vector<int>& p2)
{
	return p1[1] < p2[1];
}

bool sortFunc2(const vector<int>& p1, const vector<int>& p2)
{
	return p1[2] < p2[2];
}

void check4RecordStation(vector<vector<int>>& v, int lenOfSub, int** originalData)
{
	// each station occurance time
	for (int i = 0; i < lenOfSub; i++)
	{
		vector<int> temp_arr;
		int station1 = originalData[i][0];
		int station2 = originalData[i][1];
		//cout << station1 << " " << station2 << endl;

		int count1 = instance_ptroutedata->recordNum4Station[station1] + 1;
		int count2 = instance_ptroutedata->recordNum4Station[station2] + 1;

		instance_ptroutedata->recordNum4Station[station1] = count1;
		instance_ptroutedata->recordNum4Station[station2] = count2;

		temp_arr.push_back(station1 * 10000 + count1);
		//cout << station1 * 10000 + count1 << endl;
		temp_arr.push_back(station2 * 10000 + count2);
		//cout << station2 * 10000 + count1 << endl;
		for (int j = 0; j < 6; j++)
		{
			temp_arr.push_back(originalData[i][j]);
		}
		v.push_back(temp_arr);
	}


}

int get_stop_euDist(int i, int j)
{
	float	a, b, c;
	a = (instance_ptstationcoords->stationcoordx[i] - instance_ptstationcoords->stationcoordx[j]) / 1000.0f;
	b = (instance_ptstationcoords->stationcoordy[i] - instance_ptstationcoords->stationcoordy[j]) / 1000.0f;

	c = sqrt(a * a + b * b);
	assert(c >= 0);
	return int(1210 * c); // ???
}

void assignNearest4Station(int stationidx)
{
	vector<vector<int>> temp_array;
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		vector<int> temp_arr;
		int dist = get_stop_euDist(stationidx, i);
		//cout << i << " " << dist << endl;
		temp_arr.push_back(i);
		temp_arr.push_back(dist);

		temp_array.push_back(temp_arr);
	}

	sort(temp_array.begin(), temp_array.end(), sortFunc1);

	for (int j = 1; j < 5; j++)
	{
		instance_ptstationcoords->fourNearestStation[stationidx][j - 1] = temp_array[j][0]; // first line is always self2self dist as 0m
		instance_ptstationcoords->distNearestStation[stationidx][j - 1] = temp_array[j][1];
		//cout << stationidx << " " << temp_array[j][0] << " " << temp_array[j][1] << endl;
	}
}

void assigndist2Station()
{
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		assignNearest4Station(i);
	}
	cout << "Finish assign 4 stations" << endl;
}


void singleStationDataFunc(int stationidx, const vector<vector<int>> data, vector<vector<int>>& arrData, vector<vector<int>>& depData)
{
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i][2] == stationidx)
		{
			vector<int> temp_array;
			for (int j = 0; j < 8; j++)
			{
				temp_array.push_back(data[i][j]);
			}
			temp_array.push_back(data[i][4]);
			depData.push_back(temp_array);
		}
		else if (data[i][3] == stationidx)
		{
			vector<int> temp_array;
			for (int j = 0; j < 8; j++)
			{
				temp_array.push_back(data[i][j]);
			}
			temp_array.push_back(data[i][5]);
			arrData.push_back(temp_array);
		}
	}

	sort(arrData.begin(), arrData.end(), sortFunc8);
	sort(depData.begin(), depData.end(), sortFunc8);
}

void get_tm_expanded_physical(const vector<vector<int>>& v, vector<vector<int>>& graphData)
{
	for (int i = 0; i < v.size(); i++)
	{
		vector<int> temp_array;
		temp_array.push_back(v[i][0]); // new idx1
		temp_array.push_back(v[i][1]); // new idx2
		temp_array.push_back(v[i][4]); // t1
		temp_array.push_back(v[i][5]); // t2
		temp_array.push_back(v[i][6]); // line number
		temp_array.push_back(v[i][7]); // route number
		//temp_array.push_back(v[i][8]);
		temp_array.push_back(1); // type flag
		temp_array.push_back(v[i][2]); // physical station1
		temp_array.push_back(v[i][3]); // physical station2

		graphData.push_back(temp_array);
	}
}

void get_tm_expanded_real(const vector<vector<int>>& v, int stationidx, int minTrsTime, vector<vector<int>>& graphData)
{
	// flags: 1-original data with new index, 2-arrival node to depart node, 3-transfer node to depart node, 4-arrival node to transfer node, 5-arrival node to other station arrival node

	// step 0: extract data for certain station, add additional col for time point for each record of this station
	vector<vector<int>> arrData;
	vector<vector<int>> depData;
	singleStationDataFunc(stationidx, v, arrData, depData);

	//cout << stationidx << ": " << instance_spnetpt->stationNodeNum[stationidx] << endl;

	// step 1: add transportation links bwtween the stations, one for arrive node, one for depart node
	// call get_tm_expanded_physical(v, graphData) for add all physical links once
	// cout << " step 0-1 finish" << endl;

	// step 2: add arrival to depart links
	for (int i = 0; i < arrData.size(); i++)
	{
		for (int j = 0; j < depData.size(); j++)
		{
			bool check4line = (arrData[i][6] == depData[j][6]); // check line
			bool check4route = (arrData[i][7] == depData[j][7]); // check route
			bool check4time = (arrData[i][8] == depData[j][8]); // check time
			bool check4station = (arrData[i][3] == depData[j][2]); // check station


			if (check4line && check4route && check4time && check4station)
			{
				vector<int> temp_array;

				temp_array.push_back(arrData[i][1]);
				temp_array.push_back(depData[j][0]);
				temp_array.push_back(arrData[i][5]);
				temp_array.push_back(depData[j][4]);
				temp_array.push_back(arrData[i][6]);
				temp_array.push_back(arrData[i][7]);
				//temp_array.push_back(arrData[i][8]);
				temp_array.push_back(2);
				temp_array.push_back(arrData[i][3]);
				temp_array.push_back(depData[j][2]);

				graphData.push_back(temp_array);
				//cout << "add one" << endl;
			}
		}
	}
	// cout << " step 2 finish" << endl;

	// step 3: add transfer to depart links
	vector<vector<int>> transferNodes;
	for (int i = 0; i < depData.size(); i++)
	{
		int departnode = depData[i][0];

		int count = instance_ptroutedata->recordNum4Station[stationidx] + 1;
		int transfernode = stationidx * 10000 + count;
		instance_ptroutedata->recordNum4Station[stationidx] = count;

		vector<int> temp_array;
		temp_array.push_back(transfernode); // new index for transfer node
		temp_array.push_back(departnode); // index for depart node
		temp_array.push_back(depData[i][4]); // depart time
		temp_array.push_back(depData[i][4]); // depart time
		temp_array.push_back(depData[i][6]); // depart line number
		temp_array.push_back(depData[i][7]); // deapart route number
		//temp_array.push_back(depData[i][8]); // depart time
		temp_array.push_back(3);
		temp_array.push_back(depData[i][2]);
		temp_array.push_back(depData[i][2]);
		graphData.push_back(temp_array);


		// prepare for add edge between arrive node and transfer node
		vector<int> temp_arr;
		temp_arr.push_back(transfernode); // node index
		temp_arr.push_back(depData[i][4]); // depart time
		temp_arr.push_back(depData[i][6]); // line number
		temp_arr.push_back(depData[i][7]); // route number
		temp_arr.push_back(depData[i][2]); // depart station
		transferNodes.push_back(temp_arr);

	}
	sort(transferNodes.begin(), transferNodes.end(), sortFunc1); // sort transfer nodes according to the time value
	// cout << " step 3 finish" << endl;

	// step 4: add arrival to transfer links
	for (int i = 0; i < arrData.size(); i++)
	{
		int time_point = 0;
		int count = 0;
		int time = 0;
		for (int j = 0; j < transferNodes.size(); j++)
		{
			time = transferNodes[j][1] - arrData[i][5];
			if ((count == 0 || transferNodes[j][1] == time_point) && time >= minTrsTime && arrData[i][7] != transferNodes[j][3]) // not check line
			{
				if (count == 0)
				{
					time_point = transferNodes[j][1];
					count += 1;
				}

				vector<int> temp_array;

				temp_array.push_back(arrData[i][1]);
				temp_array.push_back(transferNodes[j][0]);
				temp_array.push_back(arrData[i][5]);
				temp_array.push_back(transferNodes[j][1]);
				temp_array.push_back(transferNodes[j][2]);
				temp_array.push_back(transferNodes[j][3]);
				//temp_array.push_back(transferNodes[j][1]); // depart time, not equal to arrive this station time
				temp_array.push_back(4);
				temp_array.push_back(arrData[i][3]);
				temp_array.push_back(transferNodes[j][4]);

				graphData.push_back(temp_array);
				//cout << arrData[i][1] << endl;
				//break;
			}
		}

	}
	// cout << " step 4 finish" << endl;

	// step 5: add links between transfers
	// step 6: add links to walkable stations
	for (int j = 0; j < 4; j++)
	{
		int dist = instance_ptstationcoords->distNearestStation[stationidx][j];

		if (dist <= 300)
		{
			int secondStationIdx = instance_ptstationcoords->fourNearestStation[stationidx][j];
			//cout << stationidx << " " << dist << " " << secondStationIdx << endl;
			vector<vector<int>> arrData2;
			vector<vector<int>> depData2;
			singleStationDataFunc(secondStationIdx, v, arrData2, depData2);

			for (int m = 0; m < arrData.size(); m++)
			{
				int firstTime = arrData[m][5];

				for (int n = 0; n < arrData2.size(); n++)
				{
					int secondTime = arrData2[n][5];

					if (secondTime >= firstTime + 2) // some is too large, leave as it
					{
						vector<int> temp_array;

						temp_array.push_back(arrData[m][1]);
						temp_array.push_back(arrData2[n][1]);
						temp_array.push_back(arrData[m][5]);
						temp_array.push_back(arrData2[n][5]);
						temp_array.push_back(1111);
						temp_array.push_back(1111);
						//temp_array.push_back(arrData[m][6]); // may design a new value for walk
						//temp_array.push_back(arrData[m][7]); // may design a new value for walk
						//temp_array.push_back(arrData[m][8]);
						temp_array.push_back(5);
						temp_array.push_back(arrData[m][3]);
						temp_array.push_back(arrData2[n][3]);

						graphData.push_back(temp_array);
						//cout << "add one" << endl;
						break;
					}
				}
			}

		}
	}
	// cout << " step 5-6 finish" << endl;
	//cout << "-----" << graphData.size() << "-----" << endl;
	//sort(v.begin(), v.end(), sortFunc6);
}

vector<vector<int>> tm_expanded()
{
	vector<vector<int>> arr;
	vector<vector<int>> graphData;

	// 1 new index for every record
	// arr cols = [newIdx S1/ newIdx S2/ S1/ S2/ T1/ T2/ Line number/ Route nuimber]
	check4RecordStation(arr, instance_ptroutedata->stationRecord, instance_ptroutedata->routesData);

	// 2 build data
	// graphData cols = [newIdx S1/ newIdx S2/ T1/ T2/ Line number/ Route nuimber/ edge flag/ S1/ S2]
	get_tm_expanded_physical(arr, graphData);
	
	for (int i = 0; i < instance_ptstationcoords->stationNum; i++)
	{
		get_tm_expanded_real(arr, i, 2, graphData);
	}

	// replace old index
	vector<int> temp_array;
	for (int i = 0; i < graphData.size(); i++)
	{
		temp_array.push_back(graphData[i][0]);
		temp_array.push_back(graphData[i][1]);
	}
	sort(temp_array.begin(), temp_array.end());
	temp_array.erase(unique(temp_array.begin(), temp_array.end()), temp_array.end());
	int numberOfNodes = (int)temp_array.size();
	
	instance_spnetpt->nodeNum = numberOfNodes;
	instance_spnetpt->oldIdx = generate_int_arr(instance_spnetpt->nodeNum);
	instance_spnetpt->newIdx = generate_int_arr(instance_spnetpt->nodeNum);

	for (int i = 0; i < instance_spnetpt->nodeNum; i++)
	{
		instance_spnetpt->oldIdx[i] = temp_array[i];
		instance_spnetpt->newIdx[i] = i;
	}

	for (int i = 0; i < graphData.size(); i++)
	{
		int tempidx1 = graphData[i][0];
		int tempidx2 = graphData[i][1];

		for (int j = 0; j < instance_spnetpt->nodeNum; j++)
		{
			if (tempidx1 == instance_spnetpt->oldIdx[j])
			{
				graphData[i][0] = instance_spnetpt->newIdx[j];
			}

			if (tempidx2 == instance_spnetpt->oldIdx[j])
			{
				graphData[i][1] = instance_spnetpt->newIdx[j];
			}
		}
	}

	sort(graphData.begin(), graphData.end(), sortFunc0);
	cout << "finish PT network expanded" << endl;
	return graphData;
}