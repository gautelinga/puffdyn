#include <string>
#include <map>
using namespace std;

#ifndef __PARAMETERS_H
#define __PARAMETERS_H

class Parameters {
public:
  Parameters(int argc, char* argv[]);
  void set(string key, string value){ params[key] = value; };
  string get(string key, string default_value);
  string get(string key, const char* default_value){ return get(key, string(default_value)); };
  int get(string key, int default_value);
  double get(string key, double default_value);
  bool get_bool(string key, bool default_value);
  string get(string key);
  void dump();
  //int get_int(string key, int default_value);
  //double get_double(string key, double default_value);
  map<string, string>::const_iterator begin(){ return params.begin(); };
  map<string, string>::const_iterator end(){ return params.end(); };
private:
  map<string, string> params;
};

#endif