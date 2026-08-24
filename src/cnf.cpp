#include "cnf.h"

std::string CNFClause::exp() {
	std::string res = "";
	for (int i = 0; i < nodes.size(); i++) {
		if (!flipped[i]) res += std::to_string(nodes[i]);
		else res += std::to_string(-nodes[i]);
		res += " ";
	}
	return res;
}

int CNF::addInput() {
	return numNodes++;
}

void CNF::addAssert(int node, bool val) {
	CNFClause clause;
	clause.nodes.push_back(node);
	clause.flipped.push_back(!val);
	clauses.push_back(clause);
}

int CNF::addIntermediate(std::vector<int> deps, std::vector<bool> flipped, int op) {
	int newNode = numNodes++;
	if (op == 0) {
		CNFClause clause1;
		clause1.nodes = deps;
		clause1.flipped = flipped;
		clause1.nodes.push_back(newNode);
		clause1.flipped.push_back(1);
		for (int i = 0; i < deps.size(); i++) {
			CNFClause clause2;
			clause2.nodes = {deps[i], newNode};
			clause2.flipped = {!flipped[i], 0};
			clauses.push_back(clause2);
		}
		clauses.push_back(clause1);
	}
	else {
		CNFClause clause1;
		clause1.nodes = deps;
		clause1.flipped = flipped;
		clause1.nodes.push_back(newNode);
		clause1.flipped.push_back(0);
		for (int i = 0; i < deps.size(); i++) {
			clause1.flipped[i] = !clause1.flipped[i];
			CNFClause clause2;
			clause2.nodes = {deps[i], newNode};
			clause2.flipped = {flipped[i], 1};
			clauses.push_back(clause2);
		}
		clauses.push_back(clause1);
	}
	return newNode;
}

std::string CNF::exp() {
	std::string res = "";
	for (CNFClause& clause : clauses) res += clause.exp() + "\n";
	return res;
}
