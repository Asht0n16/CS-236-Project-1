#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Token.h"
#include "Scanner.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Make sure input is provided
	if (argc < 2 || *argv[1] == -1)
	{
		cerr << "No input file found." << endl;
		return 1;
	}

	// Insert input to scanner
	Scanner scan(argv[1]);

	//// -- Scan output tester -- //
	//scan.vectorTest(1, "test1", 1);
	//scan.vectorTest(STRING, "test2", 2);
	//scan.vectorTest(RULES, "test3", 3);

	// Output scanner results
	cout << scan;

	//// -- Token Tester -- //
	//vector<Token> tokens;
	//tokens.push_back(Token(1, "test1", 1));
	//tokens.push_back(Token(STRING, "test2", 2));
	//tokens.push_back(Token(RULES, "test3", 3));

	//for (size_t i = 0; i < tokens.size(); i++)
	//{
	//	cout << tokens.at(i) << endl;
	//}
	//cout << "Total Tokens = " << tokens.size();
	//// -- End Token Tester -- //

	return 0;
}