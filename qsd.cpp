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

class Queue;

int COUNT = 0;

const string RESULTS_DIR = "results/";

class Node {
public:
  Node(double);
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
};

class Queue {
public:
  Queue(double*, int, double, double, double, double);
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
};

int main(int argc, char* argv[]){
  int N;      // Number of initial puffs
  double d;   // Interaction distance
  double wd;  // Decay rate
  double ws;  // Splitting rate
  double T;   // Total simulation time
  double dt;  // Timestep
  double D;   // Diffusion
  if (argc == 8){
    N = atoi(argv[1]);
    T = atof(argv[2]);
    dt = atof(argv[3]);
    d = atof(argv[4]);
    wd = atof(argv[5]);
    ws = atof(argv[6]);
    D = atof(argv[7]);
  }
  else{
    cout << "Not enough arguments!" << endl;
    return 0;
  }

  srand(time(NULL));
  
  double* li;
  li = new double[N];
  for (int i=0; i < N; ++i){
    li[i] = double(rand())/RAND_MAX;
  }
  sort(li, li+N);

  Queue q(li, N, d, wd, ws, D);

  do {
    q.step(dt);
    //cout << "Size: " << q.size() << endl;
  } while (q.time() <= T);
  
  return 0;
}

Node::Node(double x){
  cout << "Creation of x=" << x << "!"  << endl;
  this->x = x;
  this->id = COUNT++;

  this->file.open(RESULTS_DIR + to_string(this->id)+ ".dat", ofstream::out);
}

Node::~Node(void){
  cout << "Destruction of " << this->id << "!" << endl;
  this->file.close();
}

double Node::dist(){
  double dx = this->next()->x-this->x;
  return dx-floor(dx);
}

void Node::move(double dx){
  this->x += dx;
}

Node* Node::decay(){
  Node* n_prev = this->prev();
  Node* n_next = this->next();
  n_prev->set_next(n_next);
  n_next->set_prev(n_prev);
  if (this != n_next){
    delete this;
  }
  return n_next;
}

Node* Node::split(){
  Node* n_prev = this->prev();
  Node* n = new Node(this->x);
  n_prev->set_next(n);
  n->set_prev(n_prev);
  n->set_next(this);
  return this->next();
}

void Node::dump(double t){
  //cout << "Taking a dump." << endl;
  this->file << t << " " << this->x << endl;
}

Queue::Queue(double* li, int N, double d, double ws, double wd, double D){
  cout << "Creating queue!" << endl;
  Node *n = new Node(li[0]);
  Node *n_prev, *n_first;
  n_first = n;
  for (int i=1; i<N; ++i){
    n_prev = n;
    n = new Node(li[i]);
    n->set_prev(n_prev);
    n_prev->set_next(n);
  }
  n->set_next(n_first);
  n_first->set_prev(n);
  this->first = n_first;
  this->set_size(N);

  this->d = d;
  this->ws = ws;
  this->wd = wd;
  this->D = D;
}

Queue::~Queue(){
  cout << "Destroying queue!" << endl;
  Node* n = this->first;
  Node* n_prev;
  do {
    n_prev = n;
    n = n_prev->next();
    delete n_prev;
  } while (n != this->first);
}

void Queue::step(double dt){
  this->t += dt;
  cout << "Time = " << this->time() << endl;
  Node *n = this->first;
  double dx, u;
  double Pd, Ps;
  double R;
  do {
    n->dump(this->t);
    u = this->v(n->dist());
    dx = u*dt;
    // cout << dx << " " << n->dist() << endl;
    n->move(dx);  // Not quite correct to move immediately but ok to first order in dt

    Pd = this->decay_rate(u)*dt;
    Ps = this->splitting_rate(u)*dt;
    R = double(rand())/RAND_MAX;
    if (R < Pd){
      n = n->decay();
      this->set_first(n);
      this->decrease_size();
    }
    else if (R < Pd+Ps){
      n = n->split();
      this->set_first(n->prev());
      this->increase_size();
    }
    else {
      n = n->next();
      cout << "Size: " << this->size() << endl;
    }
  } while (n != this->first);
}


double Queue::v(double dx){
  // cout << "I.e. " << dx << " " << this->d << " " << dx/this->d << endl;
  return 1.-exp(-dx/this->d);
}

double Queue::decay_rate(double v){
  return this->wd*(2.-v);
}

double Queue::splitting_rate(double v){
  return this->ws*v;
}

void Queue::set_first(Node* n){
  this->first = n;
}
