#ifndef StringUtilsH
#define StringUtilsH
#include <string>
#include <vector>

class StringUtils
{
public:
	static void replaceAll(std::string &s, const std::string &search, const std::string &replace);
	static void StringUtils::split(std::string& s, const std::string delimiter, std::vector<std::string>& result);
};

#endif

