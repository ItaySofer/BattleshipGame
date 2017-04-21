#pragma once
#ifndef MockBattleshipGameAlgoH
#define MockBattleshipGameAlgoH

#include "IBattleshipGameAlgo.h"
#include <fstream>
#include <vector>
#include <iostream>
#include "StringUtils.h"
#include <string>
#include "Macros.h"

class MockBattleshipGameAlgo : public IBattleshipGameAlgo {

public:
	MockBattleshipGameAlgo() {}; //initiates player (folderPath to attack file? already initiates attack file itself?)
	~MockBattleshipGameAlgo() override {};

	void setBoard(int player, const char** board, int numRows, int numCols) override {}; // called once to notify player on his board
	bool init(const std::string& path)
	{
		return true;
	}
	std::pair<int, int> attack() override { return std::make_pair(1, 1); }; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override {}; // notify on last move result
};

#endif
