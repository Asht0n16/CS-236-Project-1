#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <set>
#include <string>
#include <sstream>

using std::set;
using std::string;
using std::ostringstream;
using std::ostream;

typedef int NodeID;

class Node
{
private:
	set<NodeID> adjList;
	bool visited;
	int postOrderNum;

public:
	Node(): visited(false), postOrderNum(-1) {}

	set<NodeID> getList() { return adjList; }

	int getPostOrdNum() { return postOrderNum; }

	bool isVisited() { return visited; }

	void addAdjNode(NodeID n) { adjList.insert(n); }

	void assignPON(int p) { postOrderNum = p; }

	void mark() { visited = true; }

	string toString() 
	{
		ostringstream oss;
		set<NodeID>::iterator iter;
		for (iter = adjList.begin(); iter != adjList.end(); iter++)
		{
			oss << "R" << *iter << " ";
		}
		return oss.str();
	}

	friend ostream& operator<<(ostream& os, Node& n)
	{
		os << n.toString();
		return os;
	}

};

#endif //NODE_H