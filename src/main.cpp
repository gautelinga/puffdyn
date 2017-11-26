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

string bool2string(bool a){
  return (a ? "True" : "False");
}

void print_params(double L, int N, double T, double dt, double d,
                  double wd, double ws, double D, double A,
                  bool do_dump_pos, bool verbose, string init_mode,
                  string results_folder){
  cout << "================================================" << endl;
  cout << "PARAMETERS:" << endl;
  cout << "------------------------------------------------" << endl;
  cout << " L   = " << L << endl;
  cout << " N   = " << N << endl;
  cout << " T   = " << T << endl;
  cout << " dt  = " << dt << endl;
  cout << " d   = " << d << endl;
  cout << " wd  = " << wd << endl;
  cout << " ws  = " << ws << endl;
  cout << " A   = " << A << endl;
  cout << " D   = " << D << endl;
  cout << "------------------------------------------------" << endl;
  cout << " dump_pos       = " << bool2string(do_dump_pos) << endl;
  cout << " results_folder = " << results_folder << endl;
  cout << " verbose        = " << bool2string(verbose) << endl;
  cout << " init_mode      = " << init_mode << endl;
  cout << "================================================" << endl;

  ofstream ofile;
  ofile.open(results_folder + "/params.dat", ofstream::out);
  ofile << "L=" << L << endl;
  ofile << "N=" << N << endl;
  ofile << "T=" << T << endl;
  ofile << "dt=" << dt << endl;
  ofile << "d=" << d << endl;
  ofile << "wd=" << wd << endl;
  ofile << "ws=" << ws << endl;
  ofile << "A=" << A << endl;
  ofile << "D=" << D << endl;
  ofile << "dump_pos=" << bool2string(do_dump_pos) << endl;
  ofile << "results_folder=" << results_folder << endl;
  ofile << "verbose=" << bool2string(verbose) << endl;
  ofile << "init_mode=" << init_mode << endl;
  ofile.close();
}


int main(int argc, char* argv[]){
  Parameters params(argc, argv);
  double L    = params.get("L", 40.0);       // Domain length
  int    N    = params.get("N", 10);        // Number of initial puffs
  double d    = params.get("d", 0.1);       // Interaction distance
  double wd   = params.get("wd", 0.1);      // Decay rate
  double ws   = params.get("ws", 0.16);      // Splitting rate
  double A    = params.get("A", 5.0);       // Amplification factor
  double T    = params.get("T", 1000.0);      // Total simulation time
  double dt   = params.get("dt", 0.001);      // Timestep
  double D    = params.get("D", 0.0);       // Diffusion
  bool do_dump_pos = params.get_bool("dump_pos", true);                 // Dump positions
  string results_folder = params.get("results_folder", "results/One");  // Name
  bool verbose = params.get_bool("verbose", false);                     // Verbose output
  int stat_intv = params.get("stat_intv", 100);   // Sample statistics interval
  int dump_intv = params.get("dump_intv", 1000);  // Dump statistics after so many samples
  string init_mode = params.get("init_mode", "mf");  // Init mode: 'random' or 'mf'

  srand(time(NULL));
  
  double* li;
  if (init_mode == "random"){
    li = initialize_random(N, L);
  }
  else if (init_mode == "mf"){
    li = initialize_mf(N, L, d, wd, ws, A);
  }
  else {
    cout << "Unknown initializing mode." << endl;
    exit(0);
  }

  // params.dump();
  // Initialize the queue

  Queue q(li, L, N, d, ws, wd, A, D, do_dump_pos, verbose, results_folder);

  print_params(L, N, T, dt, d, wd, ws, A, D, do_dump_pos, verbose, init_mode, results_folder);

  // Do the loop.
  do {
    q.step(dt);
    if (q.timestep() % stat_intv == 0){
      q.dump_stats();
      cout << "Time = " << q.time() << ", size = " << q.size() << "." << endl;
      q.log_gaps();
      if (q.timestep() % (stat_intv*dump_intv) == 0){
        q.dump_gaps(q.time());
      }
    }
  } while (q.size() > 0 && q.time() <= T);
  
  return 0;
}