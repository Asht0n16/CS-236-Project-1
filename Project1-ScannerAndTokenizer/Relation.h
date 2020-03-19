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

	bool addTuple(Tuple t)
	{
		return tuples.insert(t).second;
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
	Relation rename(vector<string> attributes, string newName = "");

	// Returns a new relation that is the result of a natural join of this relation and the given relation
	Relation join(Relation r2);

	// Joins two schemes
	Scheme joinSchemes(Scheme s1, Scheme s2);

	// Returns true if the two given tuples can be joined
	bool joinable(Tuple t1, Scheme s1, Tuple t2, Scheme s2);

	// Joins two given tuples
	Tuple joinTuples(Tuple t1, Scheme s1, Tuple t2, Scheme s2, Scheme newScheme);

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