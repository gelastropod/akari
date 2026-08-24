#include "akaricnf.h"

int calcRow(CNF& cnf, std::vector<int>& res, const std::vector<int>& row, int s, int e) {
	if (s == e) return row[s];
	int m = (s + e) / 2;
	int left = calcRow(cnf, res, row, s, m);
	int right = calcRow(cnf, res, row, m + 1, e);
	int resOr = cnf.addIntermediate({left, right}, {0, 0}, 0);
	int resAnd = cnf.addIntermediate({left, right}, {0, 0}, 1);
	res.push_back(resAnd);
	return resOr;
}

int assertRow(CNF& cnf, const std::vector<int>& row) {
	std::vector<int> res;
	int rowOr = calcRow(cnf, res, row, 0, row.size() - 1);
	if (res.size()) {
		std::vector<bool> flipped(res.size(), 0);
		int rowAssert = cnf.addIntermediate(res, flipped, 0);
		cnf.addAssert(rowAssert, 0);
	}
	return rowOr;
}

void fillColumn(CNF& cnf, std::vector<std::vector<int>>& columnIndices, const Akari& akari, const std::vector<std::vector<int>>& cnfIndices, int i, int j) {
	std::vector<int> crntRow;
	int crnti = i;
	while (crnti < akari.h && akari.board[crnti][j] == 0) {
		crntRow.push_back(cnfIndices[crnti][j]);
		crnti++;
	}
	if (i == crnti) return;
	int rowOr = assertRow(cnf, crntRow);
	for (int k = i; k < crnti; k++) columnIndices[k][j] = rowOr;
}

void fillRow(CNF& cnf, std::vector<std::vector<int>>& rowIndices, const Akari& akari, const std::vector<std::vector<int>>& cnfIndices, int i, int j) {
	std::vector<int> crntRow;
	int crntj = j;
	while (crntj < akari.w && akari.board[i][crntj] == 0) {
		crntRow.push_back(cnfIndices[i][crntj]);
		crntj++;
	}
	if (j == crntj) return;
	int rowOr = assertRow(cnf, crntRow);
	for (int k = j; k < crntj; k++) rowIndices[i][k] = rowOr;
}

std::pair<int, int> dirs[] = {
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1}
};

void assertSpecial(CNF& cnf, const Akari& akari, const std::vector<int>& adjNodes, int req) {
	if (req == 0) for (int node : adjNodes) cnf.addAssert(node, 0);
	else if (req == adjNodes.size()) for (int node : adjNodes) cnf.addAssert(node, 1);
	else if (req == 1) {
		std::vector<bool> flipped(adjNodes.size(), 1);
		std::vector<int> indAsserts;
		for (int k = 0; k < adjNodes.size(); k++) {
			auto flippedAug = flipped;
			flippedAug[k] = 0;
			int assertNode = cnf.addIntermediate(adjNodes, flippedAug, 1);
			indAsserts.push_back(assertNode);
		}
		CNFClause clause;
		clause.nodes = indAsserts;
		clause.flipped.resize(adjNodes.size(), 0);
		cnf.clauses.push_back(clause);
	}
	else if (req == adjNodes.size() - 1) {
		std::vector<bool> flipped(adjNodes.size(), 0);
		std::vector<int> indAsserts;
		for (int k = 0; k < adjNodes.size(); k++) {
			auto flippedAug = flipped;
			flippedAug[k] = 1;
			int assertNode = cnf.addIntermediate(adjNodes, flippedAug, 1);
			indAsserts.push_back(assertNode);
		}
		CNFClause clause;
		clause.nodes = indAsserts;
		clause.flipped.resize(adjNodes.size(), 0);
		cnf.clauses.push_back(clause);
	}
	else {
		std::vector<bool> flipped(4, 1);
		std::vector<int> indAsserts;
		for (int k = 0; k < 4; k++) {
			for (int l = k + 1; l < 4; l++) {
				auto flippedAug = flipped;
				flippedAug[k] = flippedAug[l] = 0;
				int assertNode = cnf.addIntermediate(adjNodes, flippedAug, 1);
				indAsserts.push_back(assertNode);
			}
		}
		CNFClause clause;
		clause.nodes = indAsserts;
		clause.flipped.resize(6, 0);
		cnf.clauses.push_back(clause);
	}
}

std::vector<std::vector<int>> calcAkari(CNF& cnf, const Akari& akari) {
	std::vector<std::vector<int>> cnfIndices(akari.h, std::vector<int>(akari.w, -1));
	for (int i = 0; i < akari.h; i++) {
		for (int j = 0; j < akari.w; j++) {
			if (akari.board[i][j] == 0) cnfIndices[i][j] = cnf.addInput();
		}
	}
	std::vector<std::vector<int>> rowIndices(akari.h, std::vector<int>(akari.w, -1)),
								  columnIndices(akari.h, std::vector<int>(akari.w, -1));
	for (int i = 0; i < akari.h; i++) {
		for (int j = 0; j < akari.w; j++) {
			if (columnIndices[i][j] == -1) fillColumn(cnf, columnIndices, akari, cnfIndices, i, j);
			if (rowIndices[i][j] == -1) fillRow(cnf, rowIndices, akari, cnfIndices, i, j);
		}
	}
	for (int i = 0; i < akari.h; i++) {
		for (int j = 0; j < akari.w; j++) {
			if (rowIndices[i][j] == -1) continue;
			CNFClause clause;
			clause.nodes = {rowIndices[i][j], columnIndices[i][j]};
			clause.flipped = {0, 0};
			cnf.clauses.push_back(clause);
		}
	}
	for (int i = 0; i < akari.h; i++) {
		for (int j = 0; j < akari.w; j++) {
			if (akari.board[i][j] == 0 || akari.board[i][j] == 6) continue;
			std::vector<int> adjNodes;
			for (int k = 0; k < 4; k++) {
				int ci = i + dirs[k].first, cj = j + dirs[k].second;
				if (ci < 0 || ci >= akari.h || cj < 0 || cj >= akari.w || akari.board[ci][cj] != 0) continue;
				adjNodes.push_back(cnfIndices[ci][cj]);
			}
			assertSpecial(cnf, akari, adjNodes, akari.board[i][j] - 1);
		}
	}
	return cnfIndices;
}
