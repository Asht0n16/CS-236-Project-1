#include <iostream>
#include <vector>

#include "Token.h"
#include "Scanner.h"
#include "Parser.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Make sure input is provided
	if (argc < 2 || *argv[1] == -1)
	{
		cerr << "No input file found." << endl;
		return 1;
	}

	// Insert input to scanner and scan for tokens
	Scanner scan(argv[1]);

	// For testing
	// cout << scan;

	// Insert scanned tokens into Parser and check the syntax
	Parser parse(scan.getTokens());

	cout << parse;

	return 0;
}