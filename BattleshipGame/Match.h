#ifndef MatchH
#define MatchH
#include "BattleBoard.h"

class Match
{
public:
	int boardIndex;
	int playerAIndex;
	int playerBIndex;

	Match(int board, int playerA, int playerB) : boardIndex(board), playerAIndex(playerA), playerBIndex(playerB){}
};

#endif