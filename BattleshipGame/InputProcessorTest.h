#ifndef InputProcessorTestH
#define InputProcessorTestH

#include "InputProcessor.h"
#include <iostream>
#include <string>

class InputProcessorTest
{
public:
	static void runTests()
	{
		extractFileNamesCurrentDirectoryTest();
		extractFileNamesOtherDirectoryTest();
		isInputValidCurrentDirectoryTest();
		isInputValidOtherDirectoryTest();
		isInputValidRelativeDirectoryTest();
		WrongPathTest();
		invalidInputTest();
	}

private:
	static void extractFileNamesCurrentDirectoryTest();
	static void extractFileNamesOtherDirectoryTest();
	static void isInputValidCurrentDirectoryTest();
	static void isInputValidOtherDirectoryTest();
	static void isInputValidRelativeDirectoryTest();
	static void WrongPathTest();
	static void invalidInputTest();
};

#endif

