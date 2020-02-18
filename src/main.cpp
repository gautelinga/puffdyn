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
#include <iterator>

#include "Node.hpp"
#include "Queue.hpp"
#include "Parameters.hpp"
#include "io.hpp"
#include "initialize.hpp"
using namespace std;


bool simulate(Queue &q, double dt, double T, int stat_intv, int dump_intv,
	      bool do_log_gaps, bool verbose){
  q.dump_stats();
  while (q.size() > 0 && q.time() < T) {
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
  };
  return true;
}

int main(int argc, char* argv[]){
  Parameters params(argc, argv);
  double L       = params.get("L",    1000.0);     // Domain length
  int    N       = params.get("N",      40);       // Number of initial puffs
  double lc      = params.get("lc",     12.0);     // Interaction distance
  double lin     = params.get("lin",    20.0);     // Insertion length
  double v0      = params.get("v0",      0.2406);  // Base velocity
  double alpha_d = params.get("alpha_d", 3.0799);  // Decay time
  double beta_d  = params.get("beta_d",  -9.2421); // Decay time amplification
  double alpha_s = params.get("alpha_s", 2.7048);  // Splitting time
  double beta_s  = params.get("beta_s",  1.2207);  // Splitting time amplification
  double t       = params.get("t",       0.0);     // Initial time
  double T       = params.get("T",     1e12);      // Total simulation time
  double dt      = params.get("dt",    1e1);       // Timestep
  double D       = params.get("D",       0.0);     // Diffusion
  double rho     = params.get("rho",     0.1);     // Density (optional)
  bool do_dump_pos = params.get_bool("dump_pos", true);                 // Dump positions
  string results_folder = params.get("results_folder", "results/0");  // Name
  string restart_folder = params.get("restart_folder", "");
  bool verbose   = params.get_bool("verbose", false);                     // Verbose output
  int stat_intv  = params.get("stat_intv", 100);   // Sample statistics interval
  int dump_intv  = params.get("dump_intv", 1000);  // Dump statistics after so many samples
  string init_mode = params.get("init_mode", "mf");  // Init mode: 'random', 'mf' or 'equid'
  bool do_log_gaps = params.get_bool("log_gaps", false);  // Log gaps
  bool do_log_events = params.get_bool("log_events", false);  // Log events
  bool do_clear_all = params.get_bool("clear_all", true); // Clear all previous data

  srand(time(NULL));
  
  vector<pair<int, double>> state;
  string state_file = params.get("state_file", "");
  if (state_file != ""){
    state = initialize_state(state_file);
  }
  else if (init_mode == "random"){
    state = initialize_random(N, L);
  }
  else if (init_mode == "mf"){
    state = initialize_mf(L, lc, alpha_d, beta_d, alpha_s, beta_s);
  }
  else if (init_mode == "equid"){
    double dist = L/N;
    state = initialize_equid(N, L, dist);
  }
  else if (init_mode == "pair"){
    state = initialize_pair(N, L);
  }
  else if (init_mode == "density"){
    N = int(rho*L/lc);
    double dist = L/N;
    state = initialize_equid(N, L, dist);
  }
  else {
    cout << "Unknown initializing mode." << endl;
    exit(0);
  }

  // Initialize the queue
  Queue q(state,
	  t, L, lc, lin, v0,
	  alpha_s, beta_s, alpha_d, beta_d, D,
	  do_dump_pos, do_log_events, verbose, do_clear_all,
	  results_folder);
  
  N = state.size();

  print_params(L, N, t, T, dt, lc, lin, v0,
	       alpha_d, beta_d, alpha_s, beta_s, D,
	       do_dump_pos, verbose, init_mode,
               do_log_gaps, do_log_events, do_clear_all,
	       restart_folder,
	       results_folder);

  params.set("N", N);
  params.set("L", L);
  params.set("lc", lc);
  params.set("lin", lin);
  params.set("v0", v0);
  params.set("alpha_d", alpha_d);
  params.set("beta_d",  beta_d);
  params.set("alpha_s", alpha_s);
  params.set("beta_s",  beta_s);
  params.set("t", t);
  params.set("T", T);
  params.set("dt", dt);
  params.set("D", D);
  params.set("rho", rho);
  params.set_bool("dump_pos", do_dump_pos);
  params.set("results_folder", results_folder);
  params.set("restart_folder", restart_folder);
  params.set_bool("verbose", verbose);
  params.set("stat_intv", stat_intv);
  params.set("dump_intv", dump_intv);
  params.set("init_mode", init_mode);
  params.set_bool("log_gaps", do_log_gaps);
  params.set_bool("log_events", do_log_events);
  params.set_bool("clear_all", do_clear_all);

  params.dump(results_folder + "/params.dat");

  // Do the loop.
  simulate(q, dt, T, stat_intv, dump_intv, do_log_gaps, true);
  
  params.set("t", q.time());
  params.set("N", q.size());
  params.set_bool("clear_all", false);
  
  // Write final state
  params.dump(results_folder + "/checkpoint/params.dat");
  q.dump_state_to_file(results_folder + "/checkpoint/state.dat");
  
  return 0;
}
