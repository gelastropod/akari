#pragma once

#include <string>
#include <vector>

struct CNFClause {
	std::vector<int> nodes;
	std::vector<bool> flipped;

	std::string exp();
};

struct CNF {
	int numNodes = 0;
	std::vector<CNFClause> clauses;

	int addInput();
	void addAssert(int node, bool val);
	int addIntermediate(std::vector<int> deps, std::vector<bool> flipped, int op);
	std::string exp();
};
