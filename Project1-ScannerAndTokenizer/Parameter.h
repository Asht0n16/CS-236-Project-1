#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

using std::string;
using std::ostream;

class Parameter
{
private:
	int type;
	string value;
	
public:
	Parameter(int t, string s) : type(t), value(s) {}

	string toString() 
	{
		if (type == ID || type == STRING)
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