#ifndef BattleshipGameAlgoFromFileH
#define BattleshipGameAlgoFromFileH

#include "IBattleshipGameAlgo.h"
#include <fstream>
#include <vector>
#include <iostream>
#include "StringUtils.h"
#include <string>
#include "Macros.h"

class BattleshipGameAlgoFromFile: public IBattleshipGameAlgo {
	std::ifstream attackFile;

public:
	~BattleshipGameAlgoFromFile() override;

	void setBoard(int player, const char** board, int numRows, int numCols) override; // called once to notify player on his board
	bool init(const std::string& path) override;
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

private:
	bool isValidAttackMove(std::vector<std::string> pos);
};

#endif
