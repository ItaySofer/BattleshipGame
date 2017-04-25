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
		sameAttackFile();
	}

private:
	static void attackTest();
	static void attackTestFromOtherFolder();
	static void attackTestFromExamples();
	static void sameAttackFile();
};


#endif
