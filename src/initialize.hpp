#ifndef __INITIALIZE_H
#define __INITIALIZE_H

#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
using namespace std;


vector<pair<int, double>> initialize_state(string);
vector<pair<int, double>> initialize_random(int, double);
vector<pair<int, double>> initialize_mf(double, double, double, double, double, double);
vector<pair<int, double>> initialize_equid(int &, double, double);
vector<pair<int, double>> initialize_pair(int &, double);

#endif // __INITIALIZE_H
