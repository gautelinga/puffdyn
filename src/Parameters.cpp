#include "Parameters.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <boost/algorithm/string.hpp>
using namespace std;


Parameters::Parameters(int argc, char* argv[]){
  parse_args(argc, argv);
  string restart_folder = get("restart_folder", "");
  if (restart_folder != ""){
    parse_file(restart_folder + "/params.dat");
    parse_args(argc, argv);
    set("state_file", restart_folder + "/state.dat");
    set_bool("clear_all", false);
  }
}

Parameters::Parameters(string infile){
  parse_file(infile);
}

void Parameters::parse_args(int argc, char* argv[]){
  size_t found;
  string argstr;
  for (int iarg=1; iarg < argc; ++iarg){
    argstr = string(argv[iarg]);
    found = argstr.find('=');
    if (found != string::npos){
      set(argstr.substr(0, found), argstr.substr(found+1));
    }
  }
}

void Parameters::parse_file(string infile){
  ifstream input(infile);
  if (!input){
    cout << "File " << infile <<" doesn't exist." << endl;
    cout << "Continuing..." << endl;
    return;
  }
  size_t found;
  string value;
  for (string line; getline(input, line); ){
    found = line.find('=');
    if (found != string::npos){
      value = line.substr(found+1);
      boost::trim_right(value);
      set(line.substr(0, found), value);
    }
  }
}

string Parameters::get(string key) const {
  map<string, string>::const_iterator it = params.find(key);
  if (it != params.end()){
    return it->second;
  }
  else {
    return "";
  }
}

string Parameters::get(string key, string default_value) const {
  string value = get(key);
  if (value != ""){
    return value;
  }
  else {
    return default_value;
  }
}

int Parameters::get(string key, int default_value) const {
  string value = get(key);
  if (value != ""){
    return stoi(value);
  }
  else {
    return default_value;
  }
}

double Parameters::get(string key, double default_value) const {
  string value = get(key);
  if (value != ""){
    return stod(value);
  }
  else {
    return default_value;
  }
}

bool Parameters::get_bool(string key, bool default_value) const {
  string value = get(key);
  if (value == "True" || value == "true"){
    return true;
  }
  else if (value == "False" || value == "false"){
    return false;
  }
  else {
    return default_value;
  }
}

void Parameters::dump() const {
  for (map<string, string>::const_iterator it=params.begin(); it != params.end(); ++it){
    cout << it->first << ": " << it->second << endl;
  }
}

void Parameters::dump(string outfile) const {
  ofstream output(outfile);
  for (map<string, string>::const_iterator it=params.begin();
       it != params.end(); ++it){
    output << it->first << "=" << it->second << endl;
  }
}
