/** 
  * Author: Bert Heinzelman
  * Date: 12/23/15
  * File: Lexer.cpp
  * Description: Implementation file for class Lexer.
  */

#include "Lexer.h"
#include <fstream>
#include <iostream>
#include <cctype>


Lexer::Lexer(std::ifstream& stream)  : input_stream(stream) {
	current_line   = 0;
	current_column = 0;
}

Token Lexer::next_token() {
	char c = getNextChar();
	//current_column++;
	Token t;
	//if whitespace; get next char until not whitespace
	while (isspace(c)) {
		c = getNextChar();
	}
	//single chars
	if (c == '#') {
		while (input_stream.peek() != '\n')
			c = input_stream.get();
		//c = getNextChar();
		current_line++;
		current_column = 0;
		return next_token();
	}
	if (c == ';') {
		t = Token(SEMICOLON, current_line, current_column, "");
	}
	else if (c == '+') {
		t = Token(ADD, current_line, current_column, "");
	}
	else if (c == '-') {
		t = Token(SUBTRACT, current_line, current_column, "");
	}
	else if (c == '*') {
		t = Token(MULTIPLY, current_line, current_column, "");
	}
	else if (c == '/') {
		t = Token(DIVIDE, current_line, current_column, "");
	}
	else if (c == '(') {
		t = Token(L_PAREN, current_line, current_column, "");
	}
	else if (c == ')') {
		t = Token(R_PAREN, current_line, current_column, "");
	}

	//comparators
	else if (c == '>') {
		if (input_stream.peek() != '=') 
			t = Token(GREATER_THAN, current_line, current_column, "");
		else {
			t = Token(GREATER_THAN_EQ, current_line, current_column, "");
			input_stream.get();
		}
	}
	else if (c == '=') {
		if (input_stream.peek() != '=') 
			t = Token(ASSIGNMENT, current_line, current_column, "");
		else {
			t = Token(EQUAL, current_line, current_column, "");
			input_stream.get();
		}
	}
	else if (c == '!') {
		if (input_stream.peek() != '=') 
			t = Token(NOT, current_line, current_column, "");
		else {
			t = Token(NOT_EQUAL, current_line, current_column, "");
			input_stream.get();
		}
	}
	else if (c == '<') {
		if (input_stream.peek() != '=') 
			t = Token(LESS_THAN, current_line, current_column, "");
		else {
			t = Token(LESS_THAN_EQ, current_line, current_column, "");
			input_stream.get();
		}
	}
	//identifiers
	else if (isalpha(c)) {
		std::string theStr = "";
		theStr += c;
		int startColumn = current_column;
		bool getString = true;
		//make string until whitespace or any other symbol
		while (getString) {
			if (isalpha(input_stream.peek()) || isdigit(input_stream.peek())) {
				c = input_stream.get();
				theStr += c;
			}
			else 
				getString = false;
		}
		//possible reserved words
		if (theStr == "and") {
			t = Token(AND, current_line, startColumn, theStr);
		}
		else if (theStr == "or") {
			t = Token(OR, current_line, startColumn, theStr);
		}
		else if (theStr == "iread") {
			t = Token(READ_INT, current_line, startColumn, theStr);
		}
		else if (theStr == "sread") {
			t = Token(READ_STRING, current_line, startColumn, theStr);
		}
		else if (theStr == "fread") {
			t = Token(READ_FLOAT, current_line, startColumn, theStr);
		}
		else if (theStr == "while") {
			t = Token(WHILE, current_line, startColumn, theStr);
		}
		else if (theStr == "do") {
			t = Token(DO, current_line, startColumn, theStr);
		}
		else if (theStr == "then") {
			t = Token(THEN, current_line, startColumn, theStr);
		}
		else if (theStr == "end") {
			t = Token(END, current_line, startColumn, theStr);
		}
		else if (theStr == "print") {
			t = Token(PRINT, current_line, startColumn, theStr);
		}
		else if (theStr == "println") {
			t = Token(PRINT_LN, current_line, startColumn, theStr);
		}
		else if (theStr == "true") {
			t = Token(BOOL_TRUE, current_line, startColumn, theStr);
		}
		else if (theStr == "false") {
			t = Token(BOOL_FALSE, current_line, startColumn, theStr);
		}
		else if (theStr == "if") {
			t = Token(IF, current_line, startColumn, theStr);
		}
		else if (theStr == "else") {
			t = Token(ELSE, current_line, startColumn, theStr);
		}
		else if (theStr == "not") {
			t = Token(NOT, current_line, startColumn, theStr);
		}
		else {
			t = Token(ID, current_line, startColumn, theStr);
		}
	}
	//string type
	else if (c == '"') {
		std::string theStr = "";
		int firstLetterColumn = current_column;
		c = getNextChar();
		//make string until another double quotation
		while (c != '"') {
			theStr += c;
			c = input_stream.get();
			current_column++;
			if (c == '\n') {
				current_line++;
				return Token(ID, current_line, firstLetterColumn, "String Error");
			}	
		}
		t = Token(STRING_TYPE, current_line, firstLetterColumn, theStr);
	}
	//number type
	else if (isdigit(c)) {
		std::string theNumber = "";
		theNumber += c;
		bool hasDecimal = false;
		int firstLetterColumn = current_column;
		while (isdigit(input_stream.peek()) || input_stream.peek() == '.') {
			c = getNextChar();
			//block ensures only one decimal point or 
			//Token type returned is ERROR
			if (c == '.') {
				if (!hasDecimal) {
					theNumber += c;
					hasDecimal = true;
				}
				else {
					//returns error if more than one decimal point
					return Token(ERROR, current_line, firstLetterColumn, "Invalid Decimal Number");
				}
			}
			else {
				theNumber += c;
			}	
		}	
		//return correct type FLOAT_TYPE or INT_TYPE
		t = (hasDecimal) ? Token(FLOAT_TYPE, current_line, firstLetterColumn, theNumber) : Token(INT_TYPE, current_line, firstLetterColumn, theNumber);
	}
	//end of stream token
	else if (input_stream.eof()) {
		t = Token(EOS, current_line, current_column, "");
	}
	//ERROR Token
	else  {
		std::string lex = "";
		lex += c;
		t = Token(ERROR, current_line, current_column, lex);
	}
	return t;
}

char Lexer::getNextChar() {
	char c = input_stream.get();
	current_column++;
	while (c == '\n') {
		current_line++;
		current_column = 0;
		c = input_stream.get();
	}
	return c;
}	



