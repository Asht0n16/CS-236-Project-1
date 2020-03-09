#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <deque>

#include "Scanner.h"
#include "Token.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"

using std::vector;
using std::string;
using std::ostringstream;
using std::ostream;
using std::set;
using std::deque;

class Parser
{
private:
	deque<Token> tokenList;
	vector<Predicate> listOfSchemes;
	vector<Predicate> listOfFacts;
	vector<Rule> listOfRules;
	vector<Predicate> listOfQueries;
	set<string> listOfDomains;
	bool correctSyntax;
	Token* badToken;

public:
	Parser(deque<Token>& list) : tokenList(list), correctSyntax(true)
	{
		startParsing();
	}

	// Getter functions
	vector<Predicate> getSchemes() const { return listOfSchemes; }

	vector<Predicate> getFacts() const { return listOfFacts; }

	vector<Rule> getRules() const { return listOfRules; }

	vector<Predicate> getQueries() const { return listOfQueries; }

	set<string> getDomain() const { return listOfDomains; }

	// Parsing functions:
	void match(int type);

	void startParsing();//	->	SCHEMES COLON scheme schemeList
						//		FACTS COLON factList
						//	    RULES COLON ruleList
						//		QUERIES COLON query queryList

	void scheme();		//	-> 	ID LEFT_PAREN ID idList RIGHT_PAREN

	void schemeList();	//	->	scheme schemeList | lambda

	void factList();	//	->	fact factList | lambda

	void fact();		//	->	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD

	void ruleList();	//	->	rule ruleList | lambda

	void rule();		//	->	headPredicate COLON_DASH predicate predicateList PERIOD

	void queryList();	//	->	query queryList | lambda

	void query();		//	->  predicate Q_MARK

	void headPredicate();//	->	ID LEFT_PAREN ID idList RIGHT_PAREN

	void predicate(int t);//->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN

	void predicateList();//	->	COMMA predicate predicateList | lambda

	void parameterList(int t);//->COMMA parameter parameterList | lambda

	void stringList();	//	-> 	COMMA STRING stringList | lambda

	void idList(int t);	//	-> 	COMMA ID idList | lambda

	void parameter(int t);//->	STRING | ID | expression

	string expression(int t);//->LEFT_PAREN exParameter operator exParameter RIGHT_PAREN

	Parameter exParameter(int t);//->STRING | ID | expression

	char operator_();	//	->	ADD | MULTIPLY

	string toString();

	friend ostream& operator<<(ostream& os, Parser& p)
	{
		os << p.toString();
		return os;
	}

};

#endif //PARSER_H
