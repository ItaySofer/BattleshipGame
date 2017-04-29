#include "BattleshipGameAlgoSmart.h"

std::pair<int, int> BattleshipGameAlgoSmart::attack() {
	//In case that there are no previous "hit" positions, choose random position to attack from attackPosVec
	if (attackHitPosVec.empty()) {
		std::pair<int, int> res = std::make_pair(-1, -1);
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
							std::pair<int, int> res = attackUp(false);
							if (res != std::make_pair(-1, -1)) {
								return res;
							}
							break;
						}
						case Direction::down: {
							std::pair<int, int> res = attackDown(false);
							if (res != std::make_pair(-1, -1)) {
								return res;
							}
							break;
						}
						case Direction::left: {
							std::pair<int, int> res = attackLeft(false);
							if (res != std::make_pair(-1, -1)) {
								return res;
							}
							break;
						}
						case Direction::right: {
							std::pair<int, int> res = attackRight(false);
							if (res != std::make_pair(-1, -1)) {
								return res;
							}
							break;
						}
						default: {
							//should not get here - just for compilation
							return (std::make_pair(-1, -1));
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

			default:{
				//should not get here - just for compilation
				return (std::make_pair(-1, -1));
				break;
			}
		}
	}
}

std::pair<int, int> BattleshipGameAlgoSmart::attackUp(bool done) {
	//Check if can attack up
	std::pair<int, int> currPos = *(attackHitPosVec.begin());
	if (currPos.first - 1 >= 0) {
		currPos.first--;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::up;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	//In case we got here - can't attack up
	if (done) {
		resetAttack();
		return attack();
	}
	else {
		return (std::make_pair(-1, -1));
	}
}

std::pair<int, int> BattleshipGameAlgoSmart::attackDown(bool done) {
	//Check if can attack down
	std::pair<int, int> currPos = *(attackHitPosVec.end() - 1);
	if (currPos.first + 1 < rows) {
		currPos.first++;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::down;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	//In case we got here - can't attack down
	if (done) {
		resetAttack();
		return attack();
	}
	else {
		return (std::make_pair(-1, -1));
	}
}

std::pair<int, int> BattleshipGameAlgoSmart::attackUpDown(bool done) {
	//Check if can attack up
	std::pair<int, int> currPos = attackUp(false);
	if (currPos != std::make_pair(-1, -1)) {
		return currPos;
	}

	//In case we got here - can't attack up - try attack down
	return attackDown(done);
}

std::pair<int, int> BattleshipGameAlgoSmart::attackDownUp(bool done) {
	//Check if can attack down
	std::pair<int, int> currPos = attackDown(false);
	if (currPos != std::make_pair(-1, -1)) {
		return currPos;
	}

	//In case we got here - can't attack down - try attack up
	direction = Direction::up;
	return attackUp(done);
}

std::pair<int, int> BattleshipGameAlgoSmart::attackLeft(bool done) {
	//Check if can attack left
	std::pair<int, int> currPos = *(attackHitPosVec.begin());
	if (currPos.second - 1 >= 0) {
		currPos.second--;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::left;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	if (done) {
		resetAttack();
		return attack();
	}
	else {
		return (std::make_pair(-1, -1));
	}
}

std::pair<int, int> BattleshipGameAlgoSmart::attackRight(bool done) {
	//Check if can attack right
	std::pair<int, int> currPos = *(attackHitPosVec.end() - 1);
	if (currPos.second + 1 < cols) {
		currPos.second++;
		auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), currPos);
		if (pos != attackPosVec.end()) {//can attack in currPos
			lastAttackedDirection = Direction::right;
			attackPosVec.erase(pos);
			adjustPosStartFrom1(currPos);
			return currPos;
		}
	}
	//In case we got here - can't attack right
	if (done) {
		resetAttack();
		return attack();
	}
	else {
		return (std::make_pair(-1, -1));
	}
}

std::pair<int, int> BattleshipGameAlgoSmart::attackLeftRight(bool done) {
	//Check if can attack left
	std::pair<int, int> currPos = attackLeft(false);
	if (currPos != std::make_pair(-1, -1)) {
		return currPos;
	}

	//In case we got here - can't attack left - try attack right
	direction = Direction::right;
	return attackRight(done);
}

std::pair<int, int> BattleshipGameAlgoSmart::attackRightLeft(bool done) {
	//Check if can attack right
	std::pair<int, int> currPos = attackRight(false);
	if (currPos != std::make_pair(-1, -1)) {
		return currPos;
	}

	//In case we got here - can't attack right - try attack left
	direction = Direction::left;
	return attackLeft(done);
}

void BattleshipGameAlgoSmart::resetAttack() {
	attackHitPosVec.clear();
	direction = Direction::none;
	lastAttackedDirection = Direction::none;
	checkDirections = { Direction::up, Direction::down, Direction::left, Direction::right };
}

void BattleshipGameAlgoSmart::removeShipsSurroundingPos()
{
	for (auto pos : attackHitPosVec)
	{
		std::vector<std::pair<int, int>> surroundings = {	std::make_pair(pos.first - 1, pos.second),
															std::make_pair(pos.first + 1, pos.second),
															std::make_pair(pos.first, pos.second - 1),
															std::make_pair(pos.first, pos.second + 1)
														};
		for (auto surr : surroundings)
		{
			removePosFromAttackPosVec(surr);
		}
	}
}

void BattleshipGameAlgoSmart::removePosFromAttackPosVec(const std::pair<int, int>& attackedPos)
{
	auto pos = std::find(attackPosVec.begin(), attackPosVec.end(), attackedPos);
	if (pos != attackPosVec.end()) {
		attackPosVec.erase(pos);
	}
}

void BattleshipGameAlgoSmart::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	row--;//adjust back to zero-based range
	col--;//adjust back to zero-based range
	std::pair<int, int> attackedPos = std::make_pair(row, col);
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
				else if	((direction == Direction::up)|| (direction == Direction::down)|| 
						(direction == Direction::left) || (direction == Direction::right)) {
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
				//Update attackHitPosVec
				if ((lastAttackedDirection == Direction::none) || (lastAttackedDirection == Direction::left) || (lastAttackedDirection == Direction::up)) {
					attackHitPosVec.insert(attackHitPosVec.begin(), attackedPos);
				}
				else{
					attackHitPosVec.push_back(attackedPos);
				}
				break;
			}
			case AttackResult::Sink: {
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
