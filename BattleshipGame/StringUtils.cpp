#include "StringUtils.h"
#include <iostream>
#include <vector>
#include <cctype>

void StringUtils::replaceAll(std::string& s, const std::string& search, const std::string& replace) {
	for (size_t pos = 0; ; pos += replace.length()) {
		// Locate the substring to replace
		pos = s.find(search, pos);
		if (pos == std::string::npos) break;
		// Replace by erasing and inserting
		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}

void StringUtils::split(std::string& s, const std::string delimiter, std::vector<std::string>& result) {
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		result.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
}

bool StringUtils::isNumber(const std::string& s)
{
	int i = 0;
	while (i < s.length() - 1) {
		if (!std::isdigit(s[i])) {
			return false;
		}
		i++;
	}
	return true;
}