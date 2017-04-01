#include "BattleshipGameAlgo.h"
#include "StringUtils.h"
#include <string>

extern int RowNum;
extern int ColNum;

BattleshipGameAlgo::BattleshipGameAlgo(std:: string path, std::string attackFileSuffix) {
	std::string fileName = getCompleteAttackFilePath(path, attackFileSuffix);
	attackFile = std::ifstream(fileName);
}

BattleshipGameAlgo:: ~BattleshipGameAlgo() {
	if (attackFile.is_open()) {
		attackFile.close();
	}
}


void BattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols) {}

std::pair<int, int> BattleshipGameAlgo::attack() {
	std::string line;
	while (attackFile.good())
	{
		std::getline(attackFile, line);
		StringUtils::replaceAll(line, "\r", "");
		StringUtils::replaceAll(line, " ", "");

		std::vector<std::string> pos;
		StringUtils::split(line, ",", pos);

		if (isValidAttackMove(pos)) {
			return std::pair<int, int>(std::stoi(pos[0]), std::stoi(pos[1]));
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

	if (std::stoi(pos[0]) > RowNum || std::stoi(pos[1]) > ColNum) {
		return false;
	}

	return true;
}
