#include "io.hpp"

#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <boost/algorithm/string.hpp>
using namespace std;

string bool2string(bool a){
  return (a ? "True" : "False");
}

void print_params(double L,
		  int N,
		  double t,
		  double T,
		  double dt,
		  double lc,
		  double lin,
		  double v0,
                  double alpha_d,
		  double beta_d, 
		  double alpha_s,
		  double beta_s,
		  double D,
                  bool do_dump_pos,
		  bool verbose,
		  string init_mode,
                  bool do_log_gaps,
		  bool do_log_events,
		  bool clear_all,
		  string restart_folder,
                  string results_folder){
  cout << "================================================" << endl;
  cout << "PARAMETERS:" << endl;
  cout << "------------------------------------------------" << endl;
  cout << " L       = " << L << endl;
  cout << " N       = " << N << endl;
  cout << " t       = " << t << endl;
  cout << " T       = " << T << endl;
  cout << " dt      = " << dt << endl;
  cout << " lc      = " << lc << endl;
  cout << " lin     = " << lin << endl;
  cout << " v0      = " << v0 << endl;
  cout << " alpha_d = " << alpha_d << endl;
  cout << " beta_d  = " << beta_d << endl;
  cout << " alpha_s = " << alpha_s << endl;
  cout << " beta_s  = " << beta_s << endl;
  cout << " D       = " << D << endl;
  cout << "------------------------------------------------" << endl;
  cout << " dump_pos       = " << bool2string(do_dump_pos) << endl;
  cout << " restart_folder = " << restart_folder << endl;
  cout << " results_folder = " << results_folder << endl;
  cout << " verbose        = " << bool2string(verbose) << endl;
  cout << " init_mode      = " << init_mode << endl;
  cout << " log_gaps       = " << bool2string(do_log_gaps) << endl;
  cout << " log_events     = " << bool2string(do_log_events) << endl;
  cout << " clear_all      = " << bool2string(clear_all) << endl;
  cout << "================================================" << endl;
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
    output << li[i] << " ";
  }
  output.close();
}

vector<pair<int, double>> list_to_state(const int &N, const double* li){
  vector<pair<int, double>> state;
  for (int i=0; i<N; ++i){
    state.push_back(make_pair(i, li[i]));
  }
  return state;
}

vector<pair<int, double>> file_to_state(const string infile){
  ifstream input(infile);
  vector<pair<int, double>> state;
  double x_loc;
  int id_loc;
  while (input >> id_loc >> x_loc){
    state.push_back(make_pair(id_loc, x_loc));
  }
  return state;
}

void state_to_file(const vector<pair<int, double>> state, const string outfile){
  ofstream output(outfile);
  for (vector<pair<int, double>>::const_iterator it = state.begin();
       it != state.end(); it++){
    output << it->first << " " << it->second << endl;
  }
  output.close();
}
