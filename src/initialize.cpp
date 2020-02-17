#include "initialize.hpp"
#include "io.hpp"

#include <math.h>
#include <vector>
#include <fstream>
using namespace std;


vector<pair<int, double>> initialize_state(string infile){
  ifstream input(infile);
  vector<pair<int, double>> state;
  double x_loc;
  int id_loc;
  while (input >> id_loc >> x_loc){
    state.push_back(make_pair(id_loc, x_loc));
  }
  return state;
}

vector<pair<int, double>> initialize_random(int N, double L){
  double* li = new double[N];
  for (int i=0; i < N; ++i){
    li[i] = L*double(rand())/RAND_MAX;
  }
  sort(li, li+N);
  return list_to_state(N, li);
}

vector<pair<int, double>> initialize_mf(int &N, double L, double lc,
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
  return list_to_state(N, li);
}

vector<pair<int, double>> initialize_equid(int &N, double L, double lc){
  N = int(L/lc);
  double* li = new double[N];
  double x = 0.5*lc;
  for (int i=0; i < N; ++i){
    li[i] = x;
    x += lc;
  }
  return list_to_state(N, li);
}

vector<pair<int, double>> initialize_pair(int &N, double L){
  double* li = new double[N];
  for (int i=0; i < N; ++i){
    li[i] = 0.5*L + i*1e-9*L;
  }
  return list_to_state(N, li);
}
