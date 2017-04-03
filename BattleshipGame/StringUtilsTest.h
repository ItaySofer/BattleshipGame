#ifndef StringUtilsTestH
#define StringUtilsTestH

#include "StringUtils.h"
#include <string>

class StringUtilsTest
{
public:
	static void runTests()
	{
		replaceAllOneSpaceAppearanceTest();
		replaceAllOneCrAppearanceTest();
		replaceAllMultipleSpaceAppearancesTest();
		splitTest();
		isNumberNumberTest();
		isNumberNotNumberTest();
	}

private:
	static void replaceAllOneSpaceAppearanceTest();
	static void replaceAllOneCrAppearanceTest();
	static void replaceAllMultipleSpaceAppearancesTest();
	static void splitTest();
	static void isNumberNumberTest();
	static void isNumberNotNumberTest();
};

#endif

