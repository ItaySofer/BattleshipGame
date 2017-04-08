#include "BattleshipGameAlgo.h"


BattleshipGameAlgo::BattleshipGameAlgo(std:: string attackFilePath) {
	attackFile = std::ifstream(attackFilePath);
}

BattleshipGameAlgo:: ~BattleshipGameAlgo() {
	if (attackFile.is_open()) {
		attackFile.close();
	}
}


void BattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols) {}

std::pair<int, int> BattleshipGameAlgo::attack() {
	std::string line;
	while (std::getline(attackFile, line))
	{
		StringUtils::replaceAll(line, "\r", "");
		StringUtils::replaceAll(line, " ", "");

		std::vector<std::string> pos;
		StringUtils::split(line, ",", pos);

		if (isValidAttackMove(pos)) {
			return std::pair<int, int>(std::stoi(pos[0]) - 1, std::stoi(pos[1]) - 1);
		}
	}
	
	return std::pair<int, int>(-1, -1);
}

void BattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result) {}


bool BattleshipGameAlgo::isValidAttackMove(std::vector<std::string> pos)
{
	if (pos.size() != 2) {
		return false;
	}

	if(!StringUtils::isNumber(pos[0]) || !StringUtils::isNumber(pos[1])) {
		return false;
	}

	if (std::stoi(pos[0]) == 0 || std::stoi(pos[0]) > NUM_ROWS || std::stoi(pos[1]) == 0 || std::stoi(pos[1]) > NUM_COLS) {
		return false;
	}

	return true;
}
