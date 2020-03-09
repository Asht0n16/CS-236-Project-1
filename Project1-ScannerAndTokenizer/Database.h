#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>

#include "Predicate.h"
#include "Rule.h"
#include "Relation.h"
#include "Token.h"

using std::vector;
using std::string;
using std::ostringstream;
using std::ostream;
using std::set;
using std::map;
using std::endl;
using std::cout;

class Database
{
private:
	vector<Predicate> listOfSchemes;
	vector<Predicate> listOfFacts;
	vector<Rule> listOfRules;
	set<string> domainSet;

	map<string, Relation> listOfRelations;

public:
	Database(vector<Predicate> schemes, vector<Predicate> facts, vector<Rule> rules, set<string> domain) : 
		listOfSchemes(schemes), 
		listOfFacts(facts), 
		listOfRules(rules), 
		domainSet(domain)
	{
		createDatabase();
	}

	void createDatabase();

	void addRelation(Predicate& scheme);

	void findRelationAndAddTuple(Predicate& fact);

	string evaluateQueries(vector<Predicate> queries);

	string toString()
	{
		ostringstream oss;
		map<string, Relation>::iterator iter;
		for (iter = listOfRelations.begin(); iter != listOfRelations.end(); iter++)
		{
			oss << iter->second << endl << endl;
		}
		return oss.str();
	}

	friend ostream& operator<<(ostream& os, Database& d)
	{
		os << d.toString();
		return os;
	}

};

#endif //DATABASE_H