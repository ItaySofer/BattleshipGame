#include "BattleshipGameAlgoSmart.h"

IBattleshipGameAlgo* GetAlgorithm() {
	return new BattleshipGameAlgoSmart();
}

Coordinate BattleshipGameAlgoSmart::attack() {
	//In case that there are no previous "hit" positions, choose random position to attack from attackPosVec
	if (attackHitPosVec.empty()) {
		Coordinate res(-1, -1, -1);
		size_t size = attackPosVec.size();
		if (size > 0) {
			int pos = rand() % size;
			res = attackPosVec[pos];
			adjustPosStartFrom1(res);
			attackPosVec.erase(attackPosVec.begin() + pos);
		}
		return res;
	}
	else {
		switch (direction) {
			case Direction::none:{
				while (checkDirections.size() > 0) {
					int dirPos = rand() % checkDirections.size();
					Direction dir = checkDirections[dirPos];
					checkDirections.erase(checkDirections.begin() + dirPos);
					switch (dir) {
						case Direction::up: {
							Coordinate res = attackUp(false);
							if (notMius1Coordinate(res)) {
								return res;
							}
							break;
						}
						case Direction::down: {
							Coordinate res = attackDown(false);
							if (notMius1Coordinate(res)) {
								return res;
							}
							break;
						}
						case Direction::left: {
							Coordinate res = attackLeft(false);
							if (notMius1Coordinate(res)) {
								return res;
							}
							break;
						}
						case Direction::right: {
							Coordinate res = attackRight(false);
							if (notMius1Coordinate(res)) {
								return res;
							}
							break;
						}
						case Direction::above: {
							Coordinate res = attackAbove(false);
							if (notMius1Coordinate(res)) {
								return res;
							}
							break;
						}
						case Direction::under: {
							Coordinate res = attackUnder(false);
							if (notMius1Coordinate(res)) {
								return res;
							}
							break;
						}
						default: {
							//should not get here - just for compilation
							return (Coordinate(-1, -1, -1));
							break;
						}
					}
				}
				//In case we got here - can't attack up&down&left&right
				resetAttack();
				return attack();
				break;
			}

			case Direction::vertical:{
				int isUp = rand() % 2;
				if (isUp) {
					return attackUpDown(true);
				}
				else {
					return attackDownUp(true);
				}
				break;
			}

			case Direction::up:{
				return attackUp(true);
				break;
			}

			case Direction::down:{
				return attackDown(true);
				break;
			}

			case Direction::horizontal: {
				int isLeft = rand() % 2;
				if (isLeft) {
					return attackLeftRight(true);
				}
				else {
					return attackRightLeft(true);
				}
				break;
			}

			case Direction::left: {
				return attackLeft(true);
				break;
			}

			case Direction::right: {
				return attackRight(true);
				break;
			}

			case Direction::depth: {
				int isAbove = rand() % 2;
				if (isAbove) {
					return attackAboveUnder(true);
				}
				else {
					return attackUnderAbove(true);
				}
				break;
			}

			case Direction::above: {
				return attackAbove(true);
				break;
			}

			case Direction::under: {
				return attackUnder(true);
				break;
			}
			default:{
				//should not get here - just for compilation
				return (Coordinate(-1, -1, -1));
				break;
			}
		}
	}
}

Coordinate BattleshipGameAlgoSmart::handlDoneDirection(bool done)
{
	if (done) {
		resetAttack();
		return attack();
	}
	else {
		return (Coordinate(-1, -1, -1));
	}
}

