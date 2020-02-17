#include "Queue.hpp"

#include <stdio.h>
#include <sys/stat.h>

#include "io.hpp"
#include <filesystem>
#include <utility>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
namespace fs = std::filesystem;

random_device rd;
mt19937 e2(rd());
normal_distribution<double> gaussrd(0.0, 1.0);

class Node;
class Queue;

Queue::Queue(const vector<pair<int, double>> state, const double t,
	     const double L, const double lc, const double lin, const double v0,
	     const double alpha_s, const double beta_s,
	     const double alpha_d, const double beta_d,
             const double D,
	     const bool do_dump_pos, const bool do_log_events,
	     const bool verbose,
	     const bool clear_all,
	     const string results_dir){
  this->init(state, t, L, lc, lin, v0, alpha_s, beta_s, alpha_d, beta_d, D,
	     do_dump_pos, do_log_events, verbose, clear_all, results_dir);
}

Queue::Queue(const string infile,
	     const double t,
	     const double L, const double lc, const double lin, const double v0,
	     const double alpha_s, const double beta_s,
	     const double alpha_d, const double beta_d,
             const double D,
	     const bool do_dump_pos, const bool do_log_events,
	     const bool verbose,
	     const bool clear_all,
	     const string results_dir){
  vector<pair<int, double>> state = file_to_state(infile);
  this->init(state, t, L, lc, lin, v0, alpha_s, beta_s, alpha_d, beta_d, D,
	     do_dump_pos, do_log_events, verbose, clear_all, results_dir);
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
  if (this->log_events_flag){
    this->deaths_file.close();
    this->births_file.close();
  }
}

void Queue::init(const vector<pair<int, double>> state,
		 const double t,
		 const double L, const double lc, const double lin, const double v0,
		 const double alpha_s, const double beta_s,
		 const double alpha_d, const double beta_d,
		 const double D,
		 const bool do_dump_pos, const bool do_log_events,
		 const bool verbose, const bool clear_all,
		 const string results_dir
		 ){
  if (verbose) {
    cout << "Creating queue!" << endl;
  }

  this->t = t;
  this->L = L;
  this->dump_pos_flag = do_dump_pos;
  this->log_events_flag = do_log_events;
  this->verbose_flag = verbose;
  this->results_dir = results_dir;

  if (clear_all && fs::is_directory(results_dir)){
    std::uintmax_t n_removed = fs::remove_all(results_dir);
    if (this->verbose())
      cout << "Removed " << n_removed << " files." << endl;
  }
  if (!fs::is_directory(results_dir)){
    if (this->verbose())
      cout << "Creating directory: " << results_dir << endl;
    fs::create_directory(results_dir);
    fs::create_directory(results_dir + "/checkpoint");
  }

  this->file.open(this->get_results_dir() + "/tdata.dat", ofstream::out | ofstream::app);
  if (this->log_events_flag){
    this->deaths_file.open(this->get_results_dir() + "/deaths.dat", ofstream::out | ofstream::app);
    this->births_file.open(this->get_results_dir() + "/births.dat", ofstream::out | ofstream::app);
  }
  
  this->load_state(state);

  this->v0 = v0;
  this->lc = lc;
  this->lin = lin;
  this->alpha_s = alpha_s;
  this->beta_s = beta_s;
  this->alpha_d = alpha_d;
  this->beta_d = beta_d;
  this->D = D;
}

void Queue::load_list(const double *li, const int N){
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
}

void Queue::load_state(const vector<pair<int, double>> state){
  Node *n = new Node(state[0].second, state[1].first, this);
  Node *n_prev, *n_first;

  n_first = n;
  for (vector<pair<int, double>>::const_iterator it=state.begin(); it != state.end(); it++){
    n_prev = n;
    n = new Node(it->second, it->first, this);
    n->set_prev(n_prev);
    n_prev->set_next(n);
  }
  n->set_next(n_first);
  n_first->set_prev(n);
  this->first = n_first;
}

double* Queue::export_list(int &N) const {
  Node *n = this->first;
  N = this->size();
  double* li = new double[N];
  int i = 0;
  do {
    li[i] = n->pos();
    ++i;
    n = n->next();
  } while (n != this->first);
  return li;
}

vector<pair<int, double>> Queue::export_state() const {
  Node *n = this->first;
  vector<pair<int, double>> state;
  do {
    state.push_back(make_pair(n->get_id(), n->pos()));
    n = n->next();
  } while (n != this->first);
  return state;
}

void Queue::dump_stats(){
  Node *n = this->first;
  double x_sum = 0.;
  double N = this->size();
  do {
    x_sum += n->dump_pos();
    n = n->next();
  } while(n != this->first);
  this->file << this->time() << " " << x_sum/N << " " << N << endl;
}

void Queue::step(const double dt){
  this->t += dt;
  ++this->it;
  Node *n = this->first;
  double expmllc_up, expmllc_down, l_up, l_down, dx, u;
  // double dx_max;
  double Pd, Ps;
  //double R;
  bool is_first;
  bool try_decay_first, try_decay, has_decayed, has_split;
  //bool has_split_or_decayed;
  do {
    l_up = n->dist_upstream();
    l_down = n->dist_downstream(); // Strictly not necessary to do this twice
    
    expmllc_up = exp(-l_up/this->lc);
    u = this->v(expmllc_up);
    Pd = 1.-exp(-this->decay_rate(expmllc_up)*dt);
    
    dx = u*dt;
    if (this->D > 0.0){
      dx += sqrt(D*dt)*gaussrd(e2);
    }
    n->to_move(dx);  // Since it is not quite correct to move immediately

    if (l_down > this->lin){
      expmllc_down = exp(-l_down/this->lc);
      Ps = 1.-exp(-this->splitting_rate(expmllc_down)*dt);
    }
    else {
      Ps = 0.0;
    }
    
    is_first = bool(n == this->first);
    has_decayed = false;
    has_split = false;
    try_decay_first = rand() % 2 == 1;
    for (int k = 0; k < 2; ++k){
      try_decay = (try_decay_first && k == 0) || (!try_decay_first && k == 1);
      if (try_decay && !has_split && rand_uniform_unit() < Pd){
	if (log_events_flag){
	  this->deaths_file << this->t << " " << n->get_id() << endl;
	}
	n = n->decay();
	if (is_first && n != NULL){
	  this->set_first(n->prev());
	}
	else if (n == NULL){
	  this->set_first(NULL);
	}
	has_decayed = true;
      }
      if (!try_decay && Ps > 0.0 && !has_decayed && rand_uniform_unit() < Ps){
	n = n->split(this->lin);
	if (log_events_flag){
	  this->births_file << this->t << " " << n->get_id() << " " << n->prev()->get_id() << endl;
	}
	if (is_first){
	  this->set_first(n->prev());
	}
	has_split = true;
      }
    }
    if (!has_decayed && !has_split) {
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

double Queue::v(const double expmllc) const {
  // Velocity
  return this->v0*expmllc;
}

double Queue::decay_rate(const double expmllc) const {
  return exp(-exp(this->alpha_d + this->beta_d*expmllc));
}

double Queue::splitting_rate(const double expmllc) const {
  return exp(-exp(this->alpha_s + this->beta_s*expmllc));
}

void Queue::set_first(Node* n){
  this->first = n;
}

void Queue::log_gaps(){
  Node* n = this->first;
  do {
    this->gaps.insert(n->dist_upstream());
    n = n->next();
  } while (n != this->first);
}

void Queue::dump_gaps(const string filename){
  ofstream gaps_file(filename, ofstream::out);
  for (set<double>::iterator it = this->gaps.begin(); it != gaps.end(); ++it){
    gaps_file << *it << endl;
  }
  gaps.clear();
  gaps_file.close();
}

void Queue::dump_gaps(const double t){
  string filename = this->get_results_dir() + "/gaps_t" + to_string(t) + ".dat";
  this->dump_gaps(filename);
}

void Queue::dump_gaps(){
  string filename = this->get_results_dir() + "/gaps.dat";
  this->dump_gaps(filename);
}

void Queue::read_state_from_file(const string infile){
  vector<pair<int, double>> state = file_to_state(infile);
  this->load_state(state);
}

void Queue::dump_state_to_file(const string outfile) const {
  vector<pair<int, double>> state = this->export_state();
  state_to_file(state, outfile);
}

double rand_uniform_unit() {
  return double(rand())/RAND_MAX;
}
