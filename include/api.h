#pragma once

#include <string>
#include <iostream>

struct AkariResult {
	int w, h;
	std::string code;
};

int getAkari(int day, AkariResult& akariResult);
std::ostream& operator<<(std::ostream& os, const AkariResult& akariResult);
