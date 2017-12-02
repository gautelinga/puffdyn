#include "Node.h"

int COUNT = 0;

Node::Node(double x, Queue* queue){
  this->x = x;
  this->id = COUNT++;
  this->queue = queue;
  if (this->queue->verbose())
    cout << "Creating node " << this->id << " at x=" << x << "!"  << endl;
  if (this->queue->do_dump_pos()){
    this->file.open(this->queue->get_results_dir() + "/xt_" + to_string(this->id)+ ".dat", ofstream::out);
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

double Node::dist() const {
  double L = this->queue->domain_size();
  if (this == this->next()){
    return L;
  }
  double distx = this->next()->x-this->x;
  if (distx > L){
    distx -= L;
  }
  else if (distx < 0.){
    distx += L;
  }
  return distx;
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
  Node* n_prev = this->prev();
  Node* n = new Node(this->x, this->get_queue());
  n->to_move(this->dx);
  n_prev->set_next(n);
  n->set_prev(n_prev);
  n->set_next(this);
  this->set_prev(n);
  return this->next();
}

double Node::dump_pos(double t){
  if (this->queue->do_dump_pos()){
    this->file << t << " " << this->x << endl;
  }
  return this->x;
}