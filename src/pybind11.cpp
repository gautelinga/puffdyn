#include "Queue.hpp"
#include "Parameters.hpp"
#include "io.hpp"

#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;

void simulate_f2f(string state_infile,
		  string params_infile,
		  string state_outfile,
		  string params_outfile,
		  string data_folder){
  Parameters params(params_infile);
  double L       = params.get("L",    1000.0);  // Domain length
  double v0      = params.get("v0",      0.2406);
  double lc      = params.get("lc",     12.0);   // Interaction distance
  double lin     = params.get("lin",    20.0);
  double alpha_d = params.get("alpha_d", 3.07);   // Decay rate
  double beta_d  = params.get("beta_d", -9.24);   //
  double alpha_s = params.get("alpha_s", 2.70);  // Splitting rate
  double beta_s  = params.get("beta_s",  1.22);  //
  double D       = params.get("D",       1e-1);   // Diffusion

  double t       = params.get("t",       0.0);   // Time
  double T       = params.get("T",       1e8);   // Total simulation time
  double dt      = params.get("dt",      1e1); // Timestep

  bool do_dump_pos = params.get_bool("dump_pos", false);                 // Dump positions
  bool verbose   = params.get_bool("verbose", false);                     // Verbose output
  int stat_intv  = params.get("stat_intv", 10);   // Sample statistics interval
  int dump_intv  = params.get("dump_intv", 10);  // Dump statistics after so many samples
  bool do_log_gaps = params.get_bool("log_gaps", false);  // Log gaps
  bool do_log_events = params.get_bool("log_events", false);  // Log events

  Queue q(state_infile, L, lc, lin, v0,
	  alpha_s, beta_s, alpha_d, beta_d, D,
	  do_dump_pos, do_log_events, false, data_folder);
  q.set_time(t);

  print_params(L, q.size(),
	       T, dt,
	       lc, lin, v0,
	       alpha_d, beta_d, alpha_s, beta_s,
	       D,
	       do_dump_pos, verbose, "none",
               do_log_gaps, do_log_events, data_folder);

  double dt_mod;
  do {
    dt_mod = min(dt, T-q.time());
    q.step(dt_mod);
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
  } while (q.size() > 0 && q.time() < T);

  q.dump_state(state_outfile);

  params.set("t", q.time());

  params.dump(params_outfile);
}

PYBIND11_MODULE(_puffdyn, m)
{
  m.doc() = "Puff dynamics.";
  m.def("_simulate_f2f", &simulate_f2f,
  	py::arg("state_infile"),
  	py::arg("params_infile"),
  	py::arg("state_outfile"),
  	py::arg("params_outfile"),
	py::arg("data_folder")
  	);
}
