#include "BattleshipGameAlgoFromFile.h"


BattleshipGameAlgoFromFile::BattleshipGameAlgoFromFile(std:: string attackFilePath) {
	attackFile = std::ifstream(attackFilePath);
}

BattleshipGameAlgoFromFile:: ~BattleshipGameAlgoFromFile() {
	if (attackFile.is_open()) {
		attackFile.close();
	}
}


void BattleshipGameAlgoFromFile::setBoard(int player, const char** board, int numRows, int numCols) {}

bool BattleshipGameAlgoFromFile::init(const std::string& path)
{
	return true;
}

std::pair<int, int> BattleshipGameAlgoFromFile::attack() {
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

void BattleshipGameAlgoFromFile::notifyOnAttackResult(int player, int row, int col, AttackResult result) {}


bool BattleshipGameAlgoFromFile::isValidAttackMove(std::vector<std::string> pos)
{
	if (pos.size() != 2) {
		return false;
	}

	if(!StringUtils::isNumber(pos[0]) || !StringUtils::isNumber(pos[1])) {
		return false;
	}

	if (std::stoi(pos[0]) <= 0 || std::stoi(pos[0]) > NUM_ROWS || std::stoi(pos[1]) <= 0 || std::stoi(pos[1]) > NUM_COLS) {
		return false;
	}

	return true;
}
