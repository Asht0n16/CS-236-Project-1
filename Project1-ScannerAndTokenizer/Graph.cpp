#include "Graph.h"

vector<NodeID> Graph::getPostOrderedNodes()
{
	vector<NodeID> list;
	// cycle 1 through numNodes
	for (int i = 1; i <= numNodes; i++)
	{
		// cycle through each node
		for (int n = 0; n < numNodes; n++)
		{
			// if the node PON = correct order placement, add to vector
			if (container[n].getPostOrdNum() == i)
			{
				list.push_back(n);
			}
		}
	}
	return list;
}

Graph Graph::reverse()
{
	Graph rev(numNodes);
	// Cycle through each node
	for (int n = 0; n < numNodes; n++)
	{
		set<NodeID> adjSet = container[n].getList();
		set<NodeID>::iterator iter;
		for (iter = adjSet.begin(); iter != adjSet.end(); iter++)
		{
			// Add backwards edge to reverse graph
			rev.addDependency(*iter, n);
		}
	}
	return rev;
}

void Graph::depthFirstSearch(NodeID n)
{
	container[n].mark();
	set<NodeID> adjList = container[n].getList();
	set<NodeID>::iterator iter;
	for (iter = adjList.begin(); iter != adjList.end(); iter++)
	{
		if (!container[*iter].isVisited())
		{
			depthFirstSearch(*iter);
		}
	}
	container[n].assignPON(postOTracker);
	postOTracker++;
	return;
}

vector<set<NodeID>> Graph::DFSForest(vector<int> order)
{
	// Go through given vector backwards to visit all nodes starting with the one with the highest PON
	vector<set<NodeID>> SCcomponents;
	int oldPostOTracker;
	for (size_t i = order.size(); i > 0; i--)
	{
		NodeID n = order.at(i - 1);
		if (!container[n].isVisited())
		{
			oldPostOTracker = postOTracker;
			depthFirstSearch(n);
			// When it returns, add newly visited nodes to a component and add that to the vector
			set<NodeID> component;
			for (int j = 0; j < numNodes; j++)
			{
				if (container[j].isVisited() && container[j].getPostOrdNum() >= oldPostOTracker && container[j].getPostOrdNum() < postOTracker)
				{
					component.insert(j);
				}
			}
			SCcomponents.push_back(component);
		}
	}
	return SCcomponents;
}

void Graph::DFSForest()
{
	for (int n = 0; n < numNodes; n++)
	{
		if (!container[n].isVisited())
		{
			depthFirstSearch(n);
		}
	}
}