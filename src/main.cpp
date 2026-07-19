#include <cassert>
#include <iostream>

#include "api.h"

int main(int argc, char* argv[]) {
	assert(argc == 2);
	int day = stoi(std::string(argv[1]));

	std::cout << day << std::endl;

	std::string response;
	int code = get("https://dailyakari.com/archivepuzzle?number=550", response);
	std::cout << response << std::endl;
	if (code != 200) {
		std::cout << "Failed to fetch: " << code << std::endl;
		return -1;
	}

	return 0;
}
