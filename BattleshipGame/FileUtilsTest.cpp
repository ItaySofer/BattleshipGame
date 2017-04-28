#include "FileUtilsTest.h"

void FileUtilsTest::convertToAbsolutePathWorkingDirectoryTest()
{
	std::string path = "";
	std::string expected = "C:\\Users\\Itay\\documents\\visual studio 2015\\Projects\\BattleshipGame\\BattleshipGame";

	std::string actual = FileUtils::convertToAbsolutePath(path);

	if (actual != expected) {
		std::cout << "convertToAbsolutePathWorkingDirectoryTest Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}

void FileUtilsTest::convertToAbsolutePathrelativePathTest()
{
	std::string path = "./relativePath";
	std::string expected = "C:\\Users\\Itay\\documents\\visual studio 2015\\Projects\\BattleshipGame\\BattleshipGame\\relativePath";

	std::string actual = FileUtils::convertToAbsolutePath(path);

	if (actual != expected) {
		std::cout << "convertToAbsolutePathrelativePathTest Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}

void FileUtilsTest::convertToAbsolutePathAbsolutePathTest()
{
	std::string path = "C:\\Users\\Itay\\documents\\visual studio 2015\\Projects\\BattleshipGame\\BattleshipGame\\relativePath";
	std::string expected = "C:\\Users\\Itay\\documents\\visual studio 2015\\Projects\\BattleshipGame\\BattleshipGame\\relativePath";

	std::string actual = FileUtils::convertToAbsolutePath(path);

	if (actual != expected) {
		std::cout << "convertToAbsolutePathAbsolutePathTest Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}
