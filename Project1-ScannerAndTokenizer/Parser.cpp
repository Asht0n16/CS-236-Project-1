#include "Parser.h"

//-----GRAMMAR------\\

//datalogProgram	->	SCHEMES COLON scheme schemeList
//						FACTS COLON factList
//				        RULES COLON ruleList
//						QUERIES COLON query queryList
//
//schemeList		->	scheme schemeList | lambda
//factList			->	fact factList | lambda
//ruleList			->	rule ruleList | lambda
//queryList			->	query queryList | lambda
//
//scheme			-> 	ID LEFT_PAREN ID idList RIGHT_PAREN
//fact				->	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
//rule			 	->	headPredicate COLON_DASH predicate predicateList PERIOD
//query			    ->  predicate Q_MARK
//
//headPredicate		->	ID LEFT_PAREN ID idList RIGHT_PAREN
//predicate			->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
//	
//predicateList		->	COMMA predicate predicateList | lambda
//parameterList		-> 	COMMA parameter parameterList | lambda
//stringList		-> 	COMMA STRING stringList | lambda
//idList  			-> 	COMMA ID idList | lambda
//
//parameter			->	STRING | ID | expression
//expression		-> 	LEFT_PAREN parameter operator parameter RIGHT_PAREN
//operator			->	ADD | MULTIPLY

void Parser::match(int type)
{
	if (tokenList.front().getType() == type)
	{
		tokenList.pop_front();
	}
	else
	{
		throw& tokenList.front();
	}
}

void Parser::startParsing()
{
	try
	{
		match(SCHEMES);
		match(COLON);
		scheme();
		schemeList();

		match(FACTS);
		match(COLON);
		factList();

		match(RULES);
		match(COLON);
		ruleList();

		match(QUERIES);
		match(COLON);
		query();
		queryList();
	}
	catch (Token* e)
	{
		badToken = e;
		correctSyntax = false;
	}
}

void Parser::scheme()
{
	// Get name of scheme and add new scheme to list
	listOfSchemes.push_back(Predicate(tokenList.front().getValue()));
	match(ID);
	match(LEFT_PAREN);

	// Get ID value and add to current scheme as a parameter
	listOfSchemes.back().addParam(ID, tokenList.front().getValue());
	match(ID);
	idList(SCHEMES);
	match(RIGHT_PAREN);
}

void Parser::schemeList()
{
	if (tokenList.front().getType() == ID)
	{
		scheme();
		schemeList();
	}
	// lambda
}

void Parser::fact()
{
	// Get name of fact and add new fact to list
	listOfFacts.push_back(Predicate(tokenList.front().getValue()));
	match(ID);
	match(LEFT_PAREN);

	// Get value of string and add as parameter to current fact and to the set of domains
	listOfFacts.back().addParam(STRING, tokenList.front().getValue());
	listOfDomains.insert(tokenList.front().getValue());
	match(STRING);
	stringList();

	match(RIGHT_PAREN);
	match(PERIOD);
}

void Parser::factList()
{
	if (tokenList.front().getType() == ID)
	{
		fact();
		factList();
	}
	// lambda
}

void Parser::rule()
{
	headPredicate();
	match(COLON_DASH);
	predicate(RULES);
	predicateList();
	match(PERIOD);
}

void Parser::ruleList()
{
	if (tokenList.front().getType() == ID)
	{
		rule();
		ruleList();
	}
	// lambda
}

void Parser::query()
{
	predicate(QUERIES);
	match(Q_MARK);
}

void Parser::queryList()
{
	if (tokenList.front().getType() == ID)
	{
		query();
		queryList();
	}
	// lambda
}

void Parser::headPredicate()
{
	// Push back a new rule whose head predicate's name is the current token value
	listOfRules.push_back(Rule(Predicate(tokenList.front().getValue())));
	match(ID);
	match(LEFT_PAREN);
	
	// Get first parameter for current rule's head predicate
	listOfRules.back().getHead().addParam(ID, tokenList.front().getValue()); 
	match(ID);
	idList(RULES);
	match(RIGHT_PAREN);
}

void Parser::predicate(int t)
{
	// Get name for predicate...
	if (t == RULES)
	{
		// ... and use for a new predicate in the current rule
		listOfRules.back().addPred(Predicate(tokenList.front().getValue()));
	}
	else if (t == QUERIES)
	{
		// ... and use to create a new predicate for the current query
		listOfQueries.push_back(Predicate(tokenList.front().getValue()));
	}
	else
	{
		throw& tokenList.front();
	}
	match(ID);
	match(LEFT_PAREN);
	parameter(t);
	parameterList(t);
	match(RIGHT_PAREN);
}

