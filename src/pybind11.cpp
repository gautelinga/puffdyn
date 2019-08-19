#include "Queue.hpp"
#include "Parameters.hpp"

#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;

void simulate_f2f(string state_infile,
		  string params_infile,
		  string state_outfile,
		  string params_outfile){
  Parameters params(params_infile);
  double L       = params.get("L",       40.0);  // Domain length
  double v0      = params.get("v0",      1.0);
  double lc      = params.get("lc",      0.1);   // Interaction distance
  double alpha_d = params.get("alpha_d", 0.1);   // Decay rate
  double beta_d  = params.get("beta_d",  0.1);   //
  double alpha_s = params.get("alpha_s", 0.16);  // Splitting rate
  double beta_s  = params.get("alpha_s", 0.16);  //
  double D       = params.get("D",       0.0);   // Diffusion

  double t       = params.get("t",       0.0);   // Time
  double T       = params.get("T",    1000.0);   // Total simulation time
  double dt      = params.get("dt",      0.001); // Timestep


  // To be removed
  double wd  = params.get("wd",  0.1); // Decay rate
  double ws  = params.get("ws", 0.16); // Splitting rate
  double A   = params.get("A", 5.0);   // Amplification
  
  Queue q(state_infile, L, lc, ws, wd, A, D, false, false, "tmp");
  q.set_time(t);
  
  double dt_mod;
  do {
    dt_mod = min(dt, T-q.time());
    q.step(dt_mod);
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
  	py::arg("params_outfile")
  	);
}
