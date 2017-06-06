#ifndef StringUtilsH
#define StringUtilsH
//#include <string>
#include <vector>

class StringUtils
{
public:
	static void replaceAll(std::string &s, const std::string &search, const std::string &replace);
	static void split(std::string s, const std::string& delimiter, std::vector<std::string>& result);
	static bool isNumber(const std::string& s);
	static bool StringUtils::endsWith(const std::string& value, const std::string& ending);
};

#endif

