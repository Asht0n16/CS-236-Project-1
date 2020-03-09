#include "Relation.h"

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
	for (iter; iter != tuples.end(); iter++)
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
	for (iter; iter != tuples.end(); iter++)
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

Relation Relation::rename(vector<string> attributes)
{
	Scheme newScheme = Scheme(attributes);
	Relation answer = Relation(this->getName(), newScheme);

	for (size_t i = 0; i < this->getSize(); i++)
	{
		answer.addTuple(this->getSingleTuple(i));
	}

	return answer;
}

//Relation& operator+(Relation& left, Relation& right)
//{
//	if (left.getName() != right.getName()) throw "Different names! Cannot add relations.";
//
//	set<Tuple>::iterator rightBegin = right.getTupleSet().begin();
//	set<Tuple>::iterator rightEnd = right.getTupleSet().end();
//	for (rightBegin; rightBegin != rightEnd; rightBegin++)
//	{
//		left.addTuple(*rightBegin);
//	}
//
//	return left;
//}