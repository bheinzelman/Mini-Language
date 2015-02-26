HW6: ast.o Parser.o Lexer.o hw6.o SymbolTable.o Token.o
	g++ -o HW6 Parser.o Lexer.o hw6.o SymbolTable.o Token.o ast.o
Parser.o: Parser.cpp
	g++ -c Parser.cpp
Lexer.o: Lexer.cpp
	g++ -c Lexer.cpp
ast.o: ast.cpp
	g++ -c ast.cpp
SymbolTable.o: SymbolTable.cpp
	g++ -c SymbolTable.cpp
Token.o: Token.cpp
	g++ -c Token.cpp
hw6.o: hw6.cpp
	g++ -c hw6.cpp
