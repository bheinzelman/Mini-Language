/**
* Author: Bert Heinzelman
* Date: 2/7/15
* Updates: 2/15/15
* File: Parser.h
* Description: Header file for class Parser. 
* Used to parse a file after it has been lexically analysed
*/

#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Token.h"
#include "ParseException.h"
#include "ast.h"
#include <istream>

class Parser {
	public:
		//constructor, takes ref to Lexer as argument
		Parser(Lexer& l);

		/* Function to parse stream of tokens
		 * Return: A StmtList object if successfull
		 * and NULL if otherwise
		 */
		StmtList* Parse();

	private:
		//data member of type lexer
		Lexer& lex;
	
		//data member for the current token that is being Parsed
		Token currentToken;

		/*  Advances current token to next token in 
		 *  the token stream;
		 *  Post: currentToken is advanced
		 */
		void Advance();

		/*  Determines if current token has correct token type
		 *  Param: an int for tokenType, and a string for an err message
		 *  Post: if currentToken is of wrong type, Error is called
		 */
		void Eat(int token_type, const std::string& errorMessage);

		/* Throws exception and couts error message
		 * Pre: invalid token detected
		 * Throw: ParseException
		 */
		void Error(const std::string& errMessage);

		//determines valid statements and returns a StmtList
		StmtList* stmts(StmtList* stmtList);

		//determines valid statement tails
		void stmtsTail(StmtList* stmtList);

		//adds else-if and else's to 
		//IfStmt
		void condTail(IfStmt* if_node);

		//returns an expression object
		Expr* expr();

		//determines complex part of Expr object
		Expr* exprTail(Expr* expr_node);

		//returns a BoolExpr
		BoolExpr* bexpr();

		//return a Complex BoolExpr
		BoolExpr* bexprTail(Expr* expr);

		//connects bool expr's with AND, OR and then returns
		BoolExpr* bConnect(ComplexBoolExpr* b_expr);

		//returns a token with the correct type
		Token term();
};
#endif
