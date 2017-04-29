#ifndef BattleshipGameAlgoNaiveH
#define BattleshipGameAlgoNaiveH

#include "BattleshipGameAlgoBase.h"
//#include <fstream>
//#include <vector>
//#include <iostream>
//#include "StringUtils.h"
//#include <string>
//#include "Macros.h"

class BattleshipGameAlgoNaive : public BattleshipGameAlgoBase {

public:
	//BattleshipGameAlgoNaive();
	//~BattleshipGameAlgoNaive() override;

	std::pair<int, int> attack() override; // ask player for his move
};

#endif
