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
#include <boost/lexical_cast.hpp>

#include "Node.hpp"
#include "Queue.hpp"
#include "Parameters.hpp"
#include "io.hpp"
#include "initialize.hpp"
using namespace std;


bool simulate(Queue &q, double dt, double T, int stat_intv, int dump_intv,
	      bool do_log_gaps, bool verbose){
  do {
    q.step(dt);
    if (q.timestep() % stat_intv == 0){
      q.dump_stats();
      if (verbose)
	cout << "Time = " << q.time() << ", size = " << q.size() << "." << endl;
      if (do_log_gaps){
        q.log_gaps();
        if (q.timestep() % (stat_intv*dump_intv) == 0){
          q.dump_gaps(q.time());
        }
      }
    }
  } while (q.size() > 0 && q.time() <= T);
  return true;
}

int main(int argc, char* argv[]){
  Parameters params(argc, argv);
  double L       = params.get("L",       4000.0);       // Domain length
  int    N       = params.get("N",       30);        // Number of initial puffs
  double lc      = params.get("lc",      12.0);       // Interaction distance
  double v0      = params.get("v0",      0.22);
  double alpha_d = params.get("alpha_d", 0.1);     // Decay rate
  double beta_d  = params.get("beta_d",  0.1);      // Decay rate amplification
  double alpha_s = params.get("alpha_s", 0.16);    // Splitting rate
  double beta_s  = params.get("beta_s",  0.1);      // Splitting rate amplification
  double T       = params.get("T",    1000.0);      // Total simulation time
  double dt      = params.get("dt",      0.001);      // Timestep
  double D       = params.get("D",       0.0);       // Diffusion
  bool do_dump_pos = params.get_bool("dump_pos", true);                 // Dump positions
  string results_folder = params.get("results_folder", "results/One");  // Name
  bool verbose   = params.get_bool("verbose", false);                     // Verbose output
  int stat_intv  = params.get("stat_intv", 100);   // Sample statistics interval
  int dump_intv  = params.get("dump_intv", 1000);  // Dump statistics after so many samples
  string init_mode = params.get("init_mode", "mf");  // Init mode: 'random' or 'mf' or 'equid'
  bool do_log_gaps = params.get_bool("log_gaps", true);  // Log gaps

  srand(time(NULL));
  
  double* li;
  if (init_mode == "random"){
    li = initialize_random(N, L);
  }
  else if (init_mode == "mf"){
    li = initialize_mf(N, L, lc, alpha_d, beta_d, alpha_s, beta_s);
  }
  else if (init_mode == "equid")
    li = initialize_equid(N, L, lc);
  else {
    cout << "Unknown initializing mode." << endl;
    exit(0);
  }

  // params.dump();
  // Initialize the queue

  Queue q(li, N, L, lc, v0, alpha_s, beta_s, alpha_d, beta_d, D, do_dump_pos, verbose, results_folder);

  print_params(L, N, T, dt, lc, v0, alpha_d, beta_d, alpha_s, beta_s, D, do_dump_pos, verbose, init_mode,
               do_log_gaps, results_folder);

  // Do the loop.
  simulate(q, dt, T, stat_intv, dump_intv, do_log_gaps, true);
  
  return 0;
}
