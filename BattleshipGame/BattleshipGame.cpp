#include "BattleshipGameManager.h"
#include "BattleshipGameManagerTest.h"
#include "BattleshipGameAlgo.h"
#include "InputProcessor.h"
#include "StringUtilsTest.h"
//testCommit
#define TEST true

int RowNum = 10;
int ColNum = 10;

int main(int argc, char* argv[])
{
	if (TEST) {
		StringUtilsTest::runTests();
		BattleshipGameManagerTest::runTests();
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

