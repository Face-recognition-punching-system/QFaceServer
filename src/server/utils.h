#ifndef UTILS_H
#define UTILS_H

#include <Poco/JSON/Parser.h>
#include <string.h>
#include <stdlib.h>
#include "iconv.h"

namespace utils {
	std::string body(std::string&&, std::string&&);
	std::string body(std::string&&);
	std::string U2G(const std::string utf8);
	std::string G2U(const std::string gb2312);
}

#endif // !UTILS_H
