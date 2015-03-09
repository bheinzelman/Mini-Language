/* implementation file for ast classes
 * Author: Bert Heinzelman
 * Date: 2/15/15
 * File: ast.cpp
 */

#include <iostream>
#include <string>
#include "ast.h"
#include "Token.h"
#include "SymbolTable.h"
#include <stdlib.h> 

const std::string INDENTATION = "  ";

//last interpreted values
int it_int = 0;
double it_float = 0.0;
bool it_bool = false;
std::string it_string = "";
int it_expr_type = -1;

//if expression has a connector
char op = ' ';
int bop = -1;

bool not_flag = false;

//evaluation functions
float add(double a, char op, double c);
int add(int a, char op, int c);
std::string add(std::string a, char op, std::string c);

bool comp(int a, int op, int b);
bool comp(double a, int op, double b);
bool comp(std::string a, int op, std::string b);

/*
 * StmtList implementation
 */

#pragma region StmtList 
void StmtList::print(std::ostream& out, std::string indent_amt)
{
	out << indent_amt << "StmtList: " << std::endl;
	for (int i = 0; i < stmts.size(); i++)
		stmts[i]->print(out, indent_amt + INDENTATION);
}

void StmtList::add_stmt(Stmt * s)
{
	stmts.push_back(s);
}

bool StmtList::type_safe(SymbolTable& sym_table, std::ostream& out) {
	sym_table.push_environment(); // create a new environment
	for (int i = 0; i < stmts.size(); i++) // check statements
		if (!stmts[i]->type_safe(sym_table, out))
			return false;
	sym_table.pop_environment(); // remove new environment
	return true;
}

void StmtList::eval(SymbolTable& sym_table, std::ostream& os) {
	sym_table.push_environment();
	for (int i = 0; i < stmts.size(); i++)
		stmts[i]->eval(sym_table, os);
	sym_table.pop_environment();
}
#pragma endregion StmtList


/*
 * BasicIf implementation
 */

#pragma region BasicIf
void BasicIf::set_if_expr(BoolExpr* new_if_expr) {
	if (new_if_expr != NULL)
		if_expr = new_if_expr;
}

void BasicIf::set_if_stmts(StmtList* new_if_stmts) {
	if_stmts = new_if_stmts;
}

BoolExpr*  BasicIf::get_if_expr(){
	return if_expr;
}

StmtList*  BasicIf::get_if_stmts() {
	return if_stmts;
}
#pragma endregion BasicIf

/*
 * IfStmt implementation
 */

#pragma region IfStmt
IfStmt::IfStmt() {
	has_else_stmt = false;
}

void IfStmt::print(std::ostream& out, std::string indent_amt) {
	out << indent_amt << "IfStmt: " << std::endl;
	out << indent_amt << "If:" << std::endl;
	if_part->get_if_expr()->print(out, indent_amt + INDENTATION);
	out << indent_amt << "THEN: " << std::endl;
	if_part->get_if_stmts()->print(out, indent_amt + INDENTATION);
	for (int i = 0; i < elseifs.size(); i++) {
		out << indent_amt << "ELSE IF:" << std::endl;
		elseifs[i]->get_if_expr()->print(out, indent_amt + INDENTATION);
		out << indent_amt << "THEN: " << std::endl;
		elseifs[i]->get_if_stmts()->print(out, indent_amt + INDENTATION);
	}
	if (has_else_stmt) {
		out << indent_amt << "ELSE:" << std::endl;
		else_stmts->print(out, indent_amt + INDENTATION);
	}
	out << indent_amt << "END: " << std::endl;
}

void IfStmt::set_basic_if(BasicIf* new_if_part) {
	if_part = new_if_part;
}

void IfStmt::add_else_if(BasicIf * else_if_part) {
	if (else_if_part != NULL)
		elseifs.push_back(else_if_part);
}

void IfStmt::set_else_stmts(StmtList* new_else_stmts) {
	has_else_stmt = true;
	else_stmts = new_else_stmts;
}

