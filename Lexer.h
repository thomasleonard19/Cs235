#pragma once
#include "Token.h"
#include "Tokenizer.h"
#include <fstream>

class Lexer
{
public:
	Lexer() {}
	Lexer(string input) {
		new Tokenizer(input);
	}
	~Lexer() {

	}
};

