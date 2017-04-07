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
		char* workDirectory;
		char* path;
		path = argc == 1 ? workDirectory : argv[1];

		InputProcessor inputProcessor = InputProcessor(path);
		if (!inputProcessor.isInputValid()) {
			return;
		}

		std::string playerAAttackFilePath = inputProcessor.getPlayerAAttackFilePath();
		std::string playerBAttackFilePath = inputProcessor.getPlayerBAttackFilePath();
		std::string boardFilePath = inputProcessor.getBoardFilePath();

		IBattleshipGameAlgo* playerA = new BattleshipGameAlgo(playerAAttackFilePath);
		IBattleshipGameAlgo* playerB = new BattleshipGameAlgo(playerBAttackFilePath);
		BattleshipGameManager battleshipManneger = BattleshipGameManager(*playerA, *playerB);
		battleshipManneger.initGame(boardFilePath);
		battleshipManneger.playGame();

		delete playerA;
		delete playerB;
	}
	
}

