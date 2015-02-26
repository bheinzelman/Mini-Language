/**
  * Author: Bert Heinzelman
  * Date: 2/7/15
  * Modified: 2/15/15
  * File: Parser.cpp
  * Description: Implementation file for class Parser.
  */

#include "Parser.h"
#include "Lexer.h"
#include "Token.h"
#include "ast.h"
#include <iostream>

Parser::Parser(Lexer& l) : lex(l)
{
	currentToken = lex.next_token();
}

StmtList* Parser::Parse() {
	StmtList* stmtList = new StmtList();
	try {
		stmts(stmtList);
		Eat(EOS, "end of stream expected");
	} catch(ParseException& e) {
		std::cerr << e.what() << std::endl;
		return NULL;
	}
	return stmtList;
}


StmtList* Parser::stmts(StmtList* stmtList) {
	//assignment
	if (currentToken.type() == ID) {
		AssignStmt* assgn_node = new AssignStmt();
		assgn_node->set_lhs_id(currentToken);
		Advance();
		Eat(ASSIGNMENT, "Expexted '='");
		assgn_node->set_rhs_expr(expr());
		stmtList->add_stmt(assgn_node);
		Eat(SEMICOLON, "Expexted ';'");
		stmtsTail(stmtList);
	}
	//output
	else if (currentToken.type() == PRINT || currentToken.type() == PRINT_LN) {
		PrintStmt* print_node = new PrintStmt();
		print_node->set_print_type(currentToken.type());
		Advance();
		Eat(L_PAREN, "Expexted '('");
		print_node->set_print_expr(expr());
		Eat(R_PAREN, "Expexted ')'");
		Eat(SEMICOLON, "Expexted ';'");
		stmtList->add_stmt(print_node);
		stmtsTail(stmtList);
	}
	//conditional
	else if (currentToken.type() == IF) {
		BasicIf* basicIfNode = new BasicIf();
		IfStmt* if_node = new IfStmt();
		Advance();
		basicIfNode->set_if_expr(bexpr());
		Eat(THEN, "Expected 'then'");
		basicIfNode->set_if_stmts(stmts(new StmtList()));
		if_node->set_basic_if(basicIfNode);

		condTail(if_node);

		Eat(END, "Expected 'end'");
		stmtList->add_stmt(if_node);
		stmtsTail(stmtList);
	}
	//loop
	else if (currentToken.type() == WHILE) {
		WhileStmt* while_node = new WhileStmt();
		Advance();
		while_node->set_while_expr(bexpr());
		Eat(DO, "Expexted 'do'");
		while_node->set_while_stmts(stmts(new StmtList()));
		stmtList->add_stmt(while_node);
		Eat(END, "Expected 'end'");
		stmtsTail(stmtList);
	}
	return stmtList;
}

void Parser::stmtsTail(StmtList* stmtList) {
	if (currentToken.type() != EOS)
		stmts(stmtList);
}


void Parser::condTail(IfStmt* if_node) {
	if (currentToken.type() == ELSE) {
		Advance();
		if (currentToken.type() == IF) {
			BasicIf * b_node = new BasicIf();
			Advance();
			b_node->set_if_expr(bexpr());
			Eat(THEN, "Then Expected");
			b_node->set_if_stmts(stmts(new StmtList()));
			if_node->add_else_if(b_node);
			condTail(if_node);
		}
		else {
			if_node->set_else_stmts(stmts(new StmtList()));
		}
	}
}


Expr* Parser::expr() {
	SimpleExpr* sim_expr = new SimpleExpr();
	Token t = term();
	if (t.type() == READ_INT) {
		ReadExpr* e = new ReadExpr();
		e->set_msg(t);
		e->set_read_type(READ_INT);
		return exprTail(e);
	}
	else if (t.type() == READ_FLOAT) {
		ReadExpr* e = new ReadExpr();
		e->set_msg(t);
		e->set_read_type(READ_FLOAT);
		return exprTail(e);
	}
	if (t.type() == READ_STRING) {
		ReadExpr* e = new ReadExpr();
		e->set_msg(t);
		e->set_read_type(READ_STRING);
		return exprTail(e);
	}
	else {
		sim_expr->set_token(t);
		return exprTail(sim_expr);
	}
	
}

Expr* Parser::exprTail(Expr* expr_node) {
	if (currentToken.type() == ADD) {
		ComplexExpr* com_expr = new ComplexExpr();
		com_expr->set_first_operand(expr_node);
		com_expr->set_math_rel(ADD);
		Advance();
		com_expr->set_rest(expr());
		return com_expr;
	}
	else if (currentToken.type() == SUBTRACT) {
		ComplexExpr* com_expr = new ComplexExpr();
		com_expr->set_first_operand(expr_node);
		com_expr->set_math_rel(SUBTRACT);
		Advance();
		com_expr->set_rest(expr());
		return com_expr;
	}
	else if (currentToken.type() == MULTIPLY) {
		ComplexExpr* com_expr = new ComplexExpr();
		com_expr->set_first_operand(expr_node);
		com_expr->set_math_rel(MULTIPLY);
		Advance();
		com_expr->set_rest(expr());
		return com_expr;
	}
	else if (currentToken.type() == DIVIDE) {
		ComplexExpr* com_expr = new ComplexExpr();
		com_expr->set_first_operand(expr_node);
		com_expr->set_math_rel(DIVIDE);
		Advance();
		com_expr->set_rest(expr());
		return com_expr;
	}
	return expr_node;
}

