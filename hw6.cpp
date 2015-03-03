/* Main file to test class parse
 * Author: Bert Heinzelman
 * Date: 2/22/15
 * File: hw6.cpp
 */

#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Lexer.h"
#include "Token.h"
#include "ast.h"
#include "SymbolTable.h"


int main(int argc, char* argv[]) {
	//checks for command line args
	if (argc != 2) {
		std::cout << "Invalid Arguments\n";
		return -1;
	}

	std::ifstream infile(argv[1]);
	//checks to see if valid file
	if (!infile) {
		std::cout << "Error opening file\n";
		return -2;
	}

	Lexer the_lexer(infile);

	Parser theParser = Parser(the_lexer);
	StmtList * root = theParser.Parse();
	SymbolTable table = SymbolTable();
	if (root != NULL) {
	  if (root->type_safe(table, std::cout)) {
	    SymbolTable s = SymbolTable();
	    root->eval(s, std::cout);
	  }
	}
}
