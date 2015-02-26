/** 
  * Author: Bert Heinzelman
  * Date: 12/23/15
  * File: Token.h
  * Description: Header file for class Token.
  * Token class is used as a container for the attributes of a token for the 'little language'
  */

#ifndef TOKEN_H
#define TOKEN_H
#include <string>

//Token types
const int SEMICOLON       = 1;
const int ASSIGNMENT      = 2;
const int GREATER_THAN    = 3;
const int GREATER_THAN_EQ = 4;
const int EQUAL           = 5;
const int NOT_EQUAL       = 6;
const int LESS_THAN       = 7;
const int LESS_THAN_EQ    = 8;
const int ADD             = 9;
const int SUBTRACT        = 10;
const int MULTIPLY        = 11;
const int DIVIDE          = 12;
const int AND             = 13;
const int OR              = 14;
const int NOT             = 15;
const int READ_INT        = 16;
const int READ_FLOAT      = 17;
const int READ_STRING     = 18;
const int L_PAREN         = 19;
const int R_PAREN         = 20;
const int WHILE           = 21;
const int DO              = 22;
const int IF              = 23;
const int ELSE            = 24;
const int THEN            = 25;
const int END             = 26;
const int PRINT           = 27;
const int PRINT_LN		  = 28;
const int COMMENT         = 29;
const int ID              = 30;
const int INT_TYPE        = 31;
const int STRING_TYPE     = 32;
const int FLOAT_TYPE      = 33;
const int BOOL_FALSE      = 34;
const int BOOL_TRUE       = 35;
const int EOS             = 36;
const int ERROR			  = 37;

const std::string tokens[37] = { "","SEMICOLON", "ASSIGNMENT", "GREATER_THAN", "GREATER_THAN_EQ",
"EQUAL", "NOT_EQUAL", "LESS_THAN", "LESS_THAN_EQ", "ADD", "SUBTRACT", "MULTIPLY", "DIVIDE",
"AND", "OR", "NOT", "READ_INT", "READ_FLOAT", "READ_STRING", "L_PAREN", "R_PAREN", "WHILE",
"DO", "IF", "ELSE", "THEN", "END", "PRINT", "PRINT_LN", "COMMENT", "ID", "INT_TYPE", "STRING_TYPE",
"FLOAT_TYPE", "BOOL_FALSE", "BOOL_TRUE" };

class Token {
	public:
		//default constructor
		Token();
		
		//constructor with line, column, type, and lexeme params
		Token(int type, int line, int col, std::string lexeme);
		
		/*
		 * Getter for the Token's type
		 * Return: The integer value for the type is returned
		 */
		int type() const;
		
		/*
		 * Getter for the line in the file in which the Token occurs
		 * Return: Line number for Token
		 */
		int line() const;

		/*
		 * Getter for the column in the file in which the Token occurs
		 * Return: Column number for Token
		 */
		int column() const;

		/*
		 * Getter for the lexeme of the Token
		 * Return: A string which contains the Token's Lexeme
		 */
		std::string lexeme() const;

		/*
		 * Sets the token's lexeme
		 */
		void setLexeme(std::string theLexeme);
	private:
		//member var for type
		int token_type;
		
		//member var for line
		int token_line;

		//member var for column
		int token_column;

		//member var for the Token's lexeme
		std::string token_lexeme;
};

#endif
