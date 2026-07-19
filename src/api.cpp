#include "api.h"

#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int get(std::string link, std::string& response) {
	cpr::Response resp = cpr::Get(cpr::Url{link});
	response = resp.text;

	if (resp.error) {
        std::cerr << "cpr error: " << resp.error.message << " (code " << (int)resp.error.code << ")\n";
    }

	return resp.status_code;
}

int getAkari(int day, std::string& code) {
	char* link;
	sprintf(link, "https://dailyakari.com/archivepuzzle?number=%d", day);

	std::string response;
	get(std::string(link), response);

	json object = json::parse(response);
	code = object["puzzlink"];
}
