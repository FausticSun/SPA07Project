#pragma once
#include "Table.h"
#include <map>
#include <utility>
#include <vector>

struct Node {
	std::vector<int> lines;
	std::vector<Node> neighbors;
	Node(std::vector<int> lines, std::vector<Node> neighbors)
		: lines(std::move(lines)), neighbors(std::move(neighbors)) {}
};

/**
class CFG {
private:
	std::map<int, Node> lineNodeMap;
	std::vector<std::vector<int>> adjList;
	std::vector<int> inDegree;
	Table whileIfTable{ 1 };
	Node createCFG(int, std::vector<int>);
	void setInDegree();

public:
	CFG(int, Table, Table);
	Node getLineNode(int);
};
**/