bool IfStmt::type_safe(SymbolTable& sym_table, std::ostream& out) {
	sym_table.push_environment();
	if (!if_part->get_if_expr()->type_safe(sym_table, out))
		return false;

	if (!if_part->get_if_stmts()->type_safe(sym_table, out))
		return false;

	for (int i = 0; i < elseifs.size(); i++) {// check statements
		if (!elseifs[i]->get_if_expr()->type_safe(sym_table, out))
			return false;
		if (!elseifs[i]->get_if_stmts()->type_safe(sym_table, out))
			return false;
	}

	if (has_else_stmt) {
		if (!else_stmts->type_safe(sym_table, out))
			return false;
	}
	sym_table.pop_environment();
	return true;
}

void IfStmt::eval(SymbolTable& sym_table, std::ostream& os) {
	sym_table.push_environment();
	it_expr_type = -1;
	bop = -1;
	if_part->get_if_expr()->eval(sym_table, os);
	if (it_bool) {
		if_part->get_if_stmts()->eval(sym_table, os);
		return;
	}
	for (int i = 0; i < elseifs.size(); i++) {
		it_expr_type = -1;
		bop = -1;
		elseifs[i]->get_if_expr()->eval(sym_table, os);
		if (it_bool) {
			elseifs[i]->get_if_stmts()->eval(sym_table, os);
			return;
		}
	}
	if (has_else_stmt) {
		else_stmts->eval(sym_table, os);
	}
	sym_table.pop_environment();
}
#pragma endregion IfStmt

/*
 *  WhileStmt class implementation
 */

#pragma region WhileStmt
void WhileStmt::print(std::ostream& out, std::string indent_amt) {
	out << indent_amt << "WhileStmt: " << std::endl;
	while_expr->print(out, indent_amt + INDENTATION);
	out << indent_amt << "DO:" << std::endl;
	while_stmts->print(out, indent_amt + INDENTATION);
	out << indent_amt << "END:" << std::endl;
}

void WhileStmt::set_while_expr(BoolExpr * new_while_expr) {
	while_expr = new_while_expr;
}

void WhileStmt::set_while_stmts(StmtList * new_while_stmts) {
	while_stmts = new_while_stmts;
}

bool WhileStmt::type_safe(SymbolTable& sym_table, std::ostream& out) {
	sym_table.push_environment();
	if (!while_expr->type_safe(sym_table, out))
		return false;
	if (!while_stmts->type_safe(sym_table, out))
		return false;
	sym_table.pop_environment();
	return true; 
}

void WhileStmt::eval(SymbolTable& sym_table, std::ostream& os) {
	sym_table.push_environment();
	it_expr_type = -1;
	bop = -1;
	while_expr->eval(sym_table, os);
	while (it_bool) {
		it_expr_type = -1;
		while_stmts->eval(sym_table, os);
		it_expr_type = -1;
		bop = -1;
		while_expr->eval(sym_table, os);
	}
	sym_table.pop_environment();
}
#pragma endregion WhileStmt

/*
 * PrintStmt implementation
 */

#pragma region PrintStmt
void PrintStmt::print(std::ostream& out, std::string indent_amt) {
	if (print_type == PRINT_LN) {
		out << indent_amt << "PrintStmt: PRINTLN " << std::endl;
	}
	else {
		out << indent_amt << "PrintStmt: PRINT " << std::endl;
	}
	print_expr->print(out, indent_amt + INDENTATION);
}

void PrintStmt::set_print_type(int new_print_type) {
	print_type = new_print_type;
}

void  PrintStmt::set_print_expr(Expr * new_print_expr) {
	print_expr = new_print_expr;
}

bool PrintStmt::type_safe(SymbolTable& sym_table, std::ostream& out) { 
	int expr_type = -1;
	if (print_expr->calc_type(sym_table, expr_type, out))
		return true; 
	return false;
}

