#include "InputProcessorTest.h"

void InputProcessorTest::extractFileNamesCurrentDirectoryTest()
{
	InputProcessor inputProcessor("");
	inputProcessor.tryExtractFileNames();
	if (inputProcessor.getPlayerAAttackFilePath() != "a.attack-a")
	{
		std::cout << "extractFileNamesCurrentDirectoryTest Failed: Expected attack-a file is ";
		std::cout << "a.attack-a";
		std::cout << " but was ";
		std::cout << inputProcessor.getPlayerAAttackFilePath();
		std::cout << std::endl;
	}

	if (inputProcessor.getPlayerBAttackFilePath() != "b.attack-b")
	{
		std::cout << "extractFileNamesCurrentDirectoryTest Failed: Expected attack-b file is ";
		std::cout << "b.attack-b";
		std::cout << " but was ";
		std::cout << inputProcessor.getPlayerBAttackFilePath();
		std::cout << std::endl;
	}

	if (inputProcessor.getBoardFilePath() != "bad_board_0.sboard")
	{
		std::cout << "extractFileNamesCurrentDirectoryTest Failed: Expected sboard file is ";
		std::cout << "board.sboard";
		std::cout << " but was ";
		std::cout << inputProcessor.getBoardFilePath();
		std::cout << std::endl;
	}
}

void InputProcessorTest::extractFileNamesOtherDirectoryTest()
{
	InputProcessor inputProcessor("C:\\Users\\Itay\\Desktop\\examples");
	inputProcessor.tryExtractFileNames();
	if (inputProcessor.getPlayerAAttackFilePath() != "C:\\Users\\Itay\\Desktop\\examples\\a.attack-a")
	{
		std::cout << "extractFileNamesOtherDirectoryTest Failed: Expected attack-a file is ";
		std::cout << "a.attack-a";
		std::cout << " but was ";
		std::cout << inputProcessor.getPlayerAAttackFilePath();
		std::cout << std::endl;
	}

	if (inputProcessor.getPlayerBAttackFilePath() != "C:\\Users\\Itay\\Desktop\\examples\\b.attack-b")
	{
		std::cout << "extractFileNamesOtherDirectoryTest Failed: Expected attack-b file is ";
		std::cout << "b.attack-b";
		std::cout << " but was ";
		std::cout << inputProcessor.getPlayerBAttackFilePath();
		std::cout << std::endl;
	}

	if (inputProcessor.getBoardFilePath() != "C:\\Users\\Itay\\Desktop\\examples\\bad_board_0.sboard")
	{
		std::cout << "extractFileNamesOtherDirectoryTest Failed: Expected sboard file is ";
		std::cout << "board.sboard";
		std::cout << " but was ";
		std::cout << inputProcessor.getBoardFilePath();
		std::cout << std::endl;
	}
}

void InputProcessorTest::isInputValidCurrentDirectoryTest()
{
	InputProcessor inputProcessor("");
	inputProcessor.tryExtractFileNames();

	if (!inputProcessor.validateInput())
	{
		std::cout << "isInputValidCurrentDirectoryTest Failed: Expected is treu but was false";
		std::cout << std::endl;
	}
}

void InputProcessorTest::isInputValidOtherDirectoryTest()
{
	InputProcessor inputProcessor("C:\\Users\\Itay\\Desktop\\examples");
	inputProcessor.tryExtractFileNames();

	if (!inputProcessor.validateInput())
	{
		std::cout << "isInputValidOtherDirectoryTest Failed: Expected is treu but was false";
		std::cout << std::endl;
	}
}

void InputProcessorTest::isInputValidRelativeDirectoryTest()
{
	InputProcessor inputProcessor("relativePath");
	inputProcessor.tryExtractFileNames();

	if (!inputProcessor.validateInput())
	{
		std::cout << "isInputValidRelativeDirectoryTest Failed: Expected is treu but was false";
		std::cout << std::endl;
	}
}

void InputProcessorTest::WrongPathTest()
{
	InputProcessor inputProcessor("\\someWrongPath");

	std::cout << std::endl << "[CHECK]" << std::endl;
	std::cout << "WrongPathTest: Check next line contains wrong path error: " << std::endl;

	inputProcessor.tryExtractFileNames();

	std::cout << "[CHECK]" << std::endl << std::endl;
}

void InputProcessorTest::invalidInputTest()
{
	InputProcessor inputProcessor("C:\\Users\\Itay\\Desktop\\emptyFolder");

	std::cout << std::endl<< "[CHECK]" << std::endl;
	std::cout << "invalidInputTest: Check next lines contains all files missing errors: " << std::endl;

	inputProcessor.tryExtractFileNames();
	inputProcessor.validateInput();

	std::cout << "[CHECK]" << std::endl;
}
