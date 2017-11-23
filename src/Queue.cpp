#include "Queue.h"


Queue::Queue(double* li, int N, double d, double ws, double wd, double D, string results_dir){
  cout << "Creating queue!" << endl;
  Node *n = new Node(li[0], results_dir);
  Node *n_prev, *n_first;

  this->results_dir = results_dir;

  n_first = n;
  for (int i=1; i<N; ++i){
    n_prev = n;
    n = new Node(li[i], results_dir);
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