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

	if (inputProcessor.getBoardFilePath() != "board.sboard")
	{
		std::cout << "extractFileNamesCurrentDirectoryTest Failed: Expected sboard file is ";
		std::cout << "board.sboard";
		std::cout << " but was ";
		std::cout << inputProcessor.getBoardFilePath();
		std::cout << std::endl;
	}
}
