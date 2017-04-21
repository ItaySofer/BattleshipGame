#ifndef BattleshipGameAlgoTestH
#define BattleshipGameAlgoTestH

#include "BattleshipGameAlgoFromFile.h"
#include "Macros.h"

class BattleshipGameAlgoFromFileTest
{
public:
	static void runTests()
	{
		attackTest();
		attackTestFromOtherFolder();
		attackTestFromExamples();
	}

private:
	static void attackTest();
	static void attackTestFromOtherFolder();
	static void attackTestFromExamples();
};


#endif
