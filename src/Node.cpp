#include "Node.h"

int COUNT = 0;

Node::Node(double x, string results_dir){
  cout << "Creating x=" << x << "!"  << endl;
  this->x = x;
  this->id = COUNT++;

  this->results_dir = results_dir;
  this->file.open(results_dir + to_string(this->id)+ ".dat", ofstream::out);
}

Node::~Node(void){
  cout << "Destroying " << this->id << "!" << endl;
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
  Node* n = new Node(this->x, this->results_dir);
  n_prev->set_next(n);
  n->set_prev(n_prev);
  n->set_next(this);
  return this->next();
}

void Node::dump(double t){
  //cout << "Taking a dump." << endl;
  this->file << t << " " << this->x << endl;
}
