#include "Queue.h"
#include <stdio.h>
#include <sys/stat.h>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
//namespace fs = boost::filesystem;

class Node;
class Queue;

Queue::Queue(double* li, int N, double d, double ws, double wd, double D, string results_dir){
  cout << "Creating queue!" << endl;
  Node *n = new Node(li[0], this);
  Node *n_prev, *n_first;

  this->results_dir = results_dir;

  struct stat sb;
  if (stat(results_dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)){
    string rm_code_str = "rm " + results_dir + "/*";
    const char* rm_code = rm_code_str.c_str();
    const int rm_err = system(rm_code);
    if (rm_err == -1){
      cout << "Couldn't empty folder!" << endl;
      exit(0);
    } 
  }
  else {
    cout << "Creating directory: " << results_dir << endl;
    string dir_code_str = "mkdir -p " + results_dir;
    const char* dir_code = dir_code_str.c_str();
    const int dir_err = system(dir_code);
    if (dir_err == -1){
      cout << "Couldn't create folder!" << endl;
      exit(0);
    }
  }

  n_first = n;
  for (int i=1; i<N; ++i){
    n_prev = n;
    n = new Node(li[i], this);
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
    }
  } while (n != this->first);
  cout << "Size: " << this->size() << endl;
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