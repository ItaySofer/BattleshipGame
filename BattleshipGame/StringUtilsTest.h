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
	}

private:
	static void replaceAllOneSpaceAppearanceTest();
	static void replaceAllOneCrAppearanceTest();
	static void replaceAllMultipleSpaceAppearancesTest();
	static void splitTest();
};

#endif

