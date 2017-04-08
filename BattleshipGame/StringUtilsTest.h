#ifndef StringUtilsTestH
#define StringUtilsTestH

#include "StringUtils.h"
#include <string>
#include <iostream>
#include <vector>

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
		endsWithPositiveTest();
		endsWithNegativeTest();
	}

private:
	static void replaceAllOneSpaceAppearanceTest();
	static void replaceAllOneCrAppearanceTest();
	static void replaceAllMultipleSpaceAppearancesTest();
	static void splitTest();
	static void isNumberNumberTest();
	static void isNumberNotNumberTest();
	static void endsWithPositiveTest();
	static void endsWithNegativeTest();
};

#endif

