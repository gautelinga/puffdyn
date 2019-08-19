#ifndef __INITIALIZE_H
#define __INITIALIZE_H

#include <stdlib.h>
#include <algorithm>
using namespace std;


double* initialize_random(int N, double L);
double* initialize_mf(int &N, double L, double d, double wd, double ws, double A);
double* initialize_equid(int &N, double L, double d);

#endif // __INITIALIZE_H
