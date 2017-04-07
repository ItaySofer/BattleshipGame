#include "BattleshipGameManager.h"
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
	} else {
		std::string path = argc == 1 ? "" : argv[1];

		InputProcessor inputProcessor = InputProcessor(path);
		if (!inputProcessor.tryExtractFileNames())
		{
			return;
		}
		if (!inputProcessor.validateInput()) {
			return;
		}

		IBattleshipGameAlgo* playerA = new BattleshipGameAlgo(inputProcessor.getPlayerAAttackFilePath());
		IBattleshipGameAlgo* playerB = new BattleshipGameAlgo(inputProcessor.getPlayerBAttackFilePath());
		BattleshipGameManager battleshipManneger = BattleshipGameManager(*playerA, *playerB);
		battleshipManneger.initGame(inputProcessor.getBoardFilePath());
		battleshipManneger.playGame();

		delete playerA;
		delete playerB;
	}
	
}