BoolExpr* Parser::bexpr() {
	if (currentToken.type() == NOT) {
		Advance();
		NotBoolExpr* not_node = new NotBoolExpr();
		not_node->set_bool_expr(bexpr());
		return not_node;
	}
	return bexprTail(expr());
}

BoolExpr* Parser::bexprTail(Expr* b_expr) {
	if (currentToken.type() == LESS_THAN) {
		ComplexBoolExpr* comBExpr = new ComplexBoolExpr();
		comBExpr->set_first_operand(b_expr);
		comBExpr->set_rel_type(LESS_THAN);
		Advance();
		comBExpr->set_second_operand(expr());
		return bConnect(comBExpr);
	}
	else if (currentToken.type() == LESS_THAN_EQ) {
		ComplexBoolExpr* comBExpr = new ComplexBoolExpr();
		comBExpr->set_first_operand(b_expr);
		comBExpr->set_rel_type(LESS_THAN_EQ);
		Advance();
		comBExpr->set_second_operand(expr());
		return bConnect(comBExpr);
	}
	else if (currentToken.type() == EQUAL) {
		ComplexBoolExpr* comBExpr = new ComplexBoolExpr();
		comBExpr->set_first_operand(b_expr);
		comBExpr->set_rel_type(EQUAL);
		Advance();
		comBExpr->set_second_operand(expr());
		return bConnect(comBExpr);
	}
	else if (currentToken.type() == NOT_EQUAL) {
		ComplexBoolExpr* comBExpr = new ComplexBoolExpr();
		comBExpr->set_first_operand(b_expr);
		comBExpr->set_rel_type(NOT_EQUAL);
		Advance();
		comBExpr->set_second_operand(expr());
		return bConnect(comBExpr);
	}
	else if (currentToken.type() == GREATER_THAN) {
		ComplexBoolExpr* comBExpr = new ComplexBoolExpr();
		comBExpr->set_first_operand(b_expr);
		comBExpr->set_rel_type(GREATER_THAN);
		Advance();
		comBExpr->set_second_operand(expr());
		return bConnect(comBExpr);
	}
	else if (currentToken.type() == GREATER_THAN_EQ) {
		ComplexBoolExpr* comBExpr = new ComplexBoolExpr();
		comBExpr->set_first_operand(b_expr);
		comBExpr->set_rel_type(GREATER_THAN_EQ);
		Advance();
		comBExpr->set_second_operand(expr());
		return bConnect(comBExpr);
	}
	SimpleBoolExpr* s_be = new SimpleBoolExpr();
	s_be->set_expr_term(b_expr);
	return s_be;
}

BoolExpr* Parser::bConnect(ComplexBoolExpr* b_expr) {
	if (currentToken.type() == AND) {
		b_expr->set_bool_connector_type(AND);
		Advance();
		b_expr->set_rest(bexpr());
		return b_expr;
	}
	else if (currentToken.type() == OR) {	
		b_expr->set_bool_connector_type(OR);
		Advance();
		b_expr->set_rest(bexpr());
		return b_expr;
	}
	b_expr->set_rest(NULL);
	return b_expr;
}

Token Parser::term() {
	Token t = currentToken;
	if (currentToken.type() == ID) {
		Advance();
	}
	else if (currentToken.type() == STRING_TYPE) {
		Advance();
	}
	else if (currentToken.type() == INT_TYPE) {
		Advance();
	}
	else if (currentToken.type() == FLOAT_TYPE) {
		Advance();
	}
	else if (currentToken.type() == BOOL_TRUE) {
		Advance();
	}
	else if (currentToken.type() == BOOL_FALSE) {
		Advance();
	}
	else if (currentToken.type() == READ_INT) {
		Advance();
		Eat(L_PAREN, "Expexted '('");
		t.setLexeme(currentToken.lexeme());
		Eat(STRING_TYPE, "Expexted string");
		Eat(R_PAREN, "Expexted ')'");
	}
	else if (currentToken.type() == READ_FLOAT) {
		Advance();
		Eat(L_PAREN, "Expexted '('");
		t.setLexeme(currentToken.lexeme());
		Eat(STRING_TYPE, "Expexted string");
		Eat(R_PAREN, "Expexted ')'");
	}
	else if (currentToken.type() == READ_STRING) {
		Advance();
		Eat(L_PAREN, "Expexted '('");
		t.setLexeme(currentToken.lexeme());
		Eat(STRING_TYPE, "Expexted string");
		Eat(R_PAREN, "Expexted ')'");
	
	}
	return t;
}

void Parser::Advance() {
	currentToken = lex.next_token();
}

void Parser::Eat(int tokenType, const std::string& errorMssg)
{
	if (currentToken.type() == tokenType)
		Advance();
	else {
		Error(errorMssg);
	}
}

void Parser::Error(const std::string& errorMssg) {
	std::cout << errorMssg << std::endl;
	std::cout << "Error line: " << currentToken.line() << " column: " << currentToken.column() << std::endl;
	throw ParseException("Parse Exception");
}
