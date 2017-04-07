#ifndef BattleshipGameAlgoH
#define BattleshipGameAlgoH

#include "IBattleshipGameAlgo.h"
#include <fstream>
#include <vector>

extern int RowNum;
extern int ColNum;

class BattleshipGameAlgo: public IBattleshipGameAlgo {
	std::ifstream attackFile;

public:
	BattleshipGameAlgo(std::string attackFilePath); //initiates player (path to attack file? already initiates attack file itself?)
	~BattleshipGameAlgo() override;

	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

private:
	std::string getCompleteAttackFilePath(std::string path, std::string fileSuffix);
	bool isValidAttackMove(std::vector<std::string> pos);
};

#endif