void PrintStmt::eval(SymbolTable& sym_table, std::ostream& os) {
	it_expr_type = -1;
	print_expr->eval(sym_table, os);
	if (it_expr_type == STRING_TYPE) {
		os << it_string;
	}
	else if(it_expr_type == INT_TYPE) {
		os << it_int;
	}
	else if (it_expr_type == FLOAT_TYPE) {
		os << it_float;
	}
	else if (it_expr_type == BOOL_FALSE || it_expr_type == BOOL_TRUE) {
		if (!it_bool) {
			os << "false";
		}
		else if (it_bool) {
			os << "true";
		}
	}
	if (print_type == PRINT_LN)
		os << std::endl;
}
#pragma endregion PrintStmt

/*
 * Assignment implementation
 */

#pragma region AssignStmt
void AssignStmt::print(std::ostream& out, std::string indent_amt) {
	out << indent_amt << "AssignStmt: " << std::endl 
		<< indent_amt + INDENTATION << "ID: " << id.lexeme() << std::endl;
	assign_expr->print(out, indent_amt + INDENTATION);

}

void AssignStmt::set_lhs_id(Token new_id) {
	id = new_id;
}

void AssignStmt::set_rhs_expr(Expr* new_assign_expr) {
	assign_expr = new_assign_expr;
}

bool AssignStmt::type_safe(SymbolTable& sym_table, std::ostream& out) { 
	int id_type = -1;
	int expr_type = -1;

	if (!assign_expr->calc_type(sym_table, expr_type, out))
		return false;
	if (!sym_table.lookup(id.lexeme(), id_type)) {
		sym_table.addVariable(id.lexeme(), expr_type);
		return true;
	}
	if (id_type == BOOL_TRUE || BOOL_FALSE) {
		if (expr_type == BOOL_TRUE || expr_type == BOOL_FALSE) {
			return true;
		}
	}
	if (id_type != expr_type) {
		out << "ERROR: type mismatch on line " << id.line() << "\n";
		return false;
	}
	return true;

}

void AssignStmt::eval(SymbolTable& sym_table, std::ostream& os) {
	it_expr_type = -1;
	assign_expr->eval(sym_table, os);
	if (it_expr_type == STRING_TYPE) {
		std::string* val = new std::string(it_string);
		sym_table.set_value(id.lexeme(), val, STRING_TYPE);
	}
	else if (it_expr_type == INT_TYPE) {
		int* val = new int(it_int);
		sym_table.set_value(id.lexeme(), val, INT_TYPE);
	}
	else if (it_expr_type == FLOAT_TYPE) {
		double* val = new double(it_float);
		sym_table.set_value(id.lexeme(), val, FLOAT_TYPE);
	}
	else if (it_expr_type == BOOL_FALSE || it_expr_type == BOOL_TRUE) {
		bool* val;
		if (it_bool) {
			val = new bool(true);
			sym_table.set_value(id.lexeme(), val, BOOL_TRUE);
		}
		else {
			val = new bool(false);
			sym_table.set_value(id.lexeme(), val, BOOL_FALSE);
		}
		
	}
	
}
#pragma endregion AssignStmt

/*
 * Simple Expr implementation
 */

#pragma region SimpleExpr
void SimpleExpr::print(std::ostream& out, std::string indent_amt) {
	out << indent_amt << "Simple Expr: " << tokens[term.type()] << "("
		<< term.lexeme() << ")" << std::endl;
}

void SimpleExpr::set_token(Token new_term) {
	term = new_term;
}

bool SimpleExpr::calc_type(SymbolTable& sym_table, int& type, std::ostream& out) {
	int new_type;
	if (term.type() == ID) {
		if (!sym_table.lookup(term.lexeme(), new_type)) {
			out << "Use of undeclared id: " << term.lexeme() << std::endl;
			return false;
		}
	}
	else {
		new_type = term.type();
	}
	if (type != -1) {
		if (new_type != type) {
			out << "Expression with type mismatch Line:" << term.line() << std::endl;
			return false;
		}
	}

	type = new_type;
	return true;
}

