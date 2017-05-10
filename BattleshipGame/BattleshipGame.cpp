#include "BattleshipGameManager.h"
#include "BattleshipGameManagerTest.h"
#include "InputProcessor.h"
#include "StringUtilsTest.h"
#include "BattleshipGameAlgoFromFileTest.h"
#include "InputProcessorTest.h"
#include "FileUtilsTest.h"

#define TEST false

int main(int argc, char* argv[])
{
	if (TEST) {
		/*StringUtilsTest::runTests();
		FileUtilsTest::runTests();
		InputProcessorTest::runTests();
		BattleshipGameAlgoFromFileTest::runTests();
		BattleshipGameManagerTest::runTests();*/
		//brake to see test results;
	} else {
	
		InputProcessor inputProcessor(argc, argv);
		if (!inputProcessor.validateFolderPath())
		{
			return -1;
		}
		BattleshipGameManager battleshipManneger(inputProcessor);

		if (!battleshipManneger.initGame())
		{
			Sleep(7000);
			return -1;
		}
		battleshipManneger.playGame();
		Sleep(2000);
	}
}

