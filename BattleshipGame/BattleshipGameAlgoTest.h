#ifndef BattleshipGameAlgoTestH
#define BattleshipGameAlgoTestH

#include "BattleshipGameAlgo.h"
#include "Macros.h"

class BattleshipGameAlgoTest
{
public:
	static void runTests()
	{
		attackTest();
		attackTestFromOtherFolder();
	}

private:
	static void attackTest();
	static void attackTestFromOtherFolder();
};


#endif
