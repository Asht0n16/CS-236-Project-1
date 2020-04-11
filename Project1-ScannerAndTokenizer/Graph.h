#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "Node.h"

using std::map;
using std::string;
using std::ostringstream;
using std::endl;
using std::vector;

class Graph
{
private:
	map<NodeID, Node> container;
	int postOTracker;
	int numNodes;

public:
	Graph(int numRules) : postOTracker(1), numNodes(numRules) {}

	// Returns the PONs in the correct order
	vector<NodeID> getPostOrderedNodes();

	// Returns a single Node
	Node getNode(NodeID n) { return container[n]; }

	// Adds a dependency from n1 to n2, i.e. n1 depends on n2
	void addDependency(NodeID n1, NodeID n2)
	{
		container[n1].addAdjNode(n2);
	}

	// Returns a graph that is a reverse of this
	Graph reverse();

	// Performs a DFS, assigning PONs along the way
	void depthFirstSearch(NodeID n);

	// Performs DFS until all nodes are visited and assigned numbers. One takes a list which will dictate what order of nodes to begin DFS on
	// and returns a vector of sets that contain strongly connected components.
	// The other simply performs DFS on nodes in numeric order.
	vector<set<NodeID>> DFSForest(vector<int> order);
	void DFSForest();

	string toString() 
	{
		ostringstream oss;
		for (int n = 0; n < numNodes; n++)
		{
			oss << "R" << n << ": " << container[n] << endl;
		}
		return oss.str();
	}

	friend ostream& operator<<(ostream& os, Graph& g)
	{
		os << g.toString();
		return os;
	}
};

#endif //GRAPH_H