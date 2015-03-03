/**
  * Author: Bert Heinzelman
  * Date: 2/20/15
  * File:SymbolTable.h
  */

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include <iostream>
#include <map>

typedef std::pair<int, void*> Entry;
typedef std::map<std::string, Entry> Environment;

class SymbolTable {
 public:
  SymbolTable();
  bool lookup(std::string var_name, int& type);
  void addVariable(std::string x, int type);
  void push_environment();
  void pop_environment();

  //values
  void set_value(std::string var_name, void* value, int type);  void* get_value(std::string var_name, int& type);
 private:
  std::vector<Environment*> scopes;
};
#endif
