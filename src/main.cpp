#include <cassert>
#include <iostream>

#include "api.h"

int main(int argc, char* argv[]) {
	assert(argc == 2);
	int day = stoi(std::string(argv[1]));

	std::cout << "========== Processing day: " << day << " ==========\n" << std::endl;

	AkariResult akari;
	int code = getAkari(day, akari);
	if (code != 200) {
		std::cout << "Failed to fetch: " << code << std::endl;
		return -1;
	}
	std::cout << "Akari result: " << akari << std::endl;

	return 0;
}
