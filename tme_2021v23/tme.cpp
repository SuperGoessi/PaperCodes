// tme.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

#include <iostream>
#include <time.h>
#include <ctime>
#include <cstdio>
#include "inout.h"
#include "checkFunc.h"
#include "timeexpanded.h"
#include "search.h"
#include "binaryheap.h"
#include "netbuild.h"
#include "test.h"
#include "sepsearch.h"
#include "deleteStation.h"

int main()
{
    // with preselection
    test_data_pre();
    allocate_instances();
    assignPVSearchNet(); // no time assign
    assignPVPTConnNet();
    assignPV2Shop(); // delete
    assignSpeedProfile(5);
    vector<vector<int>> ptData = tm_expanded();
    assignPTSearchNet(ptData);
    assignPTSearchNet_reverse(ptData);
    assignSimplePTSearchNet(computeAverageTime());
    init_heaps(); 
    assignSta2Shop(4);
    assignSta2Park(4);

    time_t begin, end; // time_t is a datatype to store time values.

    time(&begin); // note time before execution

    vector<vector<int>> population_result;

    
        

    int startNode = findNewIdx(370395);
    int endNode = findNewIdx(370395);
    int startTime = 500;
    int timeBudget = 60; // minute
    int activityTime = 15;
    int transferBudget = 1;
    int pvflag1 = 0;
    double densityfactor = 2;
  
    // single person test
    assign_stp_ap_init(startNode, endNode, startTime, timeBudget * 10000, transferBudget, activityTime * 10000, densityfactor);
  
    
  
    
    /*-----------------------------------------bike and car mode---------------------------------------------------------*/
    vector<vector<int>> simplifiedResult_bike;
    vector<vector<int>> simplifiedResult_car;
    for (int j = 1; j < 3; j++)
    {
        pvflag1 = j;
       // distanceDelete(pvflag1);
        // single modal
        unimodal_search(startNode, endNode, startTime, pvflag1, 0);
        vector<vector<int>> single_mode = check_shop_single_v(0, pvflag1);

        // selection with
        vector<vector<int>> result2;
        vector<vector<int>> stations = stationPreSelection(pvflag1, 0);
        for (int i = 0; i < stations.size(); i++)
        {
            int startStation = stations[i][0];
            int startNode2 = stations[i][1];

            vector<vector<int>> arr = multimodal_search(startNode, endNode, startNode2, startStation, startTime, timeBudget, pvflag1, 1);
            result2.reserve(result2.size() + arr.size());
            result2.insert(result2.end(), arr.begin(), arr.end());
            arr.clear();

            rest_heaps();
        }
        vector<vector<int>> multi_mode = check_shop_v(result2, 1, 0, pvflag1);
        if (pvflag1 == 1)
            combineResult(single_mode, multi_mode, simplifiedResult_bike);
        if (pvflag1 == 2)
            combineResult(single_mode, multi_mode, simplifiedResult_car);

            
    }

 
    /*-----------------------------------------walk mode---------------------------------------------------------*/
    
    // walk-PT
    vector<vector<int>> result5;
    vector<vector<int>> walkstations_start = findNerestStations(4, startNode);
    vector<vector<int>> walkstation_end = findNerestStations(4, endNode);
        

    for (int i = 0; i < walkstations_start.size(); i++)
    {
        int startStation = walkstations_start[i][0];
        int startNode2 = walkstations_start[i][1];

        for (int j = 0; j < walkstation_end.size(); j++)
        {
            int endStation = walkstation_end[j][0];
            int endNode2 = walkstation_end[j][1];

            vector<vector<int>> arr = multimodal_search_diff(startNode, endNode, startNode2, startStation, endNode2, endStation, startTime, timeBudget, 0, 1);

            result5.reserve(result5.size() + arr.size());
            result5.insert(result5.end(), arr.begin(), arr.end());
            arr.clear();

            rest_heaps();
        }
    }
    
    vector<vector<int>> walk_mode = check_shop_diff_v(result5, 2, 0, pvflag1);
    vector<int> walk_sta;
    if (walk_mode.size() != 0)
    {
        walk_sta = get_walksta(walk_mode);
    }
    if (walk_sta.size() == 0)
    {
        walk_sta.push_back(0);
        walk_sta.push_back(0);
    }

    sort(simplifiedResult_bike.begin(), simplifiedResult_bike.end(), sortFunc_01);
    sort(simplifiedResult_car.begin(), simplifiedResult_car.end(), sortFunc_01);

    
    vector<vector<int>> simplifiedResult_bike_final = deleteDuplicate(simplifiedResult_bike);
    vector<vector<int>> simplifiedResult_car_final = deleteDuplicate(simplifiedResult_car);
    

    vector<int> temp = sumup(walk_sta, simplifiedResult_bike_final, simplifiedResult_car_final, 0);

    for (auto i : temp)
        cout << i << " ";
    cout << endl;

    //output_data_ptbackward(simplifiedResult_bike_final, 111);
    //output_data_ptbackward(simplifiedResult_car_final, 222);
    time(&end); // note time after execution

    double difference = difftime(end, begin);
    printf("time taken for function() %.2lf seconds.\n", difference);
    
   
    return 0;
}
