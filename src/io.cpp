#include "io.hpp"

#include <vector>
#include <fstream>
using namespace std;

string bool2string(bool a){
  return (a ? "True" : "False");
}

void print_params(double L, int N, double T, double dt, double d,
                  double wd, double ws, double D, double A,
                  bool do_dump_pos, bool verbose, string init_mode,
                  bool do_log_gaps,
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
  cout << " log_gaps       = " << do_log_gaps << endl;
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
  ofile << "log_gaps=" << bool2string(do_log_gaps) << endl;
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
    output << li[i] << " ";
  }
}
