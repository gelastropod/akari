#include "api.h"

#include <sstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int get(const std::string& link, std::string& response) {
	cpr::Response resp = cpr::Get(cpr::Url{link});
	response = resp.text;

	if (resp.error) std::cerr << "cpr error: " << resp.error.message << " (code " << (int)resp.error.code << ")\n";

	return resp.status_code;
}

int getAkari(int day, AkariResult& akariResult) {
	std::string link = "https://dailyakari.com/archivepuzzle?number=" + std::to_string(day);

	std::string response;
	int code = get(link, response);
	if (code != 200) return code;

	json object = json::parse(response);
	std::string akariLink = object["puzzlink"].get<std::string>();
	std::string akariCode = akariLink.substr(26);

	std::istringstream iss(akariCode);
	std::string width_string, height_string;
	std::getline(iss, width_string, '/');
	std::getline(iss, height_string, '/');
	std::getline(iss, akariResult.code, '/');
	akariResult.w = std::stoi(width_string);
	akariResult.h = std::stoi(height_string);

	return 200;
}

std::ostream& operator<<(std::ostream& os, const AkariResult& akariResult) {
	return os << "AkariResult(width=" << akariResult.w << ", height=" << akariResult.h << ", code=" << akariResult.code << ")";
}
