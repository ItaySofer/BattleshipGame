#ifndef BattleshipGameAlgoSmartH
#define BattleshipGameAlgoSmartH

#include "BattleshipGameAlgoBase.h"

class BattleshipGameAlgoSmart : public BattleshipGameAlgoBase {

public:
	Coordinate attack() override; // ask player for his move
	void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override; // notify on last move result

private:

	enum class Direction {
		vertical = 0,
		up,
		down,
		horizontal,
		left,
		right,
		depth,
		above,
		under,
		none
	};

	Coordinate attackUp(bool done);
	Coordinate attackDown(bool done);
	Coordinate attackUpDown(bool done);
	Coordinate attackDownUp(bool done);
	Coordinate attackLeft(bool done);
	Coordinate attackRight(bool done);
	Coordinate attackLeftRight(bool done);
	Coordinate attackRightLeft(bool done);
	Coordinate attackAbove(bool done);
	Coordinate attackUnder(bool done);
	Coordinate attackAboveUnder(bool done);
	Coordinate attackUnderAbove(bool done);

	Coordinate attackBothDirections(Coordinate(BattleshipGameAlgoSmart::*firstAttackDirection)(bool), Coordinate(BattleshipGameAlgoSmart::*secondAttackDirection)(bool), bool done, Direction dir);
	void resetAttack();
	void removeShipsSurroundingPos();
	void removePosFromAttackPosVec(const Coordinate& attackedPos);
	Coordinate handlDoneDirection(bool done);
	static bool notMius1Coordinate(const Coordinate& coor);

	std::vector<Coordinate> attackHitPosVec;

	Direction direction = Direction::none;
	Direction lastAttackedDirection = Direction::none;
	std::vector<Direction> checkDirections = { Direction::up, Direction::down, 
											   Direction::left, Direction::right,
											   Direction::above, Direction::under };
};

#endif