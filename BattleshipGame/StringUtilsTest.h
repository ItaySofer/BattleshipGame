#ifndef StringUtilsTestH
#define StringUtilsTestH

#include "StringUtils.h"
#include "Macros.h"
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
		splitNormalTest();
		splitComplexTest();
		isNumberNumberTest();
		isNumberNotNumberTest();
		endsWithPositiveTest();
		endsWithNegativeTest();
	}

private:
	static void replaceAllOneSpaceAppearanceTest();
	static void replaceAllOneCrAppearanceTest();
	static void replaceAllMultipleSpaceAppearancesTest();
	static void splitNormalTest();
	static void splitComplexTest();
	static void isNumberNumberTest();
	static void isNumberNotNumberTest();
	static void endsWithPositiveTest();
	static void endsWithNegativeTest();
};

#endif

