#pragma once
#include <vector>
#include <string>
#include <sstream>
using namespace std;
	enum class Type {
		START,
		PUNCUTATION,
		COLON,
		ID,
		ERROR,
		STRING,
		END_OF_STRING,
		START_COMMENT,
		COMMENT,
		BLOCK_COMMENT,
		UNDEFINED,
		OR_COMMAND,
	};

	class Token 
	{
	public:
		string type;
		string value;
		int line = 0;
		string toString() {
			stringstream ss;
			ss << '(' << type << ',' << value  << "," << to_string(line) << ")";
			return ss.str();
		}
		Token() {
		}
		Token(string typeOf, string values, int lineNum = 0) {
			type = typeOf;
			value = values;
			line = lineNum;
		}
		

};

