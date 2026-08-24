#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "akaricnf.h"

int main(int argc, char* argv[]) {
	assert(argc == 2);
	int day = stoi(std::string(argv[1]));

	std::cout << "========== Processing day: " << day << " ==========\n" << std::endl;

	AkariResult akariResult;
	int code = getAkari(day, akariResult);
	if (code != 200) {
		std::cout << "Failed to fetch: " << code << std::endl;
		return -1;
	}
	std::cout << "Akari result: " << akariResult << std::endl;

	Akari akari;
	code = loadAkari(akariResult, akari);
	if (code) {
		std::cout << "Failed to read akari" << std::endl;
		return -1;
	}
	std::cout << "Akari: " << akari << std::endl;

//	akari.w = 2;
//	akari.h = 2;
//	akari.board = {
//		{1, 0},
//		{0, 0}
//	};

	std::ofstream cnfFile("int.cnf");
	CNF cnf;
	std::vector<std::vector<int>> cnfIndices = calcAkari(cnf, akari);
	cnfFile << cnf.exp() << std::endl;
	cnfFile.close();

	std::system("./kissat int.cnf 2>/dev/null | grep -E '^(s|v) ' | sed -E 's/^[sv] //' > int");
	std::ifstream satFile("int");
	std::string SAT;
	satFile >> SAT;
	if (SAT == "UNSATISFIABLE") {
		std::cout << "Unsolvable akari!" << std::endl;
		return -1;
	}

	std::vector<bool> satisfiableValues(cnf.numNodes, 0);
	for (int i = 0; i < cnf.numNodes; i++) {
		int v;
		satFile >> v;
		satisfiableValues[i] = v > 0;
	}

	for (int i = 0; i < akari.h; i++) {
		for (int j = 0; j < akari.w; j++) {
			if (akari.board[i][j] != 0) {
				std::cout << "0";
				continue;
			}
			std::cout << satisfiableValues[cnfIndices[i][j]];
		}
		std::cout << std::endl;
	}

	return 0;
}
