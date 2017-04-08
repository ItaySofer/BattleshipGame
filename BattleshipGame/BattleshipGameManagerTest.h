#ifndef BattlshipGamemanagerTestH
#define BattlshipGamemanagerTestH

#include "BattleshipGameManager.h"

class BattleshipGameManagerTest
{
public:
	static void runTests()
	{
		checkTestBoards();
	}

private:

	static void checkTestBoards();
	static void checkBoard(BattleshipGameManager& battleshipManneger, std::string path);
	static void output(BattleBoard& battleBoard);
};

#endif

