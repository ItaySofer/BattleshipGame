#include "BattleshipGameManager.h"
#include "BattleshipGameManagerTest.h"
#include "BattleshipGameAlgo.h"
#include "InputProcessor.h"
#include "StringUtilsTest.h"
#include "BattleshipGameAlgoTest.h"
#include "InputProcessorTest.h"

#define TEST false

int main(int argc, char* argv[])
{
	if (TEST) {
		StringUtilsTest::runTests();
		InputProcessorTest::runTests();
		BattleshipGameAlgoTest::runTests();
		//BattleshipGameManagerTest::runTests();
		//brake to see test results;
	} else {
		std::string path = argc == 1 ? "" : argv[1];

		InputProcessor inputProcessor = InputProcessor(path);
		if (!inputProcessor.tryExtractFileNames())
		{
			return -1;
		}
		if (!inputProcessor.validateInput()) {
			return -1;
		}

		BattleshipGameAlgo playerA(inputProcessor.getPlayerAAttackFilePath());
		BattleshipGameAlgo playerB(inputProcessor.getPlayerBAttackFilePath());
		BattleshipGameManager battleshipManneger = BattleshipGameManager(playerA, playerB);
		battleshipManneger.initGame(inputProcessor.getBoardFilePath());
		battleshipManneger.playGame();
	}
}

