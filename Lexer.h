/** 
  * Author: Bert Heinzelman
  * Date: 12/23/15
  * File: Lexer.h
  * Description: Header file for class Lexer. 
  * Class Lexer is used to convert written programs into its token stream.
  */

#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <istream>

class Lexer {
	public:
		//constructor
		Lexer(std::ifstream& input_stream);

		/* Returns the next token
		 * Return: The next token in the stream 
		 */
		Token next_token();
	private:

		/* Helper function to move to next character in the file stream
		 * Pre: Stream is at a position in the file stream
		 * Post: cursor is taken to the next non new line character
		 * Return: The next character
		 */
		char getNextChar();

		//member variable for the current line in the file stream
		int current_line;

		//member variable for the current column in the file stream
		int current_column;
		
		//The ifstream that is to be tokenized
		std::ifstream& input_stream;
};
#endif
