#include "BattleshipGameManager.h"
#include "BattleshipGameAlgo.h"
#include "InputValidator.h"
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
		char* workDirectory;
		char* path;
		path = argc == 1 ? workDirectory : argv[1];

		InputValidator inputValidator = InputValidator(path);
		if (!inputValidator.isInputValid()) {
			return;
		}

		IBattleshipGameAlgo* playerA = new BattleshipGameAlgo(path, "attack-a");
		IBattleshipGameAlgo* playerB = new BattleshipGameAlgo(path, "attack-b");
		BattleshipGameManager battleshipManneger = BattleshipGameManager(*playerA, *playerB);
		battleshipManneger.initGame();
		battleshipManneger.playGame();

		delete playerA;
		delete playerB;
	}
	
}