void SimpleExpr::eval(SymbolTable& sym_table, std::ostream& os) {
	int prev_type = it_expr_type;
	it_expr_type = term.type();
	if (it_expr_type == INT_TYPE) {
		int val = atoi(term.lexeme().c_str());
		if (prev_type == -1)
			it_int = val;
		else
			it_int = add(it_int, op, val);
	}
	else if (it_expr_type == FLOAT_TYPE) {
		double val = atof(term.lexeme().c_str());
		if (prev_type == -1)
			it_float = -1;
		else
			it_float = add(it_float, op, val);
	}
	else if (it_expr_type == STRING_TYPE) {
		std::string val = term.lexeme();
		if (prev_type == -1)
			it_string = val;
		else
			it_string = add(it_string, op, val);
	}
	else if (it_expr_type == BOOL_FALSE) {
		it_bool = false;
	}
	else if (it_expr_type == BOOL_TRUE) {
		it_bool = true;
	}
	else if (it_expr_type == ID) {
		int type = -1;
		void* val = (sym_table.get_value(term.lexeme(), type));
		if (type == INT_TYPE) {
			int i_val = *((int*)val);
			if (prev_type == -1)
				it_int = i_val;
			else
				it_int = add(it_int, op, i_val);
			it_expr_type = INT_TYPE;
		}
		else if (type == FLOAT_TYPE) {
			double f_val = *((float*)val);
			if (prev_type == -1)
				it_float = f_val;
			else
				it_float = add(it_float, op, f_val);
			it_expr_type = FLOAT_TYPE;
		}
		else if (type == STRING_TYPE) {
			std::string s_val = *((std::string*)val);
			if (prev_type == -1)
				it_string = s_val;
			else
				it_string = add(it_string, op, s_val);
			it_expr_type = STRING_TYPE;
		}
		else if (type == BOOL_TRUE) {
			it_expr_type = BOOL_TRUE;
			it_bool = true;
		}
		else if (type == BOOL_FALSE) {
			it_expr_type = BOOL_FALSE;
			it_bool = false;
		}
	}
}

#pragma endregion SimpleExpr

/*
 * ReadExpr implementation
 */

#pragma region ReadExpr
void ReadExpr::print(std::ostream& out, std::string indent_amt) {
	std::string x;
	if (read_type == READ_INT)
		x = "READ_INT";
	else if (read_type == READ_FLOAT)
		x = "READ_FLOAT";
	else
		x = "READ_STRING";
	out << indent_amt << "ReadExpr: " << x << "(" << msg.lexeme() << ")" << std::endl;
}

void ReadExpr::set_msg(Token new_msg) {
	msg = new_msg;
}

void ReadExpr::set_read_type(int new_read_type) {
	read_type = new_read_type;
}

bool ReadExpr::calc_type(SymbolTable& sym_table, int& type, std::ostream& out) {
	if (read_type == READ_INT) {
		type = INT_TYPE;
		return true;
	}
	if (read_type == READ_FLOAT) {
		type = FLOAT_TYPE;
		return true;
	}
	if (read_type == READ_STRING) {
		type = STRING_TYPE;
		return true;
	}
	out << "Unrecognized read expr\n";
	return false;
}

void ReadExpr::eval(SymbolTable& sym_table, std::ostream& os) {
	os << msg.lexeme();
	if (read_type == READ_INT) {
		std::cin >> it_int;
		it_expr_type = INT_TYPE;
	}
	else if (read_type == READ_FLOAT) {
		std::cin >> it_float;
		it_expr_type = FLOAT_TYPE;
	}
	else if (read_type == READ_STRING) {
		std::cin >> it_string;
		it_expr_type = STRING_TYPE;
	}
}
#pragma endregion ReadExpr

/*
 * ComplexExpr implementation
 */

#pragma region ComplexExpr
void ComplexExpr::print(std::ostream& out, std::string indent_amt) {
	out << indent_amt << "ComplexExpr: " << std::endl;
	first_operand->print(out, indent_amt + INDENTATION);
	if (math_rel == ADD) {
		out << indent_amt + INDENTATION << "+" << std::endl;
	}
	else if (math_rel == SUBTRACT) {
		out << indent_amt + INDENTATION << "-" << std::endl;
	}
	else if (math_rel == MULTIPLY) {
		out << indent_amt + INDENTATION << "*" << std::endl;
	}
	else {
		out << indent_amt + INDENTATION << "/" << std::endl;
	}
	rest->print(out, indent_amt + INDENTATION);
}

