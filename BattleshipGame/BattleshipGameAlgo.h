#ifndef BattleshipGameAlgoH
#define BattleshipGameAlgoH

#include "IBattleshipGameAlgo.h"
#include <fstream>
#include <vector>
#include <iostream>
#include "StringUtils.h"
#include <string>
#include "Macros.h"

class BattleshipGameAlgo: public IBattleshipGameAlgo {
	std::ifstream attackFile;

public:
	BattleshipGameAlgo(std::string attackFilePath); //initiates player (folderPath to attack file? already initiates attack file itself?)
	~BattleshipGameAlgo() override;

	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

private:
	bool isValidAttackMove(std::vector<std::string> pos);
};

#endif
