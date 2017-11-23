#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <string>
#include "Node.h"

using namespace std;

#ifndef __QUEUE_H
#define __QUEUE_H

class Queue {
public:
  Queue(double*, int, double, double, double, double, string);
  ~Queue();
  int size(){ return queue_length; };
  void set_size(int s){ this->queue_length = s; };
  void step(double);
  double time(){ return this->t; }
  double v(double);
  double decay_rate(double);
  double splitting_rate(double);
  void set_first(Node*);
  void increase_size(){ ++queue_length; };
  void decrease_size(){ --queue_length; };
private:
  Node* first;
  int queue_length;
  double t = 0.;
  double wd;
  double ws;
  double d;
  double D;
  string results_dir;
  int count=0;
};

#endif