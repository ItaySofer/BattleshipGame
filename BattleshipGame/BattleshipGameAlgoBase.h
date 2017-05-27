#ifndef BattleshipGameAlgoBaseH
#define BattleshipGameAlgoBaseH

#include "IBattleshipGameAlgo.h"
#include <vector>
#include <iostream>

class BattleshipGameAlgoBase : public IBattleshipGameAlgo {

public:
	BattleshipGameAlgoBase();
	~BattleshipGameAlgoBase() override;

	void setPlayer(int player) override;				// called every time the player changes his order
	void setBoard(const BoardData& board) override;		// called every time the player changes his order
	virtual Coordinate attack() override = 0;			// ask player for his move
	virtual void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override; // notify on last move result
	static void adjustPosStartFrom1(Coordinate& pos);

	std::vector<Coordinate> attackPosVec;
	std::vector<Coordinate>::iterator attackPosVecIt;

	int myPlayerNumber;
};

#endif
