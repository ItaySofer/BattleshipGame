#ifndef BattleshipGameAlgoSmartH
#define BattleshipGameAlgoSmartH

#include "BattleshipGameAlgoBase.h"

class BattleshipGameAlgoSmart : public BattleshipGameAlgoBase {

public:
	BattleshipGameAlgoSmart();
	~BattleshipGameAlgoSmart() override;

	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

private:
	std::pair<int, int> attackUp(bool done);
	std::pair<int, int> attackDown(bool done);
	std::pair<int, int> attackUpDown(bool done);
	std::pair<int, int> attackDownUp(bool done);
	std::pair<int, int> attackLeft(bool done);
	std::pair<int, int> attackRight(bool done);
	std::pair<int, int> attackLeftRight(bool done);
	std::pair<int, int> attackRightLeft(bool done);
	std::pair<int, int> resetAttack();

	enum class Direction {
		vertical = 0,
		up,
		down,
		horizontal,
		left,
		right,
		none
	};

	std::vector<std::pair<int, int>> attackHitPosVec;

	Direction direction = Direction::none;
	Direction lastAttackedDirection = Direction::none;
	std::vector<Direction> checkDirections = { Direction::up, Direction::down, Direction::left, Direction::right };
};

#endif