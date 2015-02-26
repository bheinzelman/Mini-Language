/* Header file for ast classes
 * Author: Bert Heinzelman
 * Date: 2/15/15
 * File: ast.h
 */

#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <ostream>
#include "Token.h"
#include "SymbolTable.h"


class ASTNode
{
public:
	virtual void print(std::ostream& out, std::string indent_amt) = 0;

};


class Stmt : public ASTNode
{
public:
	virtual bool type_safe(SymbolTable& sym_table, std::ostream& out) = 0;
	virtual void print(std::ostream& out, std::string indent_amt) = 0;
};


class StmtList : public ASTNode
{
public:
	void print(std::ostream& out, std::string indent_amt);
	void add_stmt(Stmt* s);
	bool type_safe(SymbolTable& sym_table, std::ostream& out);
private:
	std::vector<Stmt *> stmts;
};


class Expr : public ASTNode
{
public:
	virtual void print(std::ostream& out, std::string indent_amt) = 0;
	virtual bool calc_type(SymbolTable& sym_table, int& type, std::ostream& out) = 0;
};


class BoolExpr : public ASTNode
{
public:
	virtual bool type_safe(SymbolTable& sym_table, std::ostream& out) = 0;
	virtual void print(std::ostream& out, std::string indent_amt) = 0;
};

class BasicIf  {
public:
	void set_if_expr(BoolExpr * new_if_expr);
	void set_if_stmts(StmtList * new_if_stmts);
	BoolExpr * get_if_expr();
	StmtList * get_if_stmts();
private:
	BoolExpr * if_expr;
	StmtList * if_stmts;
};

class IfStmt : public Stmt
{
public:
	IfStmt();
	void print(std::ostream& out, std::string indent_amt);
	void set_basic_if(BasicIf* new_if_part);
	void add_else_if(BasicIf* else_if_part);
	void set_else_stmts(StmtList* else_stmts);
	//
	bool type_safe(SymbolTable& sym_table, std::ostream& out);
private:
	BasicIf * if_part;
	std::vector<BasicIf *> elseifs;
	bool has_else_stmt;
	StmtList* else_stmts;
};


class WhileStmt : public Stmt
{
public:
	void print(std::ostream& out, std::string indent_amt);
	void set_while_expr(BoolExpr * while_expr);
	void set_while_stmts(StmtList * while_stmts);
	//
	bool type_safe(SymbolTable& sym_table, std::ostream& out);
private:
	BoolExpr * while_expr;
	StmtList * while_stmts;
};


class PrintStmt : public Stmt
{
public:
	void print(std::ostream& out, std::string indent_amt);
	void set_print_type(int print_type);
	void set_print_expr(Expr * print_expr);
	//
	bool type_safe(SymbolTable& sym_table, std::ostream& out);
private:
	Expr * print_expr;
	int print_type;
};


class AssignStmt : public Stmt
{
public:
	bool type_safe(SymbolTable& sym_table, std::ostream& out);
	void print(std::ostream& out, std::string indent_amt);
	void set_lhs_id(Token id);
	void set_rhs_expr(Expr * assign_expr);
private:
	Token id;
	Expr* assign_expr;
};


class SimpleExpr : public Expr
{
public:
	bool calc_type(SymbolTable& sym_table, int& type, std::ostream& out);
	void print(std::ostream& out, std::string indent_amt);
	void set_token(Token term);
private:
	Token term;
};


class ReadExpr : public Expr
{
public:
	bool calc_type(SymbolTable& sym_table, int& type, std::ostream& out);
	void print(std::ostream& out, std::string indent_amt);
	void set_msg(Token msg);
	void set_read_type(int read_type);
private:
	int read_type;
	Token msg;
};


class ComplexExpr : public Expr
{
public:
	
	void print(std::ostream& out, std::string indent_amt);
	void set_first_operand(Expr * first_operand);
	void set_math_rel(int math_rel);
	void set_rest(Expr * rest);
	//
	bool calc_type(SymbolTable& sym_table, int& type, std::ostream& out);
private:
	Expr * first_operand;
	int math_rel;
	Expr * rest;
};


class SimpleBoolExpr : public BoolExpr
{
public:
	void print(std::ostream& out, std::string indent_amt);
	void set_expr_term(Expr * expr_term);
	//
	bool type_safe(SymbolTable& sym_table, std::ostream& out);
	//bool calc_type(SymbolTable& sym_table, int& type, std::ostream& out);
private:
	Expr * expr_term;
};


class ComplexBoolExpr : public BoolExpr
{
public:
	ComplexBoolExpr();
	void print(std::ostream& out, std::string indent_amt);
	void set_first_operand(Expr * first_operand);
	void set_rel_type(int bool_rel);
	void set_second_operand(Expr * second_operand);
	void set_bool_connector_type(int bool_connector_type);
	void set_rest(BoolExpr * rest);
	//
	bool type_safe(SymbolTable& sym_table, std::ostream& out);
private:
	Expr * first_operand;
	int bool_rel;
	Expr * second_operand;
	bool has_bool_op;
	int bool_connector_type;
	BoolExpr * rest;
};


class NotBoolExpr : public BoolExpr
{
public:
	void print(std::ostream& out, std::string indent_amt);
	void set_bool_expr(BoolExpr * bool_expr);
	//
	bool type_safe(SymbolTable& sym_table, std::ostream& out);
private:
	BoolExpr * bool_expr;
};
#endif
