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

#include "Node.h"
using namespace std;

class Queue {
public:
  Queue(double*, double, int, double, double, double, double, double, bool, bool, string);
  ~Queue();
  int size() const { return queue_length; };
  void set_size(int s){ this->queue_length = s; };
  void dump_stats();
  void step(double);
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
private:
  Node* first;
  double L;
  int queue_length = 0;
  double t = 0.;
  int it = 0;
  double wd;
  double ws;
  double A;
  double d;
  double D;
  string results_dir;
  int count=0;
  ofstream file;
  bool dump_pos_flag;
  bool verbose_flag;
  set<double> gaps;
};

#endif