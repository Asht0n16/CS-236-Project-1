#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

#include "Token.h"

using std::string;
using std::ostream;

class Parameter
{
private:
	int type;
	string value;
	
public:
	Parameter(int t, string s) : type(t), value(s) {}

	string getValue() { return value; }

	int getType() { return type; }

	string toString() 
	{
		if (type == ID || type == STRING)
		{
			return value;
		}
		else if (type == RULES || type == QUERIES)
		{
			return value;
		}
		return "Default";
	}

	friend ostream& operator<<(ostream& os, Parameter& p)
	{
		os << p.toString();
		return os;
	}

};

#endif //PARAMETER_H