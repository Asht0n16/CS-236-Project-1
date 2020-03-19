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
using std::pair;

class Database
{
private:
	vector<Predicate> listOfSchemes;
	vector<Predicate> listOfFacts;
	vector<Rule> listOfRules;
	set<string> domainSet;
	map<string, Relation> listOfRelations;

	// Creates the database by going through the schemes, facts, and rules and filling relations based on them
	void createDatabase();

	// Creates a new relation in the database
	void addRelation(Predicate& scheme);

	// Finds a relation based on the name of the fact and adds the tuple to it
	void findRelationAndAddTuple(Predicate& fact);

	// Evaluate a single predicate and make a new relation. This version is for queries
	Relation evaluatePredicate(Predicate pred, vector<pair<string, size_t>>& variables);

	// Evaluate a single predicate and make a new relation. This version is for rules.
	Relation evaluatePredicate(Predicate pred);

	// Takes a Rule and evaluates it, adding tuples to relations if needed. Returns true if a tuple is added
	bool evaluateRule(Rule rule);

	// Finds the variables and constancts in a predicate, then selects tuples out of the relation to match them
	void selectTuplesAndFindVariables(vector<Parameter>& parameters, 
		Relation& targetRelation, vector<pair<string, size_t>>& variables, 
		vector<pair<string, pair<size_t, size_t>>>& commonVariableIndices);

	// Generates the new needed scheme. Then projects and renames the relation to match
	void createNewSchemeAndProjectAndRename(vector<pair<string, size_t>>& variables, Relation& targetRelation);

	// Joins all relations in given vector into one using the Relation::join function
	Relation joinMultipleRelations(vector<Relation> relationsToBeJoined);

	// Projects the columns of the given relation based on the given rule
	Relation projectRuleRelation(Relation rel, Rule& rule);

	// Renames the given relation to match a relation already in the database, as given by the rule
	Relation renameToMatchDatabase(Relation rel, Rule& rule);

	// Adds all new tuples from the given relation into the matching relation in the database. Returns false if
	// no new tuples are added
	bool unionRuleIntoDatabase(Relation rel);


public:
	Database(vector<Predicate> schemes, vector<Predicate> facts, vector<Rule> rules, set<string> domain) : 
		listOfSchemes(schemes), 
		listOfFacts(facts), 
		listOfRules(rules), 
		domainSet(domain)
	{
		createDatabase();
	}

	// Take all queries and return a string with all tuples that answer each one
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