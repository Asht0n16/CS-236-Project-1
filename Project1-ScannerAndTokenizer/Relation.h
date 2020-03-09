#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <set>
#include <iomanip>

#include "Scheme.h"
#include "Tuple.h"

//using std::vector;
using std::string;
using std::ostringstream;
using std::ostream;
using std::set;
using std::endl;
using std::setw;
using std::left;
//using std::deque;

class Relation
{
private:
	string name;
	Scheme scheme;
	set<Tuple> tuples;

public:
	Relation() {}
	Relation(string n, Scheme s) : name(n), scheme(s) {}

	string getName() { return name; }

	Scheme getScheme() { return scheme; }

	set<Tuple> getTupleSet() { return tuples; }

	size_t getSize() { return tuples.size(); }

	Tuple getSingleTuple(size_t index);

	void addTuple(vector<string> newValues) 
	{ 
		Tuple newTuple(newValues);
		tuples.insert(newTuple); 
	}

	void addTuple(Tuple t)
	{
		tuples.insert(t);
	}

	// Searches the relation and returns a new relation with only the tuples that have the matching
	// value at the given index
	Relation select(size_t index, string value);

	// Searches the relation and returns a new relation with only the tuples that have matching values
	// at the two given indices
	Relation select(size_t index1, size_t index2);

	// Returns a new relation with only the columns that are in the given indices
	Relation project(vector<size_t> indices);

	// Returns a new relation with the new scheme given
	Relation rename(vector<string> attributes);

	//friend Relation& operator+(Relation left, Relation right);

	string toString()
	{
		ostringstream oss;
		oss << left << setw(20) << name + ":" << scheme << endl;
		for (set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++)
		{
			oss << setw(20) << "" << *i << endl;
		}
		return oss.str();
	}

	friend ostream& operator<<(ostream& os, Relation& r)
	{
		os << r.toString();
		return os;
	}

};

#endif //RELATION_H