Coordinate BattleshipGameAlgoSmart::attackUp(bool done) {
	//Check if can attack up
	Coordinate currPos = *(attackHitPosVec.begin());
	if (currPos.row - 1 >= 0) {
		currPos.row--;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::up;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	//In case we got here - can't attack up
	return handlDoneDirection(done);
}

Coordinate BattleshipGameAlgoSmart::attackDown(bool done) {
	//Check if can attack down
	Coordinate currPos = *(attackHitPosVec.end() - 1);
	if (currPos.row + 1 < rows) {
		currPos.row++;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::down;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	//In case we got here - can't attack down
	return handlDoneDirection(done);

}

Coordinate BattleshipGameAlgoSmart::attackUpDown(bool done) {
	return attackBothDirections(&BattleshipGameAlgoSmart::attackUp, &BattleshipGameAlgoSmart::attackDown, done, Direction::down);
}

Coordinate BattleshipGameAlgoSmart::attackDownUp(bool done) {
	return attackBothDirections(&BattleshipGameAlgoSmart::attackDown, &BattleshipGameAlgoSmart::attackUp, done, Direction::up);
}

Coordinate BattleshipGameAlgoSmart::attackLeft(bool done) {
	//Check if can attack left
	Coordinate currPos = *(attackHitPosVec.begin());
	if (currPos.col - 1 >= 0) {
		currPos.col--;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::left;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	
	//In case we got here - can't attack left
	return handlDoneDirection(done);
}

Coordinate BattleshipGameAlgoSmart::attackRight(bool done) {
	//Check if can attack right
	Coordinate currPos = *(attackHitPosVec.end() - 1);
	if (currPos.col + 1 < cols) {
		currPos.col++;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::right;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	//In case we got here - can't attack right
	return handlDoneDirection(done);
}

Coordinate BattleshipGameAlgoSmart::attackLeftRight(bool done) {
	return attackBothDirections(&BattleshipGameAlgoSmart::attackLeft, &BattleshipGameAlgoSmart::attackRight, done, Direction::right);
}

Coordinate BattleshipGameAlgoSmart::attackRightLeft(bool done) {
	return attackBothDirections(&BattleshipGameAlgoSmart::attackRight, &BattleshipGameAlgoSmart::attackLeft, done, Direction::left);
}

Coordinate BattleshipGameAlgoSmart::attackAbove(bool done) {
	//Check if can attack left
	Coordinate currPos = *(attackHitPosVec.begin());
	if (currPos.depth - 1 >= 0) {
		currPos.depth--;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::left;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}

	//In case we got here - can't attack left
	return handlDoneDirection(done);
}

Coordinate BattleshipGameAlgoSmart::attackUnder(bool done) {
	//Check if can attack right
	Coordinate currPos = *(attackHitPosVec.end() - 1);
	if (currPos.depth + 1 < depth) {
		currPos.depth++;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::right;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	//In case we got here - can't attack right
	return handlDoneDirection(done);
}

Coordinate BattleshipGameAlgoSmart::attackAboveUnder(bool done) {
	return attackBothDirections(&BattleshipGameAlgoSmart::attackAbove, &BattleshipGameAlgoSmart::attackUnder, done, Direction::right);
}

Coordinate BattleshipGameAlgoSmart::attackUnderAbove(bool done) {
	return attackBothDirections(&BattleshipGameAlgoSmart::attackUnder, &BattleshipGameAlgoSmart::attackAbove, done, Direction::left);
}

Coordinate BattleshipGameAlgoSmart::attackBothDirections(Coordinate(BattleshipGameAlgoSmart::*firstAttackDirection)(bool),
	Coordinate(BattleshipGameAlgoSmart::*secondAttackDirection)(bool), bool done, Direction dir)
{
	//Check if can attack first direction
	Coordinate currPos = (this->*firstAttackDirection)(false);
	if (notMius1Coordinate(currPos)) {
		return currPos;
	}

	//In case we got here - can't attack first direction - try second direction
	direction = dir;
	return (this->*secondAttackDirection)(done);
}

void BattleshipGameAlgoSmart::resetAttack() {
	attackHitPosVec.clear();
	direction = Direction::none;
	lastAttackedDirection = Direction::none;
	checkDirections = { Direction::up, Direction::down, Direction::left, Direction::right, Direction::above, Direction::under };
}

void BattleshipGameAlgoSmart::removeShipsSurroundingPos()
{
	for (auto pos : attackHitPosVec)
	{
		std::vector<Coordinate> surroundings = {	Coordinate(pos.row - 1, pos.col, pos.depth),
													Coordinate(pos.row + 1, pos.col, pos.depth),
													Coordinate(pos.row, pos.col - 1, pos.depth),
													Coordinate(pos.row, pos.col + 1, pos.depth),
													Coordinate(pos.row, pos.col, pos.depth - 1),
													Coordinate(pos.row, pos.col, pos.depth + 1)
														};
		for (auto surr : surroundings)
		{
			removePosFromAttackPosVec(surr);
		}
	}
}

void BattleshipGameAlgoSmart::removePosFromAttackPosVec(const Coordinate& attackedPos)
{
	auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), attackedPos);
	if (pos != attackPosVec.end()) {
		attackPosVec.erase(pos);
	}
}

void BattleshipGameAlgoSmart::notifyOnAttackResult(int player, Coordinate move, AttackResult result) {
	move.row--;//adjust back to zero-based range
	move.col--;//adjust back to zero-based range
	move.depth--;//adjust back to zero-based range
	Coordinate attackedPos = move;
	if (player == myPlayerNumber) {
		switch (result) {
			case AttackResult::Miss: {
				if (direction == Direction::vertical && lastAttackedDirection == Direction::up) {
					direction = Direction::down;
				}
				else if (direction == Direction::vertical && lastAttackedDirection == Direction::down) {
					direction = Direction::up;
				}
				else if (direction == Direction::horizontal && lastAttackedDirection == Direction::left) {
					direction = Direction::right;
				}
				else if (direction == Direction::horizontal && lastAttackedDirection == Direction::right) {
					direction = Direction::left;
				}
				else if (direction == Direction::depth && lastAttackedDirection == Direction::under) {
					direction = Direction::above;
				}
				else if (direction == Direction::depth && lastAttackedDirection == Direction::above) {
					direction = Direction::under;
				}
				else if	((direction == Direction::up)|| (direction == Direction::down)|| 
						(direction == Direction::left) || (direction == Direction::right) ||
						(direction == Direction::above) || (direction == Direction::under)) {
					resetAttack();
				}
				break;
			}
			case AttackResult::Hit: {
				//Update direction
				if ((direction == Direction::none && lastAttackedDirection == Direction::up) ||
					(direction == Direction::none && lastAttackedDirection == Direction::down)) {
					direction = Direction::vertical;
				}	
				else if ((direction == Direction::none && lastAttackedDirection == Direction::left) ||
					(direction == Direction::none && lastAttackedDirection == Direction::right)) {
					direction = Direction::horizontal;
				}
				else if ((direction == Direction::none && lastAttackedDirection == Direction::above) ||
					(direction == Direction::none && lastAttackedDirection == Direction::under)) {
					direction = Direction::depth;
				}
				//Update attackHitPosVec
				if ((lastAttackedDirection == Direction::none) || (lastAttackedDirection == Direction::left) || 
					(lastAttackedDirection == Direction::up) || (lastAttackedDirection == Direction::above)) {
					attackHitPosVec.insert(attackHitPosVec.begin(), attackedPos);
				}
				else{
					attackHitPosVec.push_back(attackedPos);
				}
				break;
			}
			case AttackResult::Sink: {
				attackHitPosVec.push_back(attackedPos);
				removeShipsSurroundingPos();
				resetAttack();
				break;
			}
			default: {
				break;
			}
		}
	}
	else{
		if (result != AttackResult::Hit) {
			removePosFromAttackPosVec(attackedPos);
		}
	}
}

bool BattleshipGameAlgoSmart::notMius1Coordinate(const Coordinate& coor)
{
	if ((coor.row != -1) || (coor.col != -1) || (coor.depth != -1)) return true;

	return false;
}