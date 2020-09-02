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

double* initialize_mf(int &N, double L, double lc,
		      double alpha_d, double beta_d,
		      double alpha_s, double beta_s){
  double ell = -lc*log(-(alpha_d-alpha_s)/(beta_d-beta_s));
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

double* initialize_equid(int &N, double L, double lc){
  N = int(L/lc);
  double* li = new double[N];
  double x = 0.5*lc;
  for (int i=0; i < N; ++i){
    li[i] = x;
    x += lc;
  }
  return li;
}

double* initialize_pair(int &N, double L){
  double* li = new double[N];
  for (int i=0; i < N; ++i){
    li[i] = 0.5*L + i*1e-9*L;
  }
  return li;
}

