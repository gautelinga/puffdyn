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

string bool2string(bool a);
void print_params(double L, int N, double T, double dt, double d,
                  double wd, double ws, double D, double A,
                  bool do_dump_pos, bool verbose, string init_mode,
                  bool do_log_gaps,
                  string results_folder);
double* list_from_file(int &N, const string infile);
void list_to_file(const int &N, const double* li, const string outfile);

#endif // __IO_H
