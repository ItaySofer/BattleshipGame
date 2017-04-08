#include "StringUtilsTest.h"

void StringUtilsTest::replaceAllOneSpaceAppearanceTest()
{
	std::string s = "Some String";
	StringUtils::replaceAll(s, " ", "");
	std::string expected = "SomeString";

	if (s.compare(expected) != 0) {
		std::cout << "replaceAllOneSpaceAppearanceTest Failed: Expected is " + expected + " but was " + s << std::endl;
	}
}

void StringUtilsTest::replaceAllOneCrAppearanceTest()
{
	std::string s = "Some\rString\r";
	StringUtils::replaceAll(s, "\r", "");
	std::string expected = "SomeString";

	if (s.compare(expected) != 0) {
		std::cout << "replaceAllOneCrAppearanceTest Failed: Expected is " + expected + " but was " + s << std::endl;
	}
}

void StringUtilsTest::replaceAllMultipleSpaceAppearancesTest()
{
	std::string s = "Another Some String ";
	StringUtils::replaceAll(s, " ", "");
	std::string expected = "AnotherSomeString";

	if (s.compare(expected) != 0) {
		std::cout << "replaceAllMultipleAppearancesTest Failed: Expected is " + expected + " but was " + s << std::endl;
	}
}

void StringUtilsTest::splitNormalTest()
{
	std::string s = "Another Some String";

	std::vector<std::string> expected = { "Another", "Some", "String" };

	std::vector<std::string> actual;
	StringUtils::split(s, " ", actual);

	if (actual != expected) {
		std::cout << "splitNormalTest Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}

void StringUtilsTest::splitComplexTest()
{
	std::string s = "Another Some   String   ";

	std::vector<std::string> expected = { "Another", "Some", "String" };

	std::vector<std::string> actual;
	StringUtils::split(s, " ", actual);

	if (actual != expected) {
		std::cout << "splitComplexTest Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}

void StringUtilsTest::isNumberNumberTest() {
	if (!StringUtils::isNumber("10")){
		std::cout << "isNumberNumberTest Failed: Expected is true but was false" << std::endl;
	}
}

void StringUtilsTest::isNumberNotNumberTest() {
	if (StringUtils::isNumber("itay")) {
		std::cout << "isNumberNotNumberTest Failed: Expected is false but was true" << std::endl;
	}
}

void StringUtilsTest::endsWithPositiveTest()
{
	std::string s = "Another Some String";

	bool expected = true;

	bool actual = StringUtils::endsWith(s, "String");

	if (actual != expected) {
		std::cout << "endsWithPositiveTest Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}

void StringUtilsTest::endsWithNegativeTest()
{
	std::string s = "Another Some String";

	bool expected = false;

	bool actual = StringUtils::endsWith(s, "BlaBla");

	if (actual != expected) {
		std::cout << "endsWithPositiveTest Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}

