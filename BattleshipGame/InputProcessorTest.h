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
		isInputValidOtherDirectoryTest();
		isInputValidRelativeDirectoryTest();
		WrongPathTest();
		invalidInputTest();
		argumentParsingTest1();
		argumentParsingTest2();
		argumentParsingTest3();
		argumentParsingTest4();
		argumentParsingTest5();
		argumentParsingTest6();
		argumentParsingTest7();
		argumentParsingTest8();
	}

private:
	static void extractFileNamesCurrentDirectoryTest();
	static void extractFileNamesOtherDirectoryTest();
	static void isInputValidOtherDirectoryTest();
	static void isInputValidRelativeDirectoryTest();
	static void WrongPathTest();
	static void invalidInputTest();
	static void argumentParsingTest1();
	static void argumentParsingTest2();
	static void argumentParsingTest3();
	static void argumentParsingTest4();
	static void argumentParsingTest5();
	static void argumentParsingTest6();
	static void argumentParsingTest7();
	static void argumentParsingTest8();
};

#endif

