#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <deque>

#include "Token.h"

using std::ifstream;
using std::string;
using std::ostringstream;
using std::ostream;
using std::deque;
using std::endl;

class Scanner
{
private:
	ifstream in;
	deque<Token> tokens;
	int lineNum;

public:
	Scanner(string inFile) : in(inFile), lineNum(1)
	{
		scanInput();
	}

	void scanInput();

	void addToDeque(Token t)
	{
		tokens.push_back(t);
	}

	deque<Token>& getTokens() 
	{
		return tokens;
	}

	void scanWords(char c);

	void scanStrings(char c);

	void scanBComment(char c);

	void scanLComment(char c);

	string toString()
	{
		ostringstream oSS;
		for (size_t i = 0; i < tokens.size(); i++)
		{
			oSS << tokens.at(i) << endl;
		}
		oSS << "Total Tokens = " << tokens.size() << endl;
		return oSS.str();
	}

	friend ostream& operator<<(ostream& os, Scanner& s)
	{
		os << s.toString();
		return os;
	}
};

#endif //SCANNER_H