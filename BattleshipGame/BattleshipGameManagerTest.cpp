#include "BattleshipGameManagerTest.h"
#include "MockBattleshipGameAlgo.h"
#include "MockInputProcessor.h"

void BattleshipGameManagerTest::checkTestBoards()
{
	checkBoard("good_board_0.sboard");
	checkBoard("good_board_1.sboard");
	checkBoard("bad_board_0.sboard");
	checkBoard("bad_board_1.sboard");
	checkBoard("bad_board_2.sboard");
	checkBoard("bad_board_3.sboard");

	std::getchar();
}

void BattleshipGameManagerTest::checkBoard(std::string path) {
	MockInputProcessor inputProcessor(path);
	BattleshipGameManager battleshipManneger = BattleshipGameManager(inputProcessor);

	std::cout << "********************************************************" << std::endl;
	std::cout << path << " Runnung battleshipManneger.initGame(gameBoardPath):" << std::endl;
	battleshipManneger.initGame();
	output(battleshipManneger.gameBoard);
	std::cout << "********************************************************" << std::endl;
	std::cout << std::endl;
}

void BattleshipGameManagerTest::output(BattleBoard& battleBoard)
{
	std::cout << "Game Board:" << std::endl;
	for (int i = 0; i < battleBoard.R; ++i)
	{
		for (int j = 0; j < battleBoard.C; ++j)
		{
			std::cout << battleBoard.matrix[i][j];
		}
		std::cout << std::endl;
	}
}