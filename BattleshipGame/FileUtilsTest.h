#ifndef FileUtilsTestH
#define FileUtilsTestH

#include "FileUtils.h"
#include "Macros.h"
#include <string>
#include <iostream>
#include <vector>

class FileUtilsTest
{
public:
	static void runTests()
	{
		convertToAbsolutePathWorkingDirectoryTest();
		convertToAbsolutePathrelativePathTest();
		convertToAbsolutePathAbsolutePathTest();
	}

private:
	static void convertToAbsolutePathWorkingDirectoryTest();
	static void convertToAbsolutePathrelativePathTest();
	static void convertToAbsolutePathAbsolutePathTest();
};

#endif

