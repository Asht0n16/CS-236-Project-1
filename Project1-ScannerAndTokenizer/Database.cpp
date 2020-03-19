#include "Database.h"

// For testing
//#define QDEBUG
//#define RDEBUG

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

	#ifdef DEBUG
	// Practice join of two relations
	Relation testJ = listOfRelations["snap"].join(listOfRelations["csg"]);
	cout << "Test join: " << endl << testJ;

	//Practice reordering of columns with project
	vector<string> oldS = listOfRelations["snap"].getScheme().getAttributes();
	vector<string> reordered;
	reordered.push_back("N");
	reordered.push_back("P");
	reordered.push_back("A");
	reordered.push_back("S");
	vector<size_t> newIndices;
	for (size_t i = 0; i < reordered.size(); i++) {
		for (size_t j = 0; j < oldS.size(); j++) {
			if (reordered.at(i) == oldS.at(j)) {
				newIndices.push_back(j);
			}
		}
	}
	Relation testR = listOfRelations["snap"].project(newIndices);
	cout << "Test reorder: " << endl << testR;
	#endif

	// Loop through Rules until no more Tuples are added (Lab 4)
	bool changesMade = false;
	size_t cycleCounter = 0;
	do 
	{
		for (size_t ruleInd = 0; ruleInd < listOfRules.size(); ruleInd++)
		{
			changesMade = evaluateRule(listOfRules.at(0));
		}
	} while (changesMade);
	
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
		vector<pair<string, size_t>> variables;
		Relation rel = evaluatePredicate(queries.at(query), variables);

		// Get the variable names
		vector<string> variableNames;
		for (size_t i = 0; i < variables.size(); i++)
		{
			variableNames.push_back(variables.at(i).first);
		}

		// Generate output string
		size_t relationSize = rel.getSize();
		oss << queries.at(query) << "? ";
		if (relationSize == 0) oss << "No" << endl;
		else
		{
			oss << "Yes(" << relationSize << ")" << endl;
			for (size_t tupleInd = 0; tupleInd < relationSize; tupleInd++)
			{
				oss << " ";
				Tuple currTuple = rel.getSingleTuple(tupleInd);
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

Relation Database::evaluatePredicate(Predicate pred, vector<pair<string, size_t>>& variables)
{
	string name = pred.getName();
	vector<Parameter> parameters = pred.getParams();
	Relation targetRelation = listOfRelations[name];
	vector<pair<string, pair<size_t, size_t>>> commonVariableIndices;

#ifdef DEBUG
	cout << "Target Relation:" << endl << targetRelation << endl;
	cout << "Predicate: " << pred << endl;
#endif

	selectTuplesAndFindVariables(parameters, targetRelation, variables, commonVariableIndices);

#ifdef DEBUG
	cout << "After Select:" << endl << targetRelation << endl;
#endif

	createNewSchemeAndProjectAndRename(variables, targetRelation);

#ifdef DEBUG
	cout << "After Project and Rename:" << endl << targetRelation << endl;
#endif

	return targetRelation;
}

Relation Database::evaluatePredicate(Predicate pred)
{
	string name = pred.getName();
	vector<Parameter> parameters = pred.getParams();
	Relation targetRelation = listOfRelations[name];
	vector<pair<string, size_t>> variables;
	vector<pair<string, pair<size_t, size_t>>> commonVariableIndices;

	selectTuplesAndFindVariables(parameters, targetRelation, variables, commonVariableIndices);

	createNewSchemeAndProjectAndRename(variables, targetRelation);

	return targetRelation;
}

bool Database::evaluateRule(Rule rule)
{
	vector<Predicate> predicates = rule.getPreds();
	vector<Relation> resultingRelations;

	// Make a new relation for each predicate
	for (size_t pred = 0; pred < predicates.size(); pred++)
	{
		resultingRelations.push_back(evaluatePredicate(predicates.at(pred)));
	}

#ifdef RDEBUG
	for (size_t i = 0; i < resultingRelations.size(); i++)
	{
		cout << resultingRelations.at(i);
	}
#endif

	// Join all predicate relations into one
	Relation joined = joinMultipleRelations(resultingRelations);

	// Project needed columns
	Relation projected = projectRuleRelation(joined, rule);

	// Rename to match relation in database
	Relation renamed = renameToMatchDatabase(projected, rule);

#ifdef RDEBUG
	cout << "Joined:" << endl << joined;
	cout << "Projected:" << endl << projected;
	cout << "Renamed:" << endl << renamed;
	cout << "Target:" << endl << listOfRelations[renamed.getName()];
#endif

	// Union the resulting relation with the existing relation in the database
	bool added = unionRuleIntoDatabase(renamed);

	return added;
}

void Database::selectTuplesAndFindVariables(
	vector<Parameter>& parameters, 
	Relation& targetRelation, 
	vector<pair<string, size_t>>& variables, 
	vector<pair<string, pair<size_t, size_t>>>& commonVariableIndices)
{
	// Cycle through parameters to find constants and common variables
	for (size_t index = 0; index < parameters.size(); index++)
	{
		if (parameters.at(index).getType() == STRING)	// If the parameter is a string, it is a constant
		{
			// Select tuples with matching value at given index
			targetRelation = targetRelation.select(index, parameters.at(index).getValue());
		}
		else if (parameters.at(index).getType() == ID)	// If the parameter is an ID, it is a variable
		{
			// Check to see if the variable has already been found
			bool alreadyFound = false;
			for (size_t i = 0; i < variables.size(); i++)	// Cycle through the current vector of variables
			{
				// If the variable has already been found, add a new pair to the commonVariableIndices vector 
				// so we know that the variable is found in multiple places
				if (parameters.at(index).getValue() == variables.at(i).first)
				{
					// This pair has the variable name first and a pair second. The second pair has the two indices the variable is found at
					pair<string, pair<size_t, size_t>> newPair(variables.at(i).first, pair<size_t, size_t>(variables.at(i).second, index));
					commonVariableIndices.push_back(newPair);
					alreadyFound = true;
				}
			}
			// Only add the variable to the variables vector if it is new
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
}

void Database::createNewSchemeAndProjectAndRename(vector<pair<string, size_t>>& variables, Relation& targetRelation)
{
	// Create vectors with the needed names and indices of the new scheme
	vector<size_t> variableIndices;
	vector<string> variableNames;
	for (size_t i = 0; i < variables.size(); i++)
	{
		variableNames.push_back(variables.at(i).first);
		variableIndices.push_back(variables.at(i).second);
	}

	// Project relation to get only columns with variable names
	targetRelation = targetRelation.project(variableIndices);

	// Rename relation to match predicate parameters
	targetRelation = targetRelation.rename(variableNames);
}

Relation Database::joinMultipleRelations(vector<Relation> relationsToBeJoined)
{
	Relation left = relationsToBeJoined.at(0);
	for (size_t rel = 1; rel < relationsToBeJoined.size(); rel++)
	{
		Relation right = relationsToBeJoined.at(rel);
		left = left.join(right);
	}

	return left;
}

Relation Database::projectRuleRelation(Relation rel, Rule& rule)
{
	// Get the scheme for the new relation and the scheme for the old
	vector<Parameter> params = rule.getHead().getParams();
	vector<string> newScheme;
	for (size_t i = 0; i < params.size(); i++)
	{
		newScheme.push_back(params.at(i).getValue());
	}
	vector<string> oldScheme = rel.getScheme().getAttributes();

	// Find where the attributes of the new scheme are in the old scheme
	vector<size_t> indices;
	for (size_t newA = 0; newA < newScheme.size(); newA++)
	{
		for (size_t oldA = 0; oldA < oldScheme.size(); oldA++)
		{
			if (newScheme.at(newA) == oldScheme.at(oldA))
			{
				indices.push_back(oldA);
			}
		}
	}

	// Return the projected result
	return rel.project(indices);
}

Relation Database::renameToMatchDatabase(Relation rel, Rule& rule)
{
	string name = rule.getHead().getName();
	vector<string> newAttributes = listOfRelations[name].getScheme().getAttributes();

	return rel.rename(newAttributes, name);
}

bool Database::unionRuleIntoDatabase(Relation rel)
{
	string name = rel.getName();
	set<Tuple> newTuples = rel.getTupleSet();

	// Cycle through all the new tuples and add them to the target relation
	bool added = false;
	for (set<Tuple>::iterator iter = newTuples.begin(); iter != newTuples.end(); iter++)
	{
		bool inserted = listOfRelations[name].addTuple(*iter);
		if (inserted)
		{
			added = true;
		}
	}

	return added;
}