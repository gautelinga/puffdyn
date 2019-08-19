#ifndef __INITIALIZE_H
#define __INITIALIZE_H

#include <stdlib.h>
#include <algorithm>
using namespace std;


double* initialize_random(int, double);
double* initialize_mf(int &, double, double, double, double, double, double);
double* initialize_equid(int &, double, double);

#endif // __INITIALIZE_H
