
#include <vector>
using namespace std;

bool sortFunc_01(vector<int> const& s1, vector<int> const& s2);

bool sortFunc_03(vector<int> const& s1, vector<int> const& s2);

bool sortFunc_13(vector<int> const& s1, vector<int> const& s2);

bool sortFunc0(const vector<int>& p1, const vector<int>& p2);

bool sortFunc1(const vector<int>& p1, const vector<int>& p2);

bool sortFunc2(const vector<int>& p1, const vector<int>& p2);

bool sortFunc8(const vector<int>& p1, const vector<int>& p2);

void check4RecordStation(vector<vector<int>>& v, int lenOfSub, int** originalData);

void singleStationDataFunc(int stationidx, const vector<vector<int>> data, vector<vector<int>>& arrData, vector<vector<int>>& depData);

void get_tm_expanded_physical(const vector<vector<int>>& v, vector<vector<int>>& graphData);

void get_tm_expanded_real(const vector<vector<int>>& v, int stationidx, int minTrsTime, vector<vector<int>>& graphData);

int get_stop_euDist(int i, int j);

void assignNearest4Station(int stationidx);

void assigndist2Station();

vector<vector<int>> tm_expanded();