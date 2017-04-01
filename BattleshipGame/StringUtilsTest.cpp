#include "StringUtilsTest.h"
#include <iostream>


void StringUtilsTest::replaceAllOneSpaceAppearanceTest()
{
	std::string s = "Some String";
	StringUtils::replaceAll(s, " ", "");
	std::string expected = "Somestring";

	if (s.compare(expected) != 0) {
		std::cout << "replaceAllOneAppearanceTest Failed: Expected is " + expected + "but was " + s + "\n";
	}
}

void StringUtilsTest::replaceAllOneCrAppearanceTest()
{
	std::string s = "Some\rString";
	StringUtils::replaceAll(s, "\r", "");
	std::string expected = "Somestring";

	if (s.compare(expected) != 0) {
		std::cout << "replaceAllOneAppearanceTest Failed: Expected is " + expected + "but was " + s + "\n";
	}
}

void StringUtilsTest::replaceAllMultipleSpaceAppearancesTest()
{
	std::string s = "Another Some String ";
	StringUtils::replaceAll(s, " ", "");
	std::string expected = "AnotherSomeString";

	if (s.compare(expected) != 0) {
		std::cout << "replaceAllMultipleAppearancesTest Failed: Expected is " + expected + "but was " + s + "\n";
	}
}

void StringUtilsTest::splitTest()
{
	std::string s = "Another Some String ";

	std::vector<std::string> expected = { "Another", "Some", "String" };

	std::vector<std::string> actual;
	StringUtils::split(s, " ", actual);

	if (actual != expected) {
		std::cout << "splitTest Failed: Expected is ";
		std::cout << expected;
		std::cout << "but was ";
		std::cout << s;
		std::cout << + "\n";
	}
}
