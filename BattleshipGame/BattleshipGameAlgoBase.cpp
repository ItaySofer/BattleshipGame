#include "BattleshipGameAlgoBase.h"

BattleshipGameAlgoBase::BattleshipGameAlgoBase() {}
BattleshipGameAlgoBase::~BattleshipGameAlgoBase() {}

void BattleshipGameAlgoBase::setBoard(int player, const char** board, int numRows, int numCols) {
	//Set myPlayerNumber
	myPlayerNumber = player;
	rows = numRows;
	cols = numCols;
	//Initialize boolean matrix
	bool** dontAttackMatrix;
	dontAttackMatrix = new bool*[numRows];
	for (int i = 0; i < numRows; i++)
	{
		dontAttackMatrix[i] = new bool[numCols]();
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

	//Delete dontAttackMatrix
	for (int i = 0; i < numRows; i++)
	{
		delete[] dontAttackMatrix[i];
	}
	delete[] dontAttackMatrix;
}

bool BattleshipGameAlgoBase::init(const std::string& path) { return true;}

void BattleshipGameAlgoBase::notifyOnAttackResult(int player, int row, int col, AttackResult result) {}

void BattleshipGameAlgoBase::adjustPosStartFrom1(std::pair<int, int>& pos) {
	pos.first++;//adjust to one-based range
	pos.second++;//adjust to one-based range
}