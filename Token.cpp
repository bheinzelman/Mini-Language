/** 
  * Author: Bert Heinzelman
  * Date: 12/23/15
  * File: Token.cpp
  * Description: Implementation file for class Token
  */

#include "Token.h"

//default constructor
Token::Token() {}

Token::Token(int type, int line, int col, std::string lexeme) : token_lexeme(lexeme)  {
	token_type   = type;
	token_line   = line;
	token_column = col;
}

int Token::type() const {
	return token_type;
}

int Token::line() const {
	return token_line;
}

int Token::column() const {
	return token_column;
}

std::string Token::lexeme() const {
	return token_lexeme;
}

void Token::setLexeme(std::string theLexeme) {
	token_lexeme = theLexeme;
}
