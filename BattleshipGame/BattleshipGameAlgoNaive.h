#ifndef BattleshipGameAlgoNaiveH
#define BattleshipGameAlgoNaiveH

#include "BattleshipGameAlgoBase.h"

class BattleshipGameAlgoNaive : public BattleshipGameAlgoBase {

public:
	std::pair<int, int> attack() override; // ask player for his move
};

#endif
