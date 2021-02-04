#pragma once
#include <string>
#include <fstream>
#include "Token.h"
#include "InputStream.h"
#include <vector>
#include <sstream>
using namespace std;
class Tokenizer
{
public:
	vector <string> output;
	Tokenizer() {}
	Tokenizer(string input){
		tokens = new vector<Token*>;
		istream = new InputStream(input);
		parser(input);
		PrintOutput();
	}
	~Tokenizer() {
		delete istream; 
		for (unsigned int i = 0; i < tokens->size(); i++) {
			delete tokens->at(i);
		}
		delete tokens;
	}
	vector <Token*>* tokens = {};
	Token currToken;
	InputStream* istream = NULL;
	string charGroup;
	void parser(string input);
	char currentChar = ' ';
	int currentCharNum = 0;
	int curLineNum = 0;
	Type IDCheck();
	Type indentifier = Type::START;
	Type TokenToEnum(char currentChar);
	bool keyWord();
	bool grammerPunctuation(char currentChar);
	bool punctiuation(char currentChar);
	bool isSpace(char currentChar);
	bool isColon(char currentChar);
	string parenPunct(char currentChar);
	char GetNextChar();
	char advanceCurrentChar();
	Type Comment();
	Type BlockComment();
	Type String();
	Type checkForDash();
	Type checkOrCommand();
	Type checkStringEnd();
	
	void PrintOutput();
	void addToTokenList(string type, string value, int lineNum = 0);
};

