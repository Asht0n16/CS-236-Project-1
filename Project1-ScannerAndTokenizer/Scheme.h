#ifndef SCHEME_H
#define SCHEME_H

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using std::vector;
using std::string;
using std::ostringstream;
using std::ostream;
using std::setw;
using std::left;

class Scheme
{
private:
	vector<string> attributes;

public:
	Scheme() {}
	Scheme(vector<string> a) : attributes(a) {}

	vector<string> getAttributes() { return attributes; }

	size_t size() { return attributes.size(); }

	string at(size_t index) { return attributes.at(index); }

	string toString() 
	{
		ostringstream oss;
		for (size_t i = 0; i < attributes.size(); i++) 
		{
			oss << left << setw(15) << attributes.at(i);
		}
		return oss.str();
	}

	friend ostream& operator<<(ostream& os, Scheme& s)
	{
		os << s.toString();
		return os;
	}

};

#endif //SCHEME_H