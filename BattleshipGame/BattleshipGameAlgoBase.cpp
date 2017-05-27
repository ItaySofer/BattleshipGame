#include "BattleshipGameAlgoBase.h"

BattleshipGameAlgoBase::BattleshipGameAlgoBase() {}
BattleshipGameAlgoBase::~BattleshipGameAlgoBase() {}

void BattleshipGameAlgoBase::setBoard(int player, const char** board, int numRows, int numCols) {
	//Set myPlayerNumber
	myPlayerNumber = player;
	rows = numRows;
	cols = numCols;

	//Initialize boolean matrix
	std::vector<std::vector<bool>> dontAttackMatrix;
	std::vector<bool> rowvector;
	for (int i = 0; i<rows; i++){
		rowvector.clear();
		for (int j = 0; j<cols; j++)
			rowvector.push_back(false);
		dontAttackMatrix.push_back(rowvector);
	}

	//Find dontAttack positions
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			if (board[i][j] != ' ') {
				dontAttackMatrix[i][j] = true;
				if (i-1 >= 0)			dontAttackMatrix[i-1][j] = true;
				if (j-1 >= 0)			dontAttackMatrix[i][j-1] = true;
				if (j + 1 < numCols)	dontAttackMatrix[i][j+1] = true;
				if (i + 1 < numRows)	dontAttackMatrix[i+1][j] = true;
			}
		}
	}
	//Count attack positions
	int count = 0;
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			if (!dontAttackMatrix[i][j]) count++;
		}
	}
	//Set attack positions vector
	attackPosVec.reserve(count);
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			if (!dontAttackMatrix[i][j]) attackPosVec.push_back(std::make_pair(i, j));
		}
	}
	//Set attack positions vector's iterator to vector begin
	attackPosVecIt = attackPosVec.begin();
}

bool BattleshipGameAlgoBase::init(const std::string& path) { return true;}

void BattleshipGameAlgoBase::notifyOnAttackResult(int player, Coordinate move, AttackResult result) {}

void BattleshipGameAlgoBase::adjustPosStartFrom1(Coordinate& pos) {
	pos.row++;//adjust to one-based range
	pos.col++;//adjust to one-based range
	pos.depth++;//adjust to one-based range
}