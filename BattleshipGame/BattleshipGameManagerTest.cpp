#include "BattleshipGameManagerTest.h"
#include "MockBattleshipGameAlgo.h"

void BattleshipGameManagerTest::checkTestBoards()
{
	MockBattleshipGameAlgo playerA;
	MockBattleshipGameAlgo playerB;
	BattleshipGameManager battleshipManneger = BattleshipGameManager(playerA, playerB);

	checkBoard(battleshipManneger, "good_board_0.sboard");
	checkBoard(battleshipManneger, "good_board_1.sboard");
	checkBoard(battleshipManneger, "bad_board_0.sboard");
	checkBoard(battleshipManneger, "bad_board_1.sboard");
	checkBoard(battleshipManneger, "bad_board_2.sboard");

	std::getchar();
}

void BattleshipGameManagerTest::checkBoard(BattleshipGameManager& battleshipManneger, std::string path) {
	std::cout << "********************************************************" << std::endl;
	std::cout << path << " initGame result:" << std::endl;
	battleshipManneger.initGame(path);
	output(battleshipManneger.gameBoard);
	std::cout << "********************************************************" << std::endl;
	std::cout << std::endl;
}
void BattleshipGameManagerTest::output(BattleBoard& battleBoard)
{
	for (int i = 0; i < battleBoard.R; ++i)
	{
		for (int j = 0; j < battleBoard.C; ++j)
		{
			std::cout << battleBoard.matrix[i][j];
		}
		std::cout << std::endl;
	}
}