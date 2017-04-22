#include "BattleshipGameManager.h"
#include "BattleshipGameManagerTest.h"
#include "BattleshipGameAlgoFromFile.h"
#include "InputProcessor.h"
#include "StringUtilsTest.h"
#include "BattleshipGameAlgoFromFileTest.h"
#include "InputProcessorTest.h"

#define TEST false

int main(int argc, char* argv[])
{
	if (TEST) {
		StringUtilsTest::runTests();
		InputProcessorTest::runTests();
		BattleshipGameAlgoFromFileTest::runTests();
		//BattleshipGameManagerTest::runTests();
		//brake to see test results;
	} else {
		std::string path = argc == 1 ? "" : argv[1];

		InputProcessor inputProcessor(path);
		if (!inputProcessor.tryExtractFileNames())
		{
			return -1;
		}
		if (!inputProcessor.validateInput()) {
			return -1;
		}

		BattleshipGameManager battleshipManneger(inputProcessor);

		if (!battleshipManneger.initGame())
		{
			return -1;
		}
		battleshipManneger.playGame();
	}
}

