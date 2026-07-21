#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "api.h"

struct Akari {
	int w, h;
	std::vector<std::vector<int>> board;
};

int loadAkari(const AkariResult& akariResult, Akari& akari);
std::ostream& operator<<(std::ostream& os, const Akari& akari);
