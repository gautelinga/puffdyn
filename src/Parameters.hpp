#include <string>
#include <map>
using namespace std;

#ifndef __PARAMETERS_H
#define __PARAMETERS_H

#include <boost/lexical_cast.hpp>

class Parameters {
public:
  Parameters(int argc, char* argv[]);
  Parameters(string infile);
  void set(string key, string value){ params[key] = value; }
  void set(string key, double value){ set(key, boost::lexical_cast<string>(value)); }
  void set(string key, int value){ set(key, boost::lexical_cast<string>(value)); }
  void set(string key, bool value){ set(key, boost::lexical_cast<string>(value)); }
  string get(string key, string default_value) const;
  string get(string key, const char* default_value) const { return get(key, string(default_value)); }
  int get(string key, int default_value) const;
  double get(string key, double default_value) const;
  bool get_bool(string key, bool default_value) const;
  string get(string key) const;
  void dump() const;
  void dump(string outfile) const;
  //int get_int(string key, int default_value);
  //double get_double(string key, double default_value);
  map<string, string>::const_iterator begin() const { return params.begin(); }
  map<string, string>::const_iterator end() const { return params.end(); }
private:
  map<string, string> params;
};

#endif
