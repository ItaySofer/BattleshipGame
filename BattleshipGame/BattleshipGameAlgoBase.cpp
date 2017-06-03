#include "BattleshipGameAlgoBase.h"

BattleshipGameAlgoBase::BattleshipGameAlgoBase() {}
BattleshipGameAlgoBase::~BattleshipGameAlgoBase() {}

void BattleshipGameAlgoBase::setPlayer(int player)
{
	myPlayerNumber = player;
}

void BattleshipGameAlgoBase::setBoard(const BoardData& board) {
	rows = board.rows();
	cols = board.cols();
	depth = board.depth();

	//Initialize boolean matrix
	std::vector<std::vector<std::vector<bool>>> dontAttackMatrix;
	
	for (int k = 0; k < board.depth(); k++) {
		std::vector<std::vector<bool>> rowvector;
		for (int i = 0; i < board.rows(); i++) {
			std::vector<bool> colvector;
			for (int j = 0; j < board.cols(); j++) {
				colvector.push_back(false);
			}
			rowvector.push_back(colvector);
		}
		dontAttackMatrix.push_back(rowvector);
	}


	//Find dontAttack positions
	for (int k = 0; k < board.depth(); k++) {
		for (int i = 0; i < board.rows(); i++) {
			for (int j = 0; j < board.cols(); j++) {
				if (board.charAt(Coordinate(i + 1, j + 1, k + 1)) != ' ') {
					dontAttackMatrix[k][i][j] = true;
					if (k - 1 >= 0)			dontAttackMatrix[k-1][i][j] = true;
					if (i - 1 >= 0)			dontAttackMatrix[k][i - 1][j] = true;
					if (j - 1 >= 0)			dontAttackMatrix[k][i][j - 1] = true;
					if (j + 1 < board.cols())	dontAttackMatrix[k][i][j + 1] = true;
					if (i + 1 < board.rows())	dontAttackMatrix[k][i + 1][j] = true;
					if (k + 1 < board.depth())	dontAttackMatrix[k + 1][i][j] = true;
				}
			}
		}
	}

	//Count attack positions
	int count = 0;
	for (int k = 0; k < board.depth(); k++) {
		for (int i = 0; i < board.rows(); i++) {
			for (int j = 0; j < board.cols(); j++) {
				if (!dontAttackMatrix[k][i][j]) count++;
			}
		}
	}

	//Set attack positions vector
	attackPosVec.reserve(count);
	for (int k = 0; k < board.depth(); k++) {
		for (int i = 0; i < board.rows(); i++) {
			for (int j = 0; j < board.cols(); j++) {
				if (!dontAttackMatrix[k][i][j]) attackPosVec.push_back(Coordinate(i, j, k));
			}
		}
	}
	//Set attack positions vector's iterator to vector begin
	attackPosVecIt = attackPosVec.begin();
}

void BattleshipGameAlgoBase::notifyOnAttackResult(int player, Coordinate move, AttackResult result) {}

void BattleshipGameAlgoBase::adjustPosStartFrom1(Coordinate& pos) {
	pos.row++;//adjust to one-based range
	pos.col++;//adjust to one-based range
	pos.depth++;//adjust to one-based range
}