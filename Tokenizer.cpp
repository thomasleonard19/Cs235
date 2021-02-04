#include "Tokenizer.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <iostream>

char Tokenizer::GetNextChar() {
	return (istream->peek());

}
char Tokenizer::advanceCurrentChar() {
	return (istream->getNext());
}

Type Tokenizer::Comment() {
	charGroup += currentChar;
	if (istream->peek() == '|') {
		currentChar = advanceCurrentChar();
		charGroup += currentChar;
		return Type::BLOCK_COMMENT;
	}
	else {
		while (istream->peek() != EOF && istream->peek() != '\n') {
			charGroup += advanceCurrentChar();

		}
		if (istream->peek() == '\n') {
			addToTokenList("COMMENT", charGroup);
			return Type::START;
		}
		if (istream->peek() == EOF) {
			// just get back into while loop so it can kick itself out and end;
			return Type::START;
		}
		return Type::UNDEFINED;
	}
}
Type Tokenizer::checkOrCommand() {
	if (currentChar == '#') {
		charGroup += currentChar;
		addToTokenList("COMMENT", charGroup, curLineNum);
		//advance();
		return Type::START;
	}
	else {
		charGroup += currentChar;
		//char i = istream->peek();
		if (currentChar == '|') {
			currentChar = advanceCurrentChar();
			return Type::OR_COMMAND;
		}
		else {
			return Type::BLOCK_COMMENT;
		}
	}
}
Type Tokenizer::BlockComment() {
	while (istream->peek() != EOF && istream->peek() != '|') {
		charGroup += advanceCurrentChar();
	}
	if (istream->peek() == '|') {
		// Add |
		currentChar = advanceCurrentChar();
		charGroup += currentChar;
		// Give next function the currentChar of what's after the bar
		currentChar = advanceCurrentChar();
		return Type::OR_COMMAND;
	}
	return Type::UNDEFINED;
}
Type Tokenizer::String() {
	while (istream->peek() != EOF && istream->peek() != '\'') {
		charGroup += advanceCurrentChar();
	}
	if (istream->peek() == '\'') {
		// Could be end of string
		charGroup += advanceCurrentChar();
		return Type::END_OF_STRING;
	}
	else {
		// returned EOF
		return Type::UNDEFINED;
	}

}
Type Tokenizer::checkStringEnd() {
	if (currentChar == '\'' && istream->peek() != '\'') {
		addToTokenList("STRING", charGroup, curLineNum);
	}
	else {
		currentChar = advanceCurrentChar();
		return Type::STRING;
	}
	return Type::START;
}
Type Tokenizer::IDCheck() {
	// check logic
	while (istream->peek() != '\n' && istream->peek() != EOF && !isspace(istream->peek())) {
		if (isalnum(istream->peek()) || istream->peek() == '_') {
			currentChar = advanceCurrentChar();
			charGroup += currentChar;
		}
		else {
			break;
		}
	}
	if (!keyWord()) {
		addToTokenList("ID", charGroup);
	}
	return Type::START;
}
bool Tokenizer::keyWord() {
	if (charGroup == "Queries") {
		addToTokenList("QUERIES", charGroup);
	}
	else if (charGroup == "Schemes") {
		addToTokenList("SCHEMES", charGroup);
	}
	else if (charGroup == "Facts") {
		addToTokenList("FACTS", charGroup);
	}
	else if (charGroup == "Rules") {
		addToTokenList("RULES", charGroup);
	}
	else {
		return false;
	}
	return true;
}
bool Tokenizer::punctiuation(char currentChar) {
	string type;
	if (currentChar == '(' || currentChar == ')' || currentChar == '*' || currentChar == '+') {
		type = parenPunct(currentChar);
	}
	string output = "";
	output += currentChar;
	if (type != "") {
		addToTokenList(type, output);
	}
	return type != "";
}
bool Tokenizer::grammerPunctuation(char currentChar) {
	string type;
	if (currentChar == '.') {
		type = "PERIOD";
	}
	else if (currentChar == ',') {
		type = "COMMA";
	}
	else if (currentChar == '?') {
		type = "Q_MARK";
	}

	string output = "";
	output += currentChar;

	if (type != "") {
		addToTokenList(type, output);
	}

	return type != "";
}
bool Tokenizer::isSpace(char currentChar) {
	if (currentChar == '\n' || currentChar == ' ' || currentChar == '\r') {
		return true;
	}
	return false;
}
bool Tokenizer::isColon(char currentChar){
	if (currentChar == ':') {
		charGroup += currentChar;
		return true;
	}
	return false;
}
string Tokenizer::parenPunct(char currentChar) {
	string type;
	if (currentChar == '(') {
		type = "LEFT_PAREN";
	}
	else if (currentChar == ')') {
		type = "RIGHT_PAREN";
	}
	else if (currentChar == '*') {
		type = "MULTIPLY";
	}
	else if (currentChar == '+') {
		type = "ADD";
	}
	return type;
}
void Tokenizer::PrintOutput() {
	for (unsigned int i = 0; i < tokens->size(); i++) {
		cout << tokens->at(i)->toString() << endl;
	}
	cout << "Total Tokens = " + to_string(tokens->size()) << endl;
}
void Tokenizer::parser(string input) {
	indentifier = Type::START;
	currentChar = ' ';
	while (currentChar != EOF) {
		switch (indentifier) {
		case Type::START:
			charGroup = "";
			currentChar = advanceCurrentChar();
			curLineNum = istream->getLineNum();
			indentifier = TokenToEnum(currentChar);
			break;

		case Type::PUNCUTATION:
			indentifier = Type::START;
			break;

		case Type::COLON:
			indentifier = checkForDash();
			break;

		case Type::ID:
			charGroup += currentChar;
			indentifier = IDCheck();
			break;

		case Type::STRING:
			charGroup += currentChar;
			indentifier = String();
			break;

		case Type::END_OF_STRING:
			indentifier = checkStringEnd();
			break;

		case Type::START_COMMENT:
			indentifier = Comment();
			break;

		case Type::COMMENT:
			break;

		case Type::BLOCK_COMMENT:
			indentifier = BlockComment();
			break;

		case Type::OR_COMMAND:
			indentifier = checkOrCommand();
			break;

		default:
			addToTokenList("UNDEFINED", charGroup, curLineNum);
			indentifier = Type::START;
			break;
		}
	}
	addToTokenList("EOF", "");
}
Type Tokenizer::checkForDash(){
	if (istream->peek() == '-') {
		addToTokenList("COLON_DASH", charGroup + advanceCurrentChar());
	}
	else {
		addToTokenList("COLON", charGroup);
	}
	return Type::START;
}
Type Tokenizer::TokenToEnum(char currentChar) {
	if (punctiuation(currentChar)) {
		return Type::PUNCUTATION;
	}
	else if (grammerPunctuation(currentChar)) {
		return Type::PUNCUTATION;
	}
	else if (isSpace(currentChar)) {
		return Type::START;
	}
	else if (isColon(currentChar)) {
		return Type::COLON;
	}
	else if (isalpha((unsigned)currentChar)) {
		return Type::ID;
	}
	else if (currentChar == '\'') {
		return Type::STRING;
	}
	else if (currentChar == '#') {
		return Type::START_COMMENT;
	}
	else {
		charGroup += currentChar;
		return Type::UNDEFINED;
	}
}

void Tokenizer::addToTokenList(string type, string value, int lineNum) {
	value = "\"" + value + "\"";
	if (lineNum == 0) {
		lineNum = istream->getLineNum();
	}
	Token* T = new Token(type, value, lineNum);
	tokens->push_back(T);
}