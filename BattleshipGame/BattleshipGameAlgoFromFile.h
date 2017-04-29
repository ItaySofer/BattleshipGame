#ifndef BattleshipGameAlgoFromFileH
#define BattleshipGameAlgoFromFileH

#include "IBattleshipGameAlgo.h"
#include <fstream>
#include <vector>
#include <iostream>
#include "StringUtils.h"
#include <string>
#include "Macros.h"
#include <queue>

class BattleshipGameAlgoFromFile: public IBattleshipGameAlgo {
	int playerNum;
	std::queue<std::pair<int, int>> attackQueue;

public:
	~BattleshipGameAlgoFromFile() = default;

	void setBoard(int player, const char** board, int numRows, int numCols) override; // called once to notify player on his board
	bool init(const std::string& path) override;
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

private:
	bool isValidAttackMove(const std::vector<std::string> pos) const;
	const std::string attackSuffix = ".attack";
};

#endif
