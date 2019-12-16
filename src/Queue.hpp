#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstdlib>
#include <random>
#include <set>

#ifndef __QUEUE_H
#define __QUEUE_H

class Node;

#include "Node.hpp"
using namespace std;

class Queue {
public:
  Queue(const double*, const int,
	const double, const double, const double, const double,
	const double, const double, const double,
	const double, const double,
	const bool, const bool, const bool, const string);
  Queue(const string,
	const double, const double, const double, const double,
	const double, const double, const double,
	const double, const double,
	const bool, const bool, const bool, const string);
  ~Queue();
  void init(const double*, const int,
	    const double, const double, const double, const double,
	    const double, const double, const double, const double,
	    const double, const bool, const bool, const bool, const string);
  int size() const { return queue_length; };
  void set_size(int s){ this->queue_length = s; };
  void load_list(const double*, const int);
  void dump_stats();
  void step(double);
  void set_time(const double t) { this->t = t; };
  double time() const { return this->t; }
  double v(double) const;
  double decay_rate(double) const;
  double splitting_rate(double) const;
  void set_first(Node*);
  void increase_size(){ ++queue_length; };
  void decrease_size(){ --queue_length; };
  string get_results_dir() const { return results_dir; };
  bool do_dump_pos() const { return dump_pos_flag; };
  void log_gaps();
  void dump_gaps(string);
  void dump_gaps(double);
  void dump_gaps();
  bool verbose() const { return verbose_flag; };
  int timestep() const { return it; };
  double domain_size() const { return L; };
  double* export_list(int &N) const;
  void read_state(string infile);
  void dump_state(string outfile) const;
private:
  Node* first;
  double L;
  int queue_length = 0;
  double t = 0.;
  int it = 0;
  double alpha_d;
  double beta_d;
  double alpha_s;
  double beta_s;
  double lc;
  double lin;
  double v0;
  double D;
  string results_dir;
  int count = 0;
  ofstream file;
  ofstream deaths_file;
  ofstream births_file;
  bool dump_pos_flag;
  bool log_events_flag;
  bool verbose_flag;
  set<double> gaps;
  double l_cutoff;
  double ws_inf;
  double wd_inf;
  double v_inf;
};

double rand_uniform_unit();

#endif
