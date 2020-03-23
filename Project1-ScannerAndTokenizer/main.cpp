#include <iostream>
#include <vector>

// For testing
//#define DEBUG

#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Database.h"

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

	// For Lab 1
	//cout << scan;

	// Insert scanned tokens into Parser and check the syntax
	Parser parse(scan.getTokens());

	// For Lab 2
	//cout << parse;

	// Put Schemes, Facts, Rules, and the Domain into database and create relations
	Database database(parse.getSchemes(), parse.getFacts(), parse.getRules(), parse.getDomain());

	#ifdef DEBUG
	cout << database;
	#endif

	// Evaluate Queries (Lab 3)
	#ifndef DEBUG
	cout << "Query Evaluation" << endl;
	cout << database.evaluateQueries(parse.getQueries());
	#endif

	return 0;
}