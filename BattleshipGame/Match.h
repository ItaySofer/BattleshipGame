#ifndef MatchH
#define MatchH
#include "MatchResult.h"
#include "BattleBoard.h"

class Match
{
public:
	int boardIndex;
	int playerAIndex;
	int playerBIndex;
	MatchResult res;

	Match(int board, int playerA, int playerB) : boardIndex(board), playerAIndex(playerA), playerBIndex(playerB){}
};

#endif