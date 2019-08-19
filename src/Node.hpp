#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <string>

#ifndef __NODE_H
#define __NODE_H

class Queue;

#include "Queue.hpp"
using namespace std;

class Node {
public:
  Node(double, Queue*);
  ~Node();
  void set_next(Node* a){ this->next_node = a; };
  void set_prev(Node* a){ this->prev_node = a; };
  Node* next() const { return this->next_node; };
  Node* prev() const { return this->prev_node; };
  double dist() const;
  Node* decay();
  Node* split();
  void to_move(double);
  void move();
  double dump_pos(double);
  double pos() const { return this->x; };
  Queue* get_queue() const { return queue; };
private:
  double x;
  double dx = 0.;
  int id;
  Node* next_node;
  Node* prev_node;
  ofstream file;
  Queue* queue;
};

#endif
