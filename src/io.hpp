#ifndef __IO_H
#define __IO_H

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
using namespace std;

string bool2string(bool);
void print_params(double, int,
		  double,
		  double, double, double,
		  double, double,
                  double, double,
		  double, double,
		  double,
                  bool, bool,
		  string,
		  bool,
		  bool,
		  bool,
		  string,
		  string);
double* list_from_file(int &N, const string infile);
void list_to_file(const int &N, const double* li, const string outfile);
vector<pair<int, double>> list_to_state(const int &N, const double* li);
vector<pair<int, double>> file_to_state(const string infile);
void state_to_file(const vector<pair<int, double>> state, const string outfile);

#endif

// __IO_H
