/**
  * Author: Bert Heinzelman
  * Date: 2/20/15
  * File: SymbolTable.cpp
  */

#include "SymbolTable.h"

SymbolTable::SymbolTable() {

}

void SymbolTable::push_environment()
{
	Environment * env = new Environment;
	scopes.push_back(env);
}

void SymbolTable::pop_environment()
{
	if (scopes.size() == 0)
		return;
	Environment * env = scopes.back();
	scopes.pop_back();
	delete env;
}

void SymbolTable::addVariable(std::string var_name, int type)
{
	if (scopes.size() == 0)
		return;
	Environment * curr_env = scopes.back(); // get current env
	(*curr_env)[var_name] = type; // assign type to var
}

bool SymbolTable::lookup(std::string var_name, int& type)
{
	Environment* e;
	for (int i = scopes.size() - 1; i >= 0; i--) {
		e = scopes[i];
		if (e->count(var_name) > 0) {
			type = (*e)[var_name];
			return true;
		}
	}
	return false;
}

