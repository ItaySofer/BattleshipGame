#include "InputProcessorTest.h"

void InputProcessorTest::extractFileNamesCurrentDirectoryTest()
{
	int argc = 1;
	char** argv = new char*[1];
	argv[0] = "test";
	InputProcessor inputProcessor(argc, argv);
	inputProcessor.tryExtractBoardFileName();

	if (inputProcessor.getBoardFilePath() != "bad_board_0.sboard")
	{
		std::cout << "extractFileNamesCurrentDirectoryTest Failed: Expected sboard file is ";
		std::cout << "board.sboard";
		std::cout << " but was ";
		std::cout << inputProcessor.getBoardFilePath();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::extractFileNamesOtherDirectoryTest()
{
	int argc = 2;
	char** argv = new char*[2];
	argv[0] = "test";
	argv[1] = "C:/Users/Itay/Desktop/examples";
	InputProcessor inputProcessor(argc, argv);
	inputProcessor.tryExtractBoardFileName();
	inputProcessor.tryExtractDllFileNames();

	std::string expectedDllA = "C:/Users/Itay/Desktop/examples/a.dll";
	if (inputProcessor.getPlayerADllFilePath() != expectedDllA)
	{
		std::cout << "extractFileNamesOtherDirectoryTest Failed: Expected playerA dll file is ";
		std::cout << expectedDllA;
		std::cout << " but was ";
		std::cout << inputProcessor.getPlayerADllFilePath();
		std::cout << std::endl;
	}

	std::string expectedDllB = "C:/Users/Itay/Desktop/examples/b.dll";
	if (inputProcessor.getPlayerBDllFilePath() != expectedDllB)
	{
		std::cout << "extractFileNamesOtherDirectoryTest Failed: Expected playerB dll file is ";
		std::cout << expectedDllB;
		std::cout << " but was ";
		std::cout << inputProcessor.getPlayerBDllFilePath();
		std::cout << std::endl;
	}

	std::string expectedBoard = "C:/Users/Itay/Desktop/examples/bad_board_0.sboard";
	if (inputProcessor.getBoardFilePath() != expectedBoard)
	{
		std::cout << "extractFileNamesOtherDirectoryTest Failed: Expected sboard file is ";
		std::cout << expectedBoard;
		std::cout << " but was ";
		std::cout << inputProcessor.getBoardFilePath();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::isInputValidOtherDirectoryTest()
{
	int argc = 2;
	char** argv = new char*[2];
	argv[0] = "test";
	argv[1] = "C:\\Users\\Itay\\Desktop\\examples";
	InputProcessor inputProcessor(argc, argv);

	if (!inputProcessor.validateInput())
	{
		std::cout << "isInputValidOtherDirectoryTest Failed: Expected is true but was false";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::isInputValidRelativeDirectoryTest()
{
	int argc = 2;
	char** argv = new char*[2];
	argv[0] = "test";
	argv[1] = "relativePath";
	InputProcessor inputProcessor(argc, argv);

	if (!inputProcessor.validateInput())
	{
		std::cout << "isInputValidRelativeDirectoryTest Failed: Expected is true but was false";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::WrongPathTest()
{
	int argc = 2;
	char** argv = new char*[2];
	argv[0] = "test";
	argv[1] = "\\someWrongPath";
	InputProcessor inputProcessor(argc, argv);

	std::cout << std::endl << "[CHECK]" << std::endl;
	std::cout << "WrongPathTest: Check next line contains wrong path error: " << std::endl;

	inputProcessor.validateFolderPath();

	std::cout << "[CHECK]" << std::endl << std::endl;
	std::cout << std::endl;
}

void InputProcessorTest::invalidInputTest()
{
	int argc = 2;
	char** argv = new char*[2];
	argv[0] = "test";
	argv[1] = "C:\\Users\\Itay\\Desktop\\emptyFolder";
	InputProcessor inputProcessor(argc, argv);

	std::cout << std::endl<< "[CHECK]" << std::endl;
	std::cout << "invalidInputTest: Check next lines contains all files missing errors: " << std::endl;

	inputProcessor.validateInput();

	std::cout << "[CHECK]" << std::endl;
	std::cout << std::endl;
}

void InputProcessorTest::argumentParsingTest1()
{
	int argc = 4;
	char** argv = new char*[4];
	argv[0] = "test";
	argv[1] = "C:\\Users\\Itay\\Desktop\\emptyFolder";
	argv[2] = "-delay600";
	argv[3] = "-quiet";
	InputProcessor inputProcessor(argc, argv);

	if (inputProcessor.folderPath != argv[1])
	{
		std::cout << "argumentParsingTest1 Failed: Expected folderPath is ";
		std::cout << argv[1];
		std::cout << " but was ";
		std::cout << inputProcessor.folderPath;
		std::cout << std::endl;
	}

	if (inputProcessor.getDelayMs() != 600)
	{
		std::cout << "argumentParsingTest1 Failed: Expected delayMs is ";
		std::cout << "600";
		std::cout << " but was ";
		std::cout << inputProcessor.getDelayMs();
		std::cout << std::endl;
	}

	if (inputProcessor.getQuiet() != true)
	{
		std::cout << "argumentParsingTest1 Failed: Expected quiet is ";
		std::cout << "true";
		std::cout << " but was ";
		std::cout << inputProcessor.getQuiet();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::argumentParsingTest2()
{
	int argc = 4;
	char** argv = new char*[4];
	argv[0] = "test";
	argv[1] = "C:\\Users\\Itay\\Desktop\\emptyFolder";
	argv[2] = "-quiet";
	argv[3] = "-delay600";
	InputProcessor inputProcessor(argc, argv);

	if (inputProcessor.folderPath != argv[1])
	{
		std::cout << "argumentParsingTest2 Failed: Expected folderPath is ";
		std::cout << argv[1];
		std::cout << " but was ";
		std::cout << inputProcessor.folderPath;
		std::cout << std::endl;
	}

	if (inputProcessor.getDelayMs() != 600)
	{
		std::cout << "argumentParsingTest2 Failed: Expected delayMs is ";
		std::cout << "600";
		std::cout << " but was ";
		std::cout << inputProcessor.getDelayMs();
		std::cout << std::endl;
	}

	if (inputProcessor.getQuiet() != true)
	{
		std::cout << "argumentParsingTest2 Failed: Expected quiet is ";
		std::cout << "true";
		std::cout << " but was ";
		std::cout << inputProcessor.getQuiet();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::argumentParsingTest3()
{
	int argc = 3;
	char** argv = new char*[3];
	argv[0] = "test";
	argv[1] = "-delay600";
	argv[2] = "-quiet";
	InputProcessor inputProcessor(argc, argv);

	if (inputProcessor.folderPath != "")
	{
		std::cout << "argumentParsingTest3 Failed: Expected folderPath is ";
		std::cout << "\"\"";
		std::cout << " but was ";
		std::cout << inputProcessor.folderPath;
		std::cout << std::endl;
	}

	if (inputProcessor.getDelayMs() != 600)
	{
		std::cout << "argumentParsingTest3 Failed: Expected delayMs is ";
		std::cout << "600";
		std::cout << " but was ";
		std::cout << inputProcessor.getDelayMs();
		std::cout << std::endl;
	}

	if (inputProcessor.getQuiet() != true)
	{
		std::cout << "argumentParsingTest3 Failed: Expected quiet is ";
		std::cout << "true";
		std::cout << " but was ";
		std::cout << inputProcessor.getQuiet();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::argumentParsingTest4()
{
	int argc = 3;
	char** argv = new char*[3];
	argv[0] = "test";
	argv[1] = "-quiet";
	argv[2] = "-delay600";
	InputProcessor inputProcessor(argc, argv);

	if (inputProcessor.folderPath != "")
	{
		std::cout << "argumentParsingTest4 Failed: Expected folderPath is ";
		std::cout << "\"\"";
		std::cout << " but was ";
		std::cout << inputProcessor.folderPath;
		std::cout << std::endl;
	}

	if (inputProcessor.getDelayMs() != 600)
	{
		std::cout << "argumentParsingTest4 Failed: Expected delayMs is ";
		std::cout << "600";
		std::cout << " but was ";
		std::cout << inputProcessor.getDelayMs();
		std::cout << std::endl;
	}

	if (inputProcessor.getQuiet() != true)
	{
		std::cout << "argumentParsingTest4 Failed: Expected quiet is ";
		std::cout << "true";
		std::cout << " but was ";
		std::cout << inputProcessor.getQuiet();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::argumentParsingTest5()
{
	int argc = 3;
	char** argv = new char*[3];
	argv[0] = "test";
	argv[1] = "-quiet";
	argv[2] = "-delay 600";
	InputProcessor inputProcessor(argc, argv);

	if (inputProcessor.folderPath != "")
	{
		std::cout << "argumentParsingTest5 Failed: Expected folderPath is ";
		std::cout << "\"\"";
		std::cout << " but was ";
		std::cout << inputProcessor.folderPath;
		std::cout << std::endl;
	}

	if (inputProcessor.getDelayMs() != 600)
	{
		std::cout << "argumentParsingTest5 Failed: Expected delayMs is ";
		std::cout << "600";
		std::cout << " but was ";
		std::cout << inputProcessor.getDelayMs();
		std::cout << std::endl;
	}

	if (inputProcessor.getQuiet() != true)
	{
		std::cout << "argumentParsingTest5 Failed: Expected quiet is ";
		std::cout << "true";
		std::cout << " but was ";
		std::cout << inputProcessor.getQuiet();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::argumentParsingTest6()
{
	int argc = 1;
	char** argv = new char*[1];
	argv[0] = "test";

	InputProcessor inputProcessor(argc, argv);

	if (inputProcessor.folderPath != "")
	{
		std::cout << "argumentParsingTest6 Failed: Expected folderPath is ";
		std::cout << "\"\"";
		std::cout << " but was ";
		std::cout << inputProcessor.folderPath;
		std::cout << std::endl;
	}

	if (inputProcessor.getDelayMs() != DEFAULT_DELAY_MS)
	{
		std::cout << "argumentParsingTest6 Failed: Expected delayMs is ";
		std::cout << DEFAULT_DELAY_MS;
		std::cout << " but was ";
		std::cout << inputProcessor.getDelayMs();
		std::cout << std::endl;
	}

	if (inputProcessor.getQuiet() != false)
	{
		std::cout << "argumentParsingTest6 Failed: Expected quiet is ";
		std::cout << "false";
		std::cout << " but was ";
		std::cout << inputProcessor.getQuiet();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::argumentParsingTest7()
{
	int argc = 2;
	char** argv = new char*[2];
	argv[0] = "test";
	argv[1] = "-delay 600";
	InputProcessor inputProcessor(argc, argv);

	if (inputProcessor.folderPath != "")
	{
		std::cout << "argumentParsingTest7 Failed: Expected folderPath is ";
		std::cout << "\"\"";
		std::cout << " but was ";
		std::cout << inputProcessor.folderPath;
		std::cout << std::endl;
	}

	if (inputProcessor.getDelayMs() != 600)
	{
		std::cout << "argumentParsingTest7 Failed: Expected delayMs is ";
		std::cout << "600";
		std::cout << " but was ";
		std::cout << inputProcessor.getDelayMs();
		std::cout << std::endl;
	}

	if (inputProcessor.getQuiet() != false)
	{
		std::cout << "argumentParsingTest7 Failed: Expected quiet is ";
		std::cout << "false";
		std::cout << " but was ";
		std::cout << inputProcessor.getQuiet();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void InputProcessorTest::argumentParsingTest8()
{
	int argc = 2;
	char** argv = new char*[2];
	argv[0] = "test";
	argv[1] = "-quiet";
	InputProcessor inputProcessor(argc, argv);

	if (inputProcessor.folderPath != "")
	{
		std::cout << "argumentParsingTest8 Failed: Expected folderPath is ";
		std::cout << "\"\"";
		std::cout << " but was ";
		std::cout << inputProcessor.folderPath;
		std::cout << std::endl;
	}

	if (inputProcessor.getDelayMs() != DEFAULT_DELAY_MS)
	{
		std::cout << "argumentParsingTest8 Failed: Expected delayMs is ";
		std::cout << DEFAULT_DELAY_MS;
		std::cout << " but was ";
		std::cout << inputProcessor.getDelayMs();
		std::cout << std::endl;
	}

	if (inputProcessor.getQuiet() != true)
	{
		std::cout << "argumentParsingTest8 Failed: Expected quiet is ";
		std::cout << "true";
		std::cout << " but was ";
		std::cout << inputProcessor.getQuiet();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}