#include "io.hpp"

#include <vector>
#include <fstream>
using namespace std;

string bool2string(bool a){
  return (a ? "True" : "False");
}

void print_params(double L, int N,
                  double T, double dt,
                  double lc, double lin, double v0, double Re,
                  double alpha_d, double beta_d, 
                  double alpha_s, double beta_s,
                  double D,
                  double rate_amplification,
                  bool do_dump_pos, bool verbose,
                  string init_mode,
                  bool do_log_gaps,
                  bool do_log_events,
                  bool tome_mod,
                  bool step_mod,
                  string results_folder){
  cout << "================================================" << endl;
  cout << "PARAMETERS:" << endl;
  cout << "------------------------------------------------" << endl;
  cout << " L       = " << L << endl;
  cout << " N       = " << N << endl;
  cout << " T       = " << T << endl;
  cout << " dt      = " << dt << endl;
  cout << " lc      = " << lc << endl;
  cout << " lin     = " << lin << endl;
  cout << " v0      = " << v0 << endl;
  cout << " Re      = " << Re << endl;
  cout << " alpha_d = " << alpha_d << endl;
  cout << " beta_d  = " << beta_d << endl;
  cout << " alpha_s = " << alpha_s << endl;
  cout << " beta_s  = " << beta_s << endl;
  cout << " D       = " << D << endl;
  cout << " rate_amplification = " << rate_amplification << endl;
  cout << "------------------------------------------------" << endl;
  cout << " dump_pos       = " << bool2string(do_dump_pos) << endl;
  cout << " results_folder = " << results_folder << endl;
  cout << " verbose        = " << bool2string(verbose) << endl;
  cout << " init_mode      = " << init_mode << endl;
  cout << " log_gaps       = " << bool2string(do_log_gaps) << endl;
  cout << " log_events     = " << bool2string(do_log_events) << endl;
  cout << " tome_mod       = " << bool2string(tome_mod) << endl;
  cout << " step_mod       = " << bool2string(step_mod) << endl;
  cout << "================================================" << endl;
  cout << " Timescales" << endl;
  cout << "----------------------------------------------- " << endl;
  cout << " tau^inf_s      = " << 1./rate_amplification * exp(exp(alpha_s)) << endl;
  cout << " tau^inf_d      = " << 1./rate_amplification * exp(exp(alpha_d)) << endl;
  cout << " tau_D          = " << L*L/D << endl;
  cout << " tau_v0         = " << lc/v0 << endl;
  cout << "================================================" << endl;

  ofstream ofile;
  ofile.open(results_folder + "/params.dat", ofstream::out);
  ofile << "L=\t" << L << endl;
  ofile << "N=\t" << N << endl;
  ofile << "T=\t" << T << endl;
  ofile << "dt=\t" << dt << endl;
  ofile << "lc=\t" << lc << endl;
  ofile << "lin=\t" << lin << endl;
  ofile << "v0=\t" << v0 << endl;
  ofile << "alpha_d=\t" << alpha_d << endl;
  ofile << "beta_d=\t" << beta_d << endl;
  ofile << "alpha_s=\t" << alpha_s << endl;
  ofile << "beta_s=\t" << beta_s << endl;
  ofile << "D=\t" << D << endl;
  ofile << "rate_amplification=" << rate_amplification << endl;
  ofile << "dump_pos=\t" << bool2string(do_dump_pos) << endl;
  ofile << "results_folder=\t" << results_folder << endl;
  ofile << "verbose=\t" << bool2string(verbose) << endl;
  ofile << "init_mode=\t" << init_mode << endl;
  ofile << "log_gaps=\t" << bool2string(do_log_gaps) << endl;
  ofile << "log_events=\t" << bool2string(do_log_events) << endl;
  ofile << "tome_mod=\t" << bool2string(tome_mod) << endl;
  ofile << "step_mod=\t" << bool2string(step_mod) << endl;
  ofile.close();
}


double* list_from_file(int &N, const string infile){
  ifstream input(infile);
  vector<double> x;
  double x_loc;
  while (input >> x_loc){
    x.push_back(x_loc);
  }
  N = x.size();
  double* li = new double[N];
  for (int i=0; i < N; ++i){
    li[i] = x[i];
  }
  return li;
}


void list_to_file(const int &N, const double* li, const string outfile){
  ofstream output(outfile);
  for (int i=0; i < N; ++i){
    output << li[i] << "\n";
  }
}
