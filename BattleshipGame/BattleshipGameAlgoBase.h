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
	virtual Coordinate attack() override = 0; // ask player for his move
	virtual void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override; // notify on last move result
	static void adjustPosStartFrom1(Coordinate& pos);

	std::vector<Coordinate> attackPosVec;
	std::vector<Coordinate>::iterator attackPosVecIt;

	int myPlayerNumber;
	int rows;
	int cols;
	int dep;
};

#endif
