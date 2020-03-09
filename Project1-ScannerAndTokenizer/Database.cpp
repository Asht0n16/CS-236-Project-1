#include "Database.h"

// For testing
//#define DEBUG

using namespace std;

void Database::createDatabase()
{
	// Create new Relations for each Scheme
	for (size_t i = 0; i < listOfSchemes.size(); i++)
	{
		addRelation(listOfSchemes.at(i));
	}

	// Add each Fact as a Tuple in a Relation
	for (size_t i = 0; i < listOfFacts.size(); i++)
	{
		findRelationAndAddTuple(listOfFacts.at(i));
	}

	// Loop through Rules until no more Tuples are added (Lab 4)

}

void Database::addRelation(Predicate& scheme)
{
	string name = scheme.getName();

	// Get parameters from scheme and pull out the attribute strings
	vector<Parameter> parameters = scheme.getParams();
	vector<string> attributes;
	for (size_t i = 0; i < parameters.size(); i++)
	{
		attributes.push_back(parameters.at(i).getValue());
	}

	Scheme newScheme = Scheme(attributes);
	listOfRelations.insert(std::pair<string, Relation>(name, Relation(name, newScheme)));
}

void Database::findRelationAndAddTuple(Predicate& fact)
{
	string name = fact.getName();

	// Get parameters from fact and pull out the value strings
	vector<Parameter> parameters = fact.getParams();
	vector<string> values;
	for (size_t i = 0; i < parameters.size(); i++)
	{
		values.push_back(parameters.at(i).getValue());
	}
	listOfRelations[name].addTuple(values);
}

string Database::evaluateQueries(vector<Predicate> queries)
{
	ostringstream oss;
	for (size_t query = 0; query < queries.size(); query++)
	{
		string name = queries.at(query).getName();
		vector<Parameter> parameters = queries.at(query).getParams();
		Relation targetRelation = listOfRelations[name];
		vector<pair<string, size_t>> variables;
		vector<pair<string, pair<size_t, size_t>>> commonVariableIndices;

#ifdef DEBUG
		cout << "Target Relation:" << endl << targetRelation << endl;
		cout << "Query: " << queries.at(query) << endl;
#endif

		// Cycle through parameters to find constants and common variables
		for (size_t index = 0; index < parameters.size(); index++)
		{
			if (parameters.at(index).getType() == STRING)
			{
				// Select tuples with matching value at given index
				targetRelation = targetRelation.select(index, parameters.at(index).getValue());
			}
			else if (parameters.at(index).getType() == ID)
			{
				bool alreadyFound = false;
				for (size_t i = 0; i < variables.size(); i++)
				{
					if (parameters.at(index).getValue() == variables.at(i).first)
					{
						pair<string, pair<size_t, size_t>> newPair(variables.at(i).first, pair<size_t, size_t>(variables.at(i).second, index));
						commonVariableIndices.push_back(newPair);
						alreadyFound = true;
					}
				}
				if (!alreadyFound) variables.push_back(pair<string, size_t>(parameters.at(index).getValue(), index));
			}
		}
		
		// Cycle through common variable indices and select tuples where two indices need to be the same
		for (size_t variable = 0; variable < commonVariableIndices.size(); variable++)
		{
			size_t firstIndex = commonVariableIndices.at(variable).second.first;
			size_t secondIndex = commonVariableIndices.at(variable).second.second;
			targetRelation = targetRelation.select(firstIndex, secondIndex);
		}

#ifdef DEBUG
		cout << "After Select:" << endl << targetRelation << endl;
#endif

		vector<size_t> variableIndices;
		vector<string> variableNames;
		for (size_t i = 0; i < variables.size(); i++)
		{
			variableIndices.push_back(variables.at(i).second);
			variableNames.push_back(variables.at(i).first);
		}

		// Project relation to get only columns with variable names
		targetRelation = targetRelation.project(variableIndices);

#ifdef DEBUG
		cout << "After Project:" << endl << targetRelation << endl;
#endif

		// Rename relation to match query parameters
		targetRelation = targetRelation.rename(variableNames);

#ifdef DEBUG
		cout << "After Rename:" << endl << targetRelation << endl;
#endif

		// Output results from query
		size_t relationSize = targetRelation.getSize();
		oss << queries.at(query) << "? ";
		if (relationSize == 0) oss << "No" << endl;
		else
		{
			oss << "Yes(" << relationSize << ")" << endl;
			for (size_t tupleInd = 0; tupleInd < relationSize; tupleInd++)
			{
				oss << " ";
				Tuple currTuple = targetRelation.getSingleTuple(tupleInd);
				for (size_t variableInd = 0; variableInd < variables.size(); variableInd++)
				{
					oss << variableNames.at(variableInd) << "=" << currTuple.values.at(variableInd);
					if (variables.size() - variableInd != 1) oss << ",";
					oss << " ";
				}
				oss << endl;
			}
		}
	}
	return oss.str();
}