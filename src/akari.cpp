#include "akari.h"

int discretise(const std::string& code, std::vector<int>& discretised) {
	for (int i = 0; i < code.size(); i++) {
		if ('0' <= code[i] && code[i] <= '9') discretised[i] = code[i] - '0';
		else if ('a' <= code[i] && code[i] <= 'z' && code[i] != 'f') discretised[i] = code[i] - 'a' + 10;
		else if (code[i] == '.') discretised[i] = 36;
		else return 1;
	}
	return 0;
}

int loadAkari(const AkariResult& akariResult, Akari& akari) {
	akari.w = akariResult.w, akari.h = akariResult.h;

	std::vector<int> discretisedBoard(akariResult.code.size()), flattenedBoard(akari.w * akari.h, 0);
	if (discretise(akariResult.code, discretisedBoard)) return 1;

	for (int i = 0, j = 0; i < akari.w * akari.h && j < akariResult.code.size(); i++, j++) {
		if (discretisedBoard[j] <= 4) flattenedBoard[i] = discretisedBoard[j] + 1;
		else if (discretisedBoard[j] <= 9) flattenedBoard[i] = discretisedBoard[j] - 4, i++;
		else if (discretisedBoard[j] <= 14) flattenedBoard[i] = discretisedBoard[j] - 9, i += 2;
		else if (discretisedBoard[j] <= 35) i += discretisedBoard[j] - 16;
		else flattenedBoard[i] = 6;
	}

	akari.board = std::vector<std::vector<int>>(akari.h, std::vector<int>(akari.w, 0));
	for (int i = 0; i < akari.h; i++)
		for (int j = 0; j < akari.w; j++)
			akari.board[i][j] = flattenedBoard[i * akari.w + j];

	return 0;
}

std::ostream& operator<<(std::ostream& os, const Akari& akari) {
	os << "Akari(width=" << akari.w << ", height=" << akari.h << ", board=" << std::endl;
	for (int i = 0; i < akari.h; i++) {
		for (int j = 0; j < akari.w; j++) {
			if (akari.board[i][j] == 0) os << '.';
			else if (akari.board[i][j] == 6) os << 'b';
			else os << (char)('0' + akari.board[i][j] - 1);
		}
		os << std::endl;
	}
	return os << ")" << std::endl;
}
