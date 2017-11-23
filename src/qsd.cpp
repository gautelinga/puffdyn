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
#include "Node.h"
#include "Queue.h"
#include "Parameters.h"
using namespace std;


double* initialize(int N){
  double* li = new double[N];
  for (int i=0; i < N; ++i){
    li[i] = double(rand())/RAND_MAX;
  }
  sort(li, li+N);
  return li;
}

void print_params(int N, double T, double dt, double d,
                  double wd, double ws, double D){
  cout << "PARAMETERS:" << endl;
  cout << " N   = " << N << endl;
  cout << " T   = " << T << endl;
  cout << " dt  = " << dt << endl;
  cout << " d   = " << d << endl;
  cout << " wd  = " << wd << endl;
  cout << " ws  = " << ws << endl;
  cout << " D   = " << D << endl;
}

int main(int argc, char* argv[]){
  Parameters params(argc, argv);
  int    N    = params.get("N", 10);        // Number of initial puffs
  double d    = params.get("d", 0.1);       // Interaction distance
  double wd   = params.get("wd", 0.0);      // Decay rate
  double ws   = params.get("ws", 0.0);      // Splitting rate
  double T    = params.get("T", 10.0);      // Total simulation time
  double dt   = params.get("dt", 0.1);      // Timestep
  double D    = params.get("D", 0.0);       // Diffusion
  string name = params.get("name", "One");  // Name 

  // params.dump();
  print_params(N, T, dt, d, wd, ws, D);

  srand(time(NULL));
  
  double* li = initialize(N);

  // Initialize the queue
  Queue q(li, N, d, wd, ws, D, name);

  // Do the loop.
  do {
    q.step(dt);
    //cout << "Size: " << q.size() << endl;
  } while (q.time() <= T);
  
  return 0;
}