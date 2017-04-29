#include "BattleshipGameAlgoNaive.h"

//BattleshipGameAlgoNaive::BattleshipGameAlgoNaive() {}
//BattleshipGameAlgoNaive::~BattleshipGameAlgoNaive() {}

std::pair<int, int> BattleshipGameAlgoNaive::attack() {
	std::pair<int, int> res = std::make_pair(-1,-1);
	if (attackPosVecIt != attackPosVec.end()) {
		res = *attackPosVecIt;
		adjustPosStartFrom1(res);
		attackPosVecIt++;
	}
	return res;
}

