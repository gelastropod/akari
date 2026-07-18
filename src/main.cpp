#include <cassert>
#include <iostream>

#include "api.h"

int main(int argc, char* argv[]) {
	assert(argc == 2);
	int day = stoi(std::string(argv[1]));

	std::cout << day << std::endl;

	std::string response;
	int code = get("https://dailyakari.com/archivepuzzle?number=550", response);
	if (code) {
		std::cerr << "die" << code << std::endl;
		return 1;
	}
	std::cout << response << std::endl;

	return 0;
}
