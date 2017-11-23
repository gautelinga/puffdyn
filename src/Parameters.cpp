#include <iostream>
#include <string>
#include <map>
#include "Parameters.h"
using namespace std;


Parameters::Parameters(int argc, char* argv[]){
  size_t found;
  for (int iarg=1; iarg < argc; ++iarg){
    string argstr = string(argv[iarg]);
    found = argstr.find('=');
    if (found != string::npos){
      set(argstr.substr(0, found), argstr.substr(found+1));
    }
  }
}

string Parameters::get(string key){
  map<string, string>::iterator it = params.find(key);
  if (it != params.end()){
    return it->second;
  }
  else {
    return "";
  }
}

string Parameters::get(string key, string default_value){
  string value = get(key);
  if (value != ""){
    return value;
  }
  else {
    return default_value;
  }
}

int Parameters::get(string key, int default_value){
  string value = get(key);
  if (value != ""){
    return stoi(value);
  }
  else {
    return default_value;
  }
}

double Parameters::get(string key, double default_value){
  string value = get(key);
  if (value != ""){
    return stod(value);
  }
  else {
    return default_value;
  }
}

bool Parameters::get(string key, bool default_value){
  string value = get(key);
  if (value == "True" || value == "true"){
    return true;
  }
  else {
    return default_value;
  }
}

void Parameters::dump(){
  for (map<string, string>::const_iterator it=params.begin(); it != params.end(); ++it){
    cout << it->first << ": " << it->second << endl;
  }
}