#pragma once
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
class InputStream
{
public:
	ifstream* input;
	InputStream(string file) {
		input = new ifstream;
		input->open(file);
		if (!*input) {
			cout << "File Not Found" << endl;
		}
	}
	~InputStream() {
		input->close();
		delete input;
	}
	char currChar= ' ';
	int currLine = 1;
	int getLineNum() {
		return currLine;
	}
	char getNext() {
		if (input->peek() == '\n') {
			currLine++;
		}
		return input->get();
	}
	char peek() {
		return input->peek();
	}

};

