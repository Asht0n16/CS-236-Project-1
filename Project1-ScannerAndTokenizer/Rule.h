#ifndef RULE_H
#define RULE_H

#include <vector>
#include <string>
#include <sstream>

#include "Predicate.h"

using std::ostream;
using std::string;
using std::ostringstream;

class Rule
{
private:
	Predicate headPred;
	vector<Predicate> predLists;

public:
	Rule(Predicate p) : headPred(p) {}

	Predicate& getHead() { return headPred; }

	vector<Predicate> getPreds() { return predLists; }

	Parameter getLastParam() { return predLists.back().getLastParam(); }

	// Adds a predicate to the rule
	void addPred(Predicate p)
	{
		predLists.push_back(p);
	}

	// Adds a parameter to the last predicate
	void addParam(int t, string p)
	{
		predLists.back().addParam(t, p);
	}

	string toString()
	{
		ostringstream oss;
		oss << headPred << " :- ";
		for (size_t i = 0; i < predLists.size(); i++)
		{
			oss << predLists.at(i) << ",";
		}
		string output = oss.str();
		output.pop_back();
		return output + ".";
	}

	friend ostream& operator<<(ostream& os, Rule& r)
	{
		os << r.toString();
		return os;
	}
};

#endif //RULE_H