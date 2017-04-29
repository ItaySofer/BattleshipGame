#include "BattleshipGameAlgoFromFile.h"
#include "FileUtils.h"


void BattleshipGameAlgoFromFile::setBoard(int player, const char** board, int numRows, int numCols)
{
	playerNum = player;
}

bool BattleshipGameAlgoFromFile::init(const std::string& path)
{
	std::string attackFilePath;
	std::vector<std::string> attackFiles = FileUtils::getFilesPathsBySuffix(path, attackSuffix);
	if (attackFiles.size() > playerNum) {
		attackFilePath = attackFiles[playerNum];
	} else if (attackFilePath.size() > playerNum - 1)
	{
		attackFilePath = attackFiles[playerNum - 1];
	} else
	{
		return false;
	}

	std::ifstream attackFile(attackFilePath);
	std::string line;
	while (std::getline(attackFile, line))
	{
		StringUtils::replaceAll(line, "\r", "");
		StringUtils::replaceAll(line, " ", "");

		std::vector<std::string> pos;
		StringUtils::split(line, ",", pos);

		if (isValidAttackMove(pos)) {
			attackQueue.push(std::pair<int, int>(std::stoi(pos[0]), std::stoi(pos[1])));
		}
	}

	if (attackFile.is_open()) {
		attackFile.close();
	}

	return true;
}

std::pair<int, int> BattleshipGameAlgoFromFile::attack() {
	if (attackQueue.size() > 0)
	{
		std::pair<int, int>& nextAttack(attackQueue.front());
		attackQueue.pop();
		return nextAttack;
	}
	
	return std::pair<int, int>(-1, -1);
}

void BattleshipGameAlgoFromFile::notifyOnAttackResult(int player, int row, int col, AttackResult result) {}

bool BattleshipGameAlgoFromFile::isValidAttackMove(const std::vector<std::string> pos) const
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
