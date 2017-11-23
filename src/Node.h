#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

#ifndef __NODE_H
#define __NODE_H

class Node {
public:
  Node(double, string);
  ~Node();
  void set_next(Node* a){ this->next_node = a; };
  void set_prev(Node* a){ this->prev_node = a; };
  Node* next(){ return this->next_node; };
  Node* prev(){ return this->prev_node; };
  double dist();
  Node* decay();
  Node* split();
  void move(double);
  void dump(double);
private:
  double x;
  int id;
  Node* next_node;
  Node* prev_node;
  ofstream file;
  string results_dir;
};

#endif