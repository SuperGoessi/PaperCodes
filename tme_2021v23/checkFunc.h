#pragma once
#include<string>

std::string fileNameGenerator(string const& str);

std::string fileNameGenerator(string const& str, int netidx, int loopidx);

int randomNumGenerator(int lower, int upper);

int findold_idx(int newIdx);

int findindex(int oldindex);

bool checkShopPPA(int shopidx, int stationidx, int earliesttime, int latesttime);
bool checkParkPPA(int parkidx, int stationidx, int earliesttime, int latesttime);
void output_data_shops(const vector<vector<int>>& arr, int personidx, int startStation, int endStation, int startTime);
void output_data_parks(const vector<vector<int>>& arr, int personidx, int startStation, int endStation, int startTime);

void output_data_ppa(int personidx, int startStation, int startTime);
void output_data_pre_step1_pv(const vector<vector<int>>& arr, int psersonidx, int pvflag);
void output_data_pre_step1_pt(const vector<vector<int>>& arr, int psersonidx, int pvflag);
void output_data_pre_step2(const vector<vector<int>>& arr, int psersonidx, int pvflag);
void output_data_pre_step3(const vector<vector<int>>& arr, int psersonidx, int pvflag);
void output_data_pre_step3_trianglecompare(const vector<vector<int>>& arr, int psersonidx, int pvflag);
void output_data_pre_step2_distcompare(const vector<vector<int>>& arr, int psersonidx, int pvflag);
void output_data_pre_step4_distcompare(const vector<vector<int>>& arr, int psersonidx, int pvflag);

void output_data_ptforward(const vector<vector<int>>& arr, int loopidx, int startstation);
void output_data_ptbackward(const vector<vector<int>>& arr, int loopidx);



void output_data(int personidx, int startStation, int endStation, int pv0fidx, int pt0fidx, int pt1fidx, int pv1fidx, int pv1bidx, int pt1bidx, int pt0bidx, int pv0bidx);
void output_data_pv0f(int personidx, int startStation, int endStation, int pv0fidx, int startTime);
void output_data_pt0f(int personidx, int startStation, int endStation, int pt0fidx, int startTime);
void output_data_pt1f(int personidx, int startStation, int endStation, int pt1fidx);
void output_data_pv1f(int personidx, int startStation, int endStation, int pv1fidx);
void output_data_pv1b(int personidx, int startStation, int endStation, int pv1bidx);
void output_data_pt1b(int personidx, int startStation, int endStation, int pt1bidx);
void output_data_pt0b(int personidx, int startStation, int endStation, int pt0bidx, int startTime);
void output_data_pv0b(int personidx, int startStation, int endStation, int pv0bidx, int startTime);

void output_shop_data();

void output_final_station(const vector<vector<int>>& arr, int psersonidx, int startStation);

void check_shop_single(int personidx, int pvfalg);
void check_park_single(int personidx, int pvfalg);

void check_park(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg);
void check_shop(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg);

void check_shop_diff(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg);
void check_park_diff(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg);

vector<vector<int>> check_shop_v(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg);
vector<vector<int>> check_shop_single_v(int personidx, int pvfalg);
vector<vector<int>> check_shop_diff_v(const vector<vector<int>>& arr, int typeidx, int personidx, int pvfalg);

void combineResult(vector<vector<int>>& singleModeResult, vector<vector<int>>& multiModeResult, vector<vector<int>>& simplifiedResult);
vector<vector<int>> findTimes4Shop(const vector<vector<int>>& multiModeResult);
vector<vector<int>> findMaxTimeRange(const vector<vector<int>>& arr);

vector<vector<int>> Result_walk(const vector<vector<int>>& walkModeResult);
vector<vector<int>> deleteDuplicate(const vector<vector<int>>& arr);

vector<int> get_walksta(const vector<vector<int>>& walkmode);
vector<int> sumup(const vector<int>& walk_sta, const vector<vector<int>>& bike, const vector<vector<int>>& car, int personidx);