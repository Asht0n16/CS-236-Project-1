#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

using std::vector;
using std::string;
using std::ostringstream;
using std::ostream;
using std::size_t;

using std::left;
using std::setw;

class Tuple
{
public:
	Tuple(vector<string> s) : values(s) {}

	vector<string> values;

	friend bool operator<(Tuple left, Tuple right)
	{
		// Debugging
		/*cout << left.values.at(0) << " and " << right.values.at(0) << endl;
		if (left.values.at(0) < right.values.at(0)) cout << "Left is less than Right" << endl;
		else cout << "Right is less than Left" << endl;*/

		for (size_t i = 0; i < left.values.size(); i++)
		{
			if (left.values.at(i) == right.values.at(i)) continue;
			return left.values.at(i) < right.values.at(i);
		}
		return left.values.at(0) < right.values.at(0);
	}

	string toString() const
	{
		ostringstream oss;
		for (size_t i = 0; i < values.size(); i++)
		{
			oss << left << setw(15) << values.at(i);
		}
		return oss.str();
	}

	friend ostream& operator<<(ostream& os, const Tuple& t)
	{
		os << t.toString();
		return os;
	}


};

#endif //TUPLE_H