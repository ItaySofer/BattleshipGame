#ifndef BattleshipGameAlgoBaseH
#define BattleshipGameAlgoBaseH

#include "IBattleshipGameAlgo.h"
#include <vector>
#include <iostream>

class BattleshipGameAlgoBase : public IBattleshipGameAlgo {

public:
	BattleshipGameAlgoBase();
	~BattleshipGameAlgoBase() override;

	void setBoard(int player, const char** board, int numRows, int numCols) override; // called once to notify player on his board
	bool init(const std::string& path) override;
	virtual std::pair<int, int> attack() override = 0; // ask player for his move TODO: check if syntax is correct
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	void adjustPosStartFrom1(std::pair<int, int>& pos);

	std::vector<std::pair<int, int>> attackPosVec;
	std::vector<std::pair<int, int>>::iterator attackPosVecIt;

	int myPlayerNumber;
	int rows;
	int cols;
};

#endif
