#include "BattleshipGameManagerTest.h"
#include "BattleshipGameAlgo.h"

void BattleshipGameManagerTest::checkTestBoards()
{
	BattleshipGameAlgo playerA();
	BattleshipGameAlgo playerB();
	BattleshipGameManager gameManager(playerA, playerB);
	gameManager.initGame();
	//bad_board_0.sboard
	//bad_board_1.sboard
	//bad_board_2.sboard
	//good_board_0.sboard
	//good_board_1.sboard
}
void BattleshipGameManagerTest::output(const BattleBoard& battleBoard)
{
	std::cout << "Printing battleBoard from instance into standard output" << std::endl;
	for (int i = 0; i < battleBoard.R; ++i)
	{
		for (int j = 0; j < battleBoard.C; ++j)
		{
			std::cout << battleBoard.matrix[i][j];
		}
		std::cout << std::endl;
	}
}