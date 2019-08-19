#include "initialize.hpp"

#include <math.h>
using namespace std;

double* initialize_random(int N, double L){
  double* li = new double[N];
  for (int i=0; i < N; ++i){
    li[i] = L*double(rand())/RAND_MAX;
  }
  sort(li, li+N);
  return li;
}

double* initialize_mf(int &N, double L, double d, double wd, double ws, double A){
  double ell = d*log(1.0 + (A*wd-ws/A)/(ws-wd));
  N = int(L/ell);
  double* li = new double[N];
  ell = L/N;
  double x = 0.5*ell;
  for (int i=0; i < N; ++i){
    li[i] = x;
    x += ell;
  }
  return li;
}

double* initialize_equid(int &N, double L, double d){
  N = int(L/d);
  double* li = new double[N];
  double x = 0.5*d;
  for (int i=0; i < N; ++i){
    li[i] = x;
    x += d;
  }
  return li;
}
