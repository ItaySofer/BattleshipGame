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
	void output(const BattleBoard& battleBoard);
};

#endif

