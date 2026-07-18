#include "api.h"

#include <cpr/cpr.h>

int get(std::string link, std::string& response) {
	cpr::Response resp = cpr::Get(cpr::Url{link});
	if (resp.status_code != 200) return 1;

	response = resp.text;
	return 0;
}
