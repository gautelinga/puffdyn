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

void updateProgress(double progress)
{

    cout << "||" ;
    for (int i=0; i<40; i++) {
      if (i<progress*40)
          cout << "#";
      else
          cout << " ";
    }
    cout << "||" << progress*100 << "%" ;
    cout.flush();
}

bool simulate(Queue &q, double dt, double T, int stat_intv, int dump_intv,
	      bool do_log_gaps, bool verbose){
  q.dump_stats();
  updateProgress(0);
  do {
    q.step(dt);
    if (q.timestep() % stat_intv == 0){
      q.dump_stats();
      if (q.timestep() % (stat_intv*100) == 0){
          cout << "\33[2K\r";
        if (verbose)
          cout << "Time = " << q.time() << ", size = " << q.size() << "." << endl;
        updateProgress(q.time()/T);
      }
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
  double L       = params.get("L",       100000.0);       // Domain length
  int    N       = params.get("N",       10000);          // Number of initial puffs
  double lc      = params.get("lc",      12.0);           // Interaction distance
  double lin     = params.get("lin",     20.0);
  double v0      = params.get("v0",      0.2406);
  double Re      = params.get("Re",      2040.);           // Reynolds number
//  double alpha_d = params.get("alpha_d", 3.0799);         // Decay time
//  double beta_d  = params.get("beta_d",  -9.2421);        // Decay time amplification
//  double alpha_s = params.get("alpha_s", 2.7048);         // Splitting time
//  double beta_s  = params.get("beta_s",  1.2207);         // Splitting time amplification
  double T       = params.get("T",       1e8);            // Total simulation time
  double dt      = params.get("dt",      1e2);            // Timestep
  double D       = params.get("D",       0.0);            // Diffusion
  double rho     = params.get("rho",     1);              // Density (optional)
  bool do_dump_pos = params.get_bool("dump_pos", false);                 // Dump positions
  string results_folder = params.get("results_folder", "results/1");  // Name
  bool verbose   = params.get_bool("verbose", false);                     // Verbose output
  int stat_intv  = params.get("stat_intv", 1000);   // Sample statistics interval
  int dump_intv  = params.get("dump_intv", 1000);  // Dump statistics after so many samples
  string init_mode = params.get("init_mode", "density");  // Init mode: 'random', 'mf' or 'equid'
  string importFile = params.get("importFile","");
  bool do_log_gaps = params.get_bool("log_gaps", false);  // Log gaps
  bool do_log_events = params.get_bool("log_events", false);  // Log events

  // Decay rate from the experiment (imposing lc=12);
  //double Ainf_d = 0.0058904;
  //double Binf_d = -9.1715;
  //double A_d    = -0.036365;
  //double B_d    = 66.4761;
  //double alpha_d = Ainf_d*Re + Binf_d;
  //double beta_d  = A_d*Re + B_d;

  // Splitting rate from the experiment (imposing lc=12);
  //double Ainf_s = -0.0031203;
  //double Binf_s = 9.1899;
  //double A_s    = 0.0033056;
  //double B_s    = -5.4757;
  //double alpha_s = Ainf_s*Re + Binf_s;
  //double beta_s  = A_s*Re + B_s;

  double a_d = -9.187149761728774;
  double b_d = 67.45338604868417;
  double c_d = 0.005897608616540853;
  double d_d = -0.036872821458203905;

  double a_s = 9.287142828546877;
  double b_s = -8.583449491545009;
  double c_s = -0.0031645844211272835;
  double d_s = 0.0047135153271309575;

  double alpha_d = c_d*Re + a_d;
  double beta_d  = d_d*Re + b_d;
  double alpha_s = c_s*Re + a_s;
  double beta_s  = d_s*Re + b_s;

  srand(time(NULL));
  
  double* li;
  if (init_mode == "random"){
    li = initialize_random(N, L);
  }
  else if (init_mode == "mf"){
    li = initialize_mf(N, L, lc, alpha_d, beta_d, alpha_s, beta_s);
  }
  else if (init_mode == "equid"){
    double dist = L/N;
    li = initialize_equid(N, L, dist);
  }
  else if (init_mode == "pair"){
    li = initialize_pair(N, L);
  }
  else if (init_mode == "density"){
    N = int(rho*L/lc);
    double dist = L/N;
    li = initialize_equid(N, L, dist);
  }
  else if (init_mode == "fromFile"){
    li = list_from_file(N,importFile);
  }
  else {
    cout << "Unknown initializing mode." << endl;
    exit(0);
  }

  // params.dump();
  // Initialize the queue

  Queue q(li, N, L, lc, lin, v0, alpha_s, beta_s, alpha_d, beta_d, D, do_dump_pos, do_log_events, verbose, results_folder);

  print_params(L, N,
	       T, dt,
           lc, lin, v0, Re,
	       alpha_d, beta_d, alpha_s, beta_s,
	       D,
	       do_dump_pos, verbose, init_mode,
               do_log_gaps, do_log_events, results_folder);

  // Do the loop.
  simulate(q, dt, T, stat_intv, dump_intv, do_log_gaps, true);

  string fileName = q.get_results_dir() + "/puffsPosition_final.dat";
  q.dump_state(fileName);

  cout << endl;
  cout << "================================================" << endl;
  cout << "=============== Simulation done ================" << endl;
  cout << "================================================" << endl;
  
  return 0;
}
