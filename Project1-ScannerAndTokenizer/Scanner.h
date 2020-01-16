#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Token.h"

using std::ifstream;
using std::string;
using std::ostringstream;
using std::ostream;
using std::endl;

class Scanner
{
private:
	ifstream in;
	vector<Token> tokens;
	int lineNum;

public:
	Scanner(string inFile) : in(inFile), lineNum(1)
	{
		scanInput();
	}

	void scanInput();

	void addToVector(Token t)
	{
		tokens.push_back(t);
	}
	
	/*void vectorTest(int t, string v, int l)
	{
		tokens.push_back(Token(t, v, l));
	}*/

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