void ComplexExpr::set_first_operand(Expr * new_first_operand) {
	first_operand = new_first_operand;
}

void ComplexExpr::set_math_rel(int new_math_rel) {
	math_rel = new_math_rel;
}

void ComplexExpr::set_rest(Expr * new_rest) {
	rest = new_rest;
}

bool ComplexExpr::calc_type(SymbolTable& sym_table, int& type, std::ostream& out) {
	int new_type = -1;
	if (!first_operand->calc_type(sym_table, new_type, out)) {
		return false;
	}
	if (new_type == STRING_TYPE) {
		if (math_rel != ADD) {
			out << "Invalid string operator " << std::endl;
			return false;
		}
	} 
	type = new_type;
	return rest->calc_type(sym_table, type, out);
}

void ComplexExpr::eval(SymbolTable& sym_table, std::ostream& os) {
	first_operand->eval(sym_table, os);
	if (math_rel == ADD)
		op = '+';
	else if (math_rel == SUBTRACT)
		op = '-';
	else if (math_rel == DIVIDE)
		op = '/';
	else if (math_rel == MULTIPLY)
		op = '*';
	rest->eval(sym_table, os);
}
#pragma endregion ComplexExpr

/*
  SimpleBool expr
 */

#pragma region SimpleBoolExpr
void SimpleBoolExpr::print(std::ostream& out, std::string indent_amt) {
	out << indent_amt << "SimpleBoolExpr: " << std::endl;
	expr_term->print(out, indent_amt + INDENTATION);
}

bool SimpleBoolExpr::type_safe(SymbolTable& sym_table, std::ostream& out) { 
	int type = -1;
	return expr_term->calc_type(sym_table, type, out);
}

void SimpleBoolExpr::set_expr_term(Expr * new_expr_term) {
	expr_term = new_expr_term;
}

void SimpleBoolExpr::eval(SymbolTable& sym_table, std::ostream& os) {
	expr_term->eval(sym_table, os);
}
#pragma endregion SimpleBoolExpr

/*
 * ComplexBoolExpr implementation
 */

#pragma region ComplexBoolExpr
ComplexBoolExpr::ComplexBoolExpr() {
	has_bool_op = false;
}

void ComplexBoolExpr::print(std::ostream& out, std::string indent_amt) {
	out << indent_amt << "ComplexBoolExpr: " << std::endl;
	first_operand->print(out, indent_amt + INDENTATION);
	std::string x;
	if (bool_rel == LESS_THAN)
		x = "<";
	else if (bool_rel == LESS_THAN_EQ)
		x = "<=";
	else if (bool_rel == EQUAL) 
		x = "==";
	else if (bool_rel == NOT_EQUAL) 
		x = "!=";
	else if (bool_rel == GREATER_THAN)
		x = ">";
	else
		x = ">=";
	out << indent_amt << INDENTATION << x << std::endl;
	second_operand->print(out, indent_amt + INDENTATION);
	if (has_bool_op) {
		std::string connect = (bool_connector_type == AND) ? ("AND") : ("OR");
		out << indent_amt << INDENTATION << connect << std::endl;
		rest->print(out, indent_amt + INDENTATION);
	}
}
void ComplexBoolExpr::set_first_operand(Expr * new_first_operand) {
	first_operand = new_first_operand;
}

void ComplexBoolExpr::set_rel_type(int new_bool_rel) {
	bool_rel = new_bool_rel;
}

void ComplexBoolExpr::set_second_operand(Expr * new_second_operand) {
	second_operand = new_second_operand;
}

void ComplexBoolExpr::set_bool_connector_type(int new_bool_connector_type){
	has_bool_op = true;
	bool_connector_type = new_bool_connector_type;
}
void ComplexBoolExpr::set_rest(BoolExpr * new_rest) {
	rest = new_rest;
}

bool ComplexBoolExpr::type_safe(SymbolTable& sym_table, std::ostream& out) {
	int type  = -1;
	int type2 = -1;
	if (!first_operand->calc_type(sym_table, type, out))
		return false;
	if (!second_operand->calc_type(sym_table, type2, out))
		return false;
	if (type != type2) {
		out << "Expression type error" << std::endl;
	}
	if (has_bool_op) {
		if (!rest->type_safe(sym_table, out))
			return false;
	}
	return true;
}

void ComplexBoolExpr::eval(SymbolTable& sym_table, std::ostream& os) {
	first_operand->eval(sym_table, os);

	int prevb = bop; //if -1 then new bool expr
	bool new_bool = false; // new bool value
	
	if (it_expr_type == INT_TYPE) {
		int val_1 = it_int;
		it_expr_type = -1;
		second_operand->eval(sym_table, os);
		int val_2 = it_int;
		new_bool = comp(val_1, bool_rel, val_2);
	}
	else if (it_expr_type == FLOAT_TYPE) {
		double val_1 = it_int;
		it_expr_type = -1;
		second_operand->eval(sym_table, os);
		double val_2 = it_int;
		new_bool = comp(val_1, bool_rel, val_2);
	}
	else if (it_expr_type == STRING_TYPE) {
		std::string val_1 = it_string;
		it_expr_type = -1;
		second_operand->eval(sym_table, os);
		std::string val_2 = it_string;
		new_bool = comp(val_1, bool_rel, val_2);
	}

	if (prevb != -1) {
		if (prevb == AND)
			it_bool = it_bool && new_bool;
		else if (prevb == OR)
			it_bool = it_bool || new_bool;
	}
	else
		it_bool = new_bool;
	if (not_flag) {
		it_bool = !it_bool;
		not_flag = false;
	}
	if (has_bool_op) {
		bop = bool_connector_type;
		it_expr_type = -1;
		rest->eval(sym_table, os);
	}
	
}
#pragma endregion ComplexBoolExpr

/*
 * NotBoolExpr
 */

#pragma region NotBoolExpr
void NotBoolExpr::print(std::ostream& out, std::string indent_amt) {
	out << indent_amt << "NotBoolExpr: " << std::endl;
	bool_expr->print(out, indent_amt + INDENTATION);
}

void NotBoolExpr::set_bool_expr(BoolExpr * new_bool_expr) {
	bool_expr = new_bool_expr;
}

bool NotBoolExpr::type_safe(SymbolTable& sym_table, std::ostream& out) { 
	if (!bool_expr->type_safe(sym_table, out))
		return false;
	return true;
}

void NotBoolExpr::eval(SymbolTable& sym_table, std::ostream& os) {
	not_flag = !not_flag;
	bool_expr->eval(sym_table, os);
}
#pragma endregion NotBoolExpr

//helper functions
float add(double a, char op, double c) {
	if (op == '+')
		return a + c;
	if (op == '-')
		return a - c;
	if (op == '*')
		return a * c;
	return a / c;
}
int add(int a, char op, int c) {
	if (op == '+')
		return a + c;
	if (op == '-')
		return a - c;
	if (op == '*')
		return a * c;
	return a / c;
}
std::string add(std::string a, char op, std::string c) {
	return a + c;
}

bool comp(int a, int op, int b) {
	if (op == GREATER_THAN)
		return a > b;
	if (op == GREATER_THAN_EQ)
		return a >= b;
	if (op == EQUAL)
		return a == b;
	if (op == LESS_THAN)
		return a < b;
	if (op == LESS_THAN_EQ)
		return a <= b;
	return a != b;
}
bool comp(double a, int op, double b) {
	if (op == GREATER_THAN)
		return a > b;
	if (op == GREATER_THAN_EQ)
		return a >= b;
	if (op == EQUAL)
		return a == b;
	if (op == LESS_THAN)
		return a < b;
	if (op == LESS_THAN_EQ)
		return a <= b;
	return a != b;
}
bool comp(std::string a, int op, std::string b) {
	if (op == GREATER_THAN)
		return a > b;
	if (op == GREATER_THAN_EQ)
		return a >= b;
	if (op == EQUAL)
		return a == b;
	if (op == LESS_THAN)
		return a < b;
	if (op == LESS_THAN_EQ)
		return a <= b;
	return a != b;
}
