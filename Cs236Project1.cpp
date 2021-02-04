#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.h"
#include "Tokenizer.h"
#include "Token.h"


using namespace std;

int main(int argc, char* argv[]) {
	Lexer* T = new Lexer(argv[1]);
	delete T;
	return 0;
}
