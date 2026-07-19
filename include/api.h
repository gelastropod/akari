#pragma once

#include <string>
#include <iostream>

struct AkariResult {
	int width, height;
	std::string code;
};

int getAkari(int day, AkariResult& akari);
std::ostream& operator<<(std::ostream& os, const AkariResult& akari);