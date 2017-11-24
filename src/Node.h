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

#include "Queue.h"
using namespace std;

class Node {
public:
  Node(double, Queue*);
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
  Queue* get_queue(){ return queue; };
private:
  double x;
  int id;
  Node* next_node;
  Node* prev_node;
  ofstream file;
  Queue* queue;
};

#endif