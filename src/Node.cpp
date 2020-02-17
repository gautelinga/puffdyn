#include "Node.hpp"
#include <iomanip>

int COUNT = 0;

Node::Node(double x, Queue* queue){
  this->x = x;
  this->id = COUNT++;
  this->queue = queue;
  if (this->queue->verbose())
    cout << "Creating node " << this->id << " at x=" << x << "!"  << endl;
  if (this->queue->do_dump_pos()){
    this->file.open(this->queue->get_results_dir() + "/xt_" + to_string(this->id)+ ".dat",
		    ofstream::out | ofstream::app);
    this->file << this->queue->time() << " " << setprecision(9) << this->x << endl;
  }
  this->queue->increase_size();
}

Node::Node(double x, int id, Queue* queue){
  this->x = x;
  this->id = id;
  COUNT = max(COUNT, id)+1;
  this->queue = queue;
  if (this->queue->verbose())
    cout << "Creating node " << this->id << " at x=" << x << "!"  << endl;
  if (this->queue->do_dump_pos()){
    this->file.open(this->queue->get_results_dir() + "/xt_" + to_string(this->id)+ ".dat",
		    ofstream::out | ofstream::app);
    this->file << this->queue->time() << " " << setprecision(9) << this->x << endl;
  }
  this->queue->increase_size();
}

Node::~Node(void){
  if (this->queue->verbose())
    cout << "Destroying " << this->id << "!" << endl;
  if (this->queue->do_dump_pos()){
    this->file.close();
  }
  this->queue->decrease_size();
}

double Node::dist_upstream() const {
  double L = this->queue->domain_size();
  if (this == this->prev()){
    return L;
  }
  double x_prev = fmod(this->prev()->x, L);
  double x = fmod(this->x, L);
  if (x_prev > x){
    x_prev -= L;
  }
  return x - x_prev;;
}

double Node::dist_downstream() const {
  double L = this->queue->domain_size();
  if (this == this->prev()){
    return L;
  }
  double x_next = fmod(this->next()->x, L);
  double x = fmod(this->x, L);
  if (x > x_next){
    x_next += L;
  }
  return x_next - x;
}

void Node::to_move(double dx){
  this->dx = dx;
}

void Node::move(){
  this->x += this->dx;
  this->dx = 0.;
}

Node* Node::decay(){
  Node* n_prev = this->prev();
  Node* n_next = this->next();
  n_prev->set_next(n_next);
  n_next->set_prev(n_prev);
  if (this->queue->do_dump_pos()){
    this->file << this->queue->time() << " " << this->x << endl;
  }
  if (this != n_next){
    delete this;
    return n_next;
  }
  else {
    delete this;
    return NULL;
  }
}

Node* Node::split(){
  return this->split(0.0);
}

Node* Node::split(double dx){
  Node* n_next = this->next();
  Node* n = new Node(this->x+dx, this->get_queue());
  n->to_move(this->dx);
  n_next->set_prev(n);
  n->set_next(n_next);
  n->set_prev(this);
  this->set_next(n);
  if (this->queue->do_dump_pos()){
    this->file << this->queue->time() << " " << this->x << endl;
  }
  return n;
}

double Node::dump_pos(){
  if (this->queue->do_dump_pos()){
    this->file << this->queue->time() << " " << this->x << endl;
  }
  return this->x;
}
