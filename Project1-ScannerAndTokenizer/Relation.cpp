#include "Relation.h"

using namespace std;

Tuple Relation::getSingleTuple(size_t index)
{
	set<Tuple>::iterator iter = tuples.begin();
	for (size_t i = 0; i < index; i++) iter++;
	return *iter;
}

Relation Relation::select(size_t index, string value)
{
	Relation answer = Relation(this->getName(), this->getScheme());
	set<Tuple>::iterator iter = tuples.begin();
	for (; iter != tuples.end(); iter++)
	{
		if (iter->values.at(index) == value)
		{
			answer.addTuple(*iter);
		}
	}
	return answer;
}

Relation Relation::select(size_t index1, size_t index2)
{
	Relation answer = Relation(this->getName(), this->getScheme());
	set<Tuple>::iterator iter = tuples.begin();
	for (; iter != tuples.end(); iter++)
	{
		if (iter->values.at(index1) == iter->values.at(index2))
		{
			answer.addTuple(*iter);
		}
	}
	return answer;
}

Relation Relation::project(vector<size_t> indices)
{
	vector<string> oldAttributes = this->getScheme().getAttributes();
	vector<string> newAttributes;
	for (size_t i = 0; i < indices.size(); i++) 
	{
		newAttributes.push_back(oldAttributes.at(indices.at(i)));
	}
	Scheme newScheme = Scheme(newAttributes);
	Relation answer = Relation(this->getName(), newScheme);

	for (size_t tupleInd = 0; tupleInd < getSize(); tupleInd++)
	{
		vector<string> keptValues;
		vector<string> oldValues = getSingleTuple(tupleInd).values;
		for (size_t i = 0; i < indices.size(); i++)
		{
			keptValues.push_back(oldValues.at(indices.at(i)));
		}
		answer.addTuple(keptValues);
	}
	
	return answer;
}

Relation Relation::rename(vector<string> attributes, string newName)
{
	Scheme newScheme = Scheme(attributes);
	if (newName == "") newName = this->getName();
	Relation answer = Relation(newName, newScheme);

	for (size_t i = 0; i < this->getSize(); i++)
	{
		answer.addTuple(this->getSingleTuple(i));
	}

	return answer;
}

Relation Relation::join(Relation r2)
{
	Relation r1 = *this;
	Scheme s1 = r1.getScheme();
	Scheme s2 = r2.getScheme();

	// Make new scheme by combining the original two
	Scheme newScheme = joinSchemes(s1, s2);

	// Make new relation with new scheme
	Relation answer = Relation(r1.getName() + " + " + r2.getName(), newScheme);

	// Combine tuples
	// For each Tuple t1 in r1...
	for (size_t r1Index = 0; r1Index < r1.getSize(); r1Index++)
	{
		// For each Tuple t2 in r2...
		for (size_t r2Index = 0; r2Index < r2.getSize(); r2Index++)
		{
			Tuple t1 = r1.getSingleTuple(r1Index);
			Tuple t2 = r2.getSingleTuple(r2Index);

			// If the tuples can be joined...
			if (joinable(t1, s1, t2, s2))
			{
				Tuple newTuple = joinTuples(t1, s1, t2, s2, newScheme);
				answer.addTuple(newTuple);
			}
		}
	}

	return answer;
}

Scheme Relation::joinSchemes(Scheme s1, Scheme s2)
{
	vector<string> scheme1 = s1.getAttributes();
	vector<string> scheme2 = s2.getAttributes();
	// Add all attributes in s2 that are not in s1 to s1
	for (size_t i = 0; i < scheme2.size(); i++)
	{
		bool found = false;
		for (size_t j = 0; j < scheme1.size(); j++)
		{
			if (scheme2.at(i) == scheme1.at(j)) found = true;
		}

		if (!found)
		{
			scheme1.push_back(scheme2.at(i));
		}
	}
	
	return Scheme(scheme1);
}

bool Relation::joinable(Tuple t1, Scheme s1, Tuple t2, Scheme s2)
{
	// For each value in scheme and tuple 1...
	for (size_t v1 = 0; v1 < s1.size(); v1++)
	{
		// For each value in scheme and tuple 2...
		for (size_t v2 = 0; v2 < s2.size(); v2++)
		{
			// If the values in the schemes are the same...
			if (s1.at(v1) == s2.at(v2))
			{
				// If the values in the tuples at these indices are different, they are not joinable
				if (t1.values.at(v1) != t2.values.at(v2))
				{
					return false;
				}
			}
		}
	}

	// If we cycled through and all values that need to match do, they are joinable
	return true;
}

Tuple Relation::joinTuples(Tuple t1, Scheme s1, Tuple t2, Scheme s2, Scheme newScheme)
{
	// We know that the two tuples are joinable, so we'll add any values of attributes in s2 that are not in s1 to t1

	// In other words, if v2 from s2 is not found in s1, v2 from t2 will be added to t1
	for (size_t v2 = 0; v2 < s2.size(); v2++)
	{
		bool found = false;
		for (size_t v1 = 0; v1 < s1.size(); v1++)
		{
			if (s1.at(v1) == s2.at(v2)) found = true;
		}

		if (!found)
		{
			t1.values.push_back(t2.values.at(v2));
		}
	}

	return t1;
}