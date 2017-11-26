#include "Queue.h"
#include <stdio.h>
#include <sys/stat.h>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
//namespace fs = boost::filesystem;

random_device rd;
mt19937 e2(rd());
normal_distribution<double> gaussrd(0.0, 1.0);

class Node;
class Queue;

Queue::Queue(double* li, double L, int N, double d, double ws, double wd, double A,
             double D, bool do_dump_pos, bool verbose, string results_dir){
  cout << "Creating queue!" << endl;

  this->L = L;
  this->dump_pos_flag = do_dump_pos;
  this->verbose_flag = verbose;
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
    if (this->verbose()){
      cout << "Creating directory: " << results_dir << endl;
    }
    string dir_code_str = "mkdir -p " + results_dir;
    const char* dir_code = dir_code_str.c_str();
    const int dir_err = system(dir_code);
    if (dir_err == -1){
      cout << "Couldn't create folder!" << endl;
      exit(0);
    }
  }

  this->file.open(this->get_results_dir() + "/tdata.dat", ofstream::out);

  Node *n = new Node(li[0], this);
  Node *n_prev, *n_first;

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

  this->d = d;
  this->ws = ws;
  this->wd = wd;
  this->A = A;
  this->D = D;

}

Queue::~Queue(){
  if (this->verbose())
    cout << "Destroying queue!" << endl;
  Node* n = this->first;
  Node* n_prev;
  if (n != NULL){
    do {
      n_prev = n;
      n = n_prev->next();
      delete n_prev;
    } while (n != this->first);
  }
  this->file.close();
}

void Queue::dump_stats(){
  Node *n = this->first;
  double x_sum = 0.;
  do {
    x_sum += n->dump_pos(this->t);
    n = n->next();
  } while(n != this->first);
  this->file << this->t << " " << x_sum/this->size() << " " << this->size() << endl;
}

void Queue::step(double dt){
  this->t += dt;
  ++this->it;
  Node *n = this->first;
  double distx, dx, u;
  double Pd, Ps;
  double R;
  bool is_first;
  do {
    distx = n->dist();
    u = this->v(distx);
    dx = u*dt;
    if (this->D > 0.0){
      dx += sqrt(D*dt)*gaussrd(e2);
    }
    dx = max(0., min(dx, distx));
    n->to_move(dx);  // Since it is not quite correct to move immediately

    Pd = this->decay_rate(u)*dt;
    Ps = this->splitting_rate(u)*dt;
    R = double(rand())/RAND_MAX;
    is_first = bool(n == this->first);
    if (R < Pd){
      n = n->decay();
      if (is_first && n != NULL){
        this->set_first(n->prev());
      }
      else if (n == NULL){
        this->set_first(NULL);
      }
    }
    else if (R < Pd+Ps){
      n = n->split();
      if (is_first){
        this->set_first(n->prev());
      }
    }
    else {
      n = n->next();
    }
  } while (n != this->first);
  if (n != NULL){
    do {
      n->move();
      n = n->next();
    } while (n != this->first);
  }
}

double Queue::v(double dist) const {
  // Velocity
  return 1.-exp(-dist/this->d);
}

double Queue::decay_rate(double v) const {
  return this->wd*(this->A + (1.-this->A)*v);
}

double Queue::splitting_rate(double v) const {
  return this->ws*(1./this->A + (1.-1./this->A)*v);
}

void Queue::set_first(Node* n){
  this->first = n;
}

void Queue::log_gaps(){
  Node* n = this->first;
  do {
    this->gaps.insert(n->dist());
    n = n->next();
  } while (n != this->first);
}

void Queue::dump_gaps(string filename){
  ofstream gaps_file(filename, ofstream::out);
  for (set<double>::iterator it = this->gaps.begin(); it != gaps.end(); ++it){
    gaps_file << *it << endl;
  }
  gaps.clear();
  gaps_file.close();
}

void Queue::dump_gaps(double t){
  string filename = this->get_results_dir() + "/gaps_t" + to_string(t) + ".dat";
  this->dump_gaps(filename);
}

void Queue::dump_gaps(){
  string filename = this->get_results_dir() + "/gaps.dat";
  this->dump_gaps(filename);
}