#include "IBattleshipGameAlgo.h"

class BattleshipGameAlgo: public IBattleshipGameAlgo {
public:
	BattleshipGameAlgo(char* attackFileSuffix); //initiates player (path to attack file? already initiates attack file itself?)
	~BattleshipGameAlgo() = default;

	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
};
