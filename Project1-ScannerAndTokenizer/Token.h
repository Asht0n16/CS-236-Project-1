#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::ostream;
using std::ostringstream;
using std::vector;

//Token types
enum {
	COMMA,
	PERIOD,
	Q_MARK,
	LEFT_PAREN,
	RIGHT_PAREN,
	COLON,
	COLON_DASH,
	MULTIPLY,
	ADD,
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	ID,
	STRING,
	COMMENT,
	UNDEFINED,
	END_OF_FILE
};

class Token
{
private:
	vector<string> outputTypes = {
		"COMMA",
		"PERIOD",
		"Q_MARK",
		"LEFT_PAREN",
		"RIGHT_PAREN",
		"COLON",
		"COLON_DASH",
		"MULTIPLY",
		"ADD",
		"SCHEMES",
		"FACTS",
		"RULES",
		"QUERIES",
		"ID",
		"STRING",
		"COMMENT",
		"UNDEFINED",
		"EOF"
	};
	int type;
	string value;
	int line;
	

public:
	Token(int t, string v, int l) : type(t), value(v), line(l) {}
	Token(int t, char v, int l) : type(t), value(1, v), line(l) {}

	int& getType() { return type; }

	string& getValue() { return value; }

	int& getLine() { return line; }

	string toString() const
	{
		ostringstream oSS;
		oSS << "(" << outputTypes.at(type) << ",\"" << value << "\"," << line << ")";
		return oSS.str();
	}

	friend ostream& operator<<(ostream& os, Token& t)
	{
		os << t.toString();
		return os;
	}
};

#endif //TOKEN_H