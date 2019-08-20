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
  q.dump_stats();
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
  double L       = params.get("L",    5000.0);       // Domain length
  int    N       = params.get("N",      40);        // Number of initial puffs
  double lc      = params.get("lc",     12.0);       // Interaction distance
  double lin     = params.get("lin",    20.0);
  double v0      = params.get("v0",      0.2406);
  double alpha_d = params.get("alpha_d", 3.0799);     // Decay time
  double beta_d  = params.get("beta_d",  -9.2421);      // Decay time amplification
  double alpha_s = params.get("alpha_s", 2.7048);    // Splitting time
  double beta_s  = params.get("beta_s",  1.2207);      // Splitting time amplification
  double T       = params.get("T",     1e12);      // Total simulation time
  double dt      = params.get("dt",    1e2);      // Timestep
  double D       = params.get("D",       0.0);       // Diffusion
  bool do_dump_pos = params.get_bool("dump_pos", true);                 // Dump positions
  string results_folder = params.get("results_folder", "results/0");  // Name
  bool verbose   = params.get_bool("verbose", false);                     // Verbose output
  int stat_intv  = params.get("stat_intv", 100);   // Sample statistics interval
  int dump_intv  = params.get("dump_intv", 1000);  // Dump statistics after so many samples
  string init_mode = params.get("init_mode", "mf");  // Init mode: 'random', 'mf' or 'equid'
  bool do_log_gaps = params.get_bool("log_gaps", false);  // Log gaps
  bool do_log_events = params.get_bool("log_events", false);  // Log events

  srand(time(NULL));
  
  double* li;
  if (init_mode == "random")
    li = initialize_random(N, L);
  else if (init_mode == "mf")
    li = initialize_mf(N, L, lc, alpha_d, beta_d, alpha_s, beta_s);
  else if (init_mode == "equid")
    li = initialize_equid(N, L, lc);
  else if (init_mode == "pair")
    li = initialize_pair(N, L);
  else {
    cout << "Unknown initializing mode." << endl;
    exit(0);
  }

  // params.dump();
  // Initialize the queue

  Queue q(li, N, L, lc, lin, v0, alpha_s, beta_s, alpha_d, beta_d, D, do_dump_pos, do_log_events, verbose, results_folder);

  print_params(L, N,
	       T, dt,
	       lc, lin, v0,
	       alpha_d, beta_d, alpha_s, beta_s,
	       D,
	       do_dump_pos, verbose, init_mode,
               do_log_gaps, do_log_events, results_folder);

  // Do the loop.
  simulate(q, dt, T, stat_intv, dump_intv, do_log_gaps, true);
  
  return 0;
}
