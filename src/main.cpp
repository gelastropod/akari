#include <cassert>
#include <iostream>

#include "akari.h"
#include "cnf.h"

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

	CNF cnf;
	int a = cnf.addInput();
	int b = cnf.addInput();
	int c = cnf.addInput();
	int d = cnf.addInput();
	int e = cnf.addIntermediate({a, c, d}, {0, 1, 1}, 0);
	int f = cnf.addIntermediate({b, e, d}, {0, 0, 1}, 1);
	cnf.addAssert(f, 1);
	std::cout << cnf.exp() << std::endl;

	return 0;
}