void Parser::predicateList()
{
	if (tokenList.front().getType() == COMMA)
	{
		match(COMMA);
		predicate(RULES);
		predicateList();
	}
	// lambda
}

void Parser::parameterList(int t)
{
	if (tokenList.front().getType() == COMMA)
	{
		match(COMMA);
		parameter(t);
		parameterList(t);
	}
	// lambda
}

void Parser::stringList()
{
	if (tokenList.front().getType() == COMMA)
	{
		match(COMMA);
		// Get value of string and add as parameter to current fact and to the set of domains
		listOfFacts.back().addParam(STRING, tokenList.front().getValue());
		listOfDomains.insert(tokenList.front().getValue());
		match(STRING);
		stringList();
	}
	// lambda
}

void Parser::idList(int t)
{
	if (tokenList.front().getType() == COMMA)
	{
		match(COMMA);
		if (t == SCHEMES)
		{
			//	Get ID value and add to current scheme as a parameter
			listOfSchemes.back().addParam(ID, tokenList.front().getValue());
		}
		else if (t == RULES)
		{
			// Get next parameter for current rule's head predicate
			listOfRules.back().getHead().addParam(ID, tokenList.front().getValue());
		}
		else
		{
			throw& tokenList.front();
		}
		match(ID);
		idList(t);
	}
	// lambda
}

void Parser::parameter(int t)
{
	if (tokenList.front().getType() == STRING)
	{
		// Get value of string...
		if (t == RULES)
		{
			//... and add to the current predicate of the current rule.
			listOfRules.back().addParam(STRING, tokenList.front().getValue());
		}
		else if (t == QUERIES)
		{
			//... and add to the predicate of the current query
			listOfQueries.back().addParam(STRING, tokenList.front().getValue());
		}
		else
		{
			throw& tokenList.front();
		}
		match(STRING);
	}
	else if (tokenList.front().getType() == ID)
	{
		if (t == RULES)
		{
			// Get value of ID and add to the current predicate of the current rule.
			listOfRules.back().addParam(ID, tokenList.front().getValue());
		}
		else if (t == QUERIES)
		{
			// Get value of ID and add to the predicate of the current query
			listOfQueries.back().addParam(ID, tokenList.front().getValue());
		}
		else
		{
			throw& tokenList.front();
		}
		match(ID);
	}
	else if (tokenList.front().getType() == LEFT_PAREN)
	{
		// We'll get here soon...
		expression(t);
	}
	else
	{
		throw& tokenList.front();
	}
}

void Parser::expression(int t)
{
	match(LEFT_PAREN);
	parameter(t);
	operator_();
	parameter(t);
	match(RIGHT_PAREN);
}

void Parser::operator_()
{
	if (tokenList.front().getType() == ADD)
	{
		match(ADD);
	}
	else if (tokenList.front().getType() == MULTIPLY)
	{
		match(MULTIPLY);
	}
	else
	{
		throw& tokenList.front();
	}
}

string Parser::toString()
{
	ostringstream oss;
	if (correctSyntax) {
		oss << "Success!" << endl
			<< "Schemes(" << listOfSchemes.size() << "):" << endl;
		for (size_t i = 0; i < listOfSchemes.size(); i++)
		{
			oss << "  " << listOfSchemes.at(i) << endl;
		}
		oss << "Facts(" << listOfFacts.size() << "):" << endl;
		for (size_t i = 0; i < listOfFacts.size(); i++)
		{
			oss << "  " << listOfFacts.at(i) << "." << endl;
		}
		oss << "Rules(" << listOfRules.size() << "):" << endl;
		for (size_t i = 0; i < listOfRules.size(); i++)
		{
			oss << "  " << listOfRules.at(i) << endl;
		}
		oss << "Queries(" << listOfQueries.size() << "):" << endl;
		for (size_t i = 0; i < listOfQueries.size(); i++)
		{
			oss << "  " << listOfQueries.at(i) << endl;
		}
		oss << "Domains(" << listOfDomains.size() << "):" << endl;
		for (set<string>::iterator i = listOfDomains.begin(); i != listOfDomains.end(); i++)
		{
			oss << "  " << *i << endl;
		}
	}
	else
	{
		oss << "Failure!" << endl
			<< "  " << *badToken << endl;
	}
	return oss.str();
}