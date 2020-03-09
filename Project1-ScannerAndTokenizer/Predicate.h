#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include <sstream>

#include "Parameter.h"

using std::string;
using std::vector;
using std::ostringstream;
using std::ostream;

class Predicate
{
private:
	string name;
	vector<Parameter> parameters;

public:
	Predicate(string n) : name(n) {}

	string getName() { return name; }

	vector<Parameter> getParams() { return parameters; }

	Parameter getLastParam() { return parameters.back(); }

	// Adds a parameter to the predicate
	void addParam(int t, string p)
	{
		parameters.push_back(Parameter(t, p));
	}

	string toString()
	{
		ostringstream oSS;
		oSS << name << "(";
		for (size_t i = 0; i < parameters.size(); i++)
		{
			oSS << parameters.at(i) << ",";
		}
		string output = oSS.str();
		output.pop_back();
		return output + ")";
	}

	friend ostream& operator<<(ostream& os, Predicate& p) 
	{
		os << p.toString();
		return os;
	}
};

#endif //PREDICATE_H