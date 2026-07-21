#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

struct ClauseNode {
	int nodeID;
	bool flipped;
};

struct BooleanExpression {
	static std::vector<BooleanExpression*> globalExpressions;
	static std::unordered_map<uint64_t, std::vector<int>> mapExpressions;
	static std::vector<int> satisfyExpressions;

	bool flipped = 0, connector = 0;
	int id;
	std::vector<BooleanExpression*> child;

	BooleanExpression();

	uint64_t hash() const;
	void reg();
	void newVar();

	BooleanExpression operator!() const;
	BooleanExpression operator||(const BooleanExpression& other) const;
	BooleanExpression operator&&(const BooleanExpression& other) const;

	void setGoal(bool goal);

	static void getClauses(std::vector<std::vector<ClauseNode>>& clauses);
	static void printCNF(std::string& out);
};
