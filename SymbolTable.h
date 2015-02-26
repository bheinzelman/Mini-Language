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

typedef std::map<std::string, int> Environment;

class SymbolTable {
 public:
  SymbolTable();
  bool lookup(std::string var_name, int& type);
  void addVariable(std::string x, int type);
  void push_environment();
  void pop_environment();
 private:
  std::vector<Environment*> scopes;
};
#endif
