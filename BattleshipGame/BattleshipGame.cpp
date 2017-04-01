#include "BattleshipGameManager.h"
#include "BattleshipGameAlgo.h"
#include "InputValidator.h"

int main(int argc, char* argv[])
{
	//test 
	char* workDirectory;
	char* path;
	path = argc == 1 ? workDirectory : argv[1];

	InputValidator inputValidator = InputValidator(path);
	if (!inputValidator.isInputValid()) {
		return;
	}

	IBattleshipGameAlgo* playerA = new BattleshipGameAlgo("attack-a");
	IBattleshipGameAlgo* playerB = new BattleshipGameAlgo("attack-b");
	BattleshipGameManager battleshipManneger = BattleshipGameManager(playerA, playerB);
	battleshipManneger.initGame();
	battleshipManneger.playGame();

	delete playerA;
	delete playerB;
}

