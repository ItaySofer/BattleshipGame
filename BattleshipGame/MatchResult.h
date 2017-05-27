#ifndef MatchResultH
#define MatchResultH

class MatchResult
{
public:
	char whoWon;
	int playerAScore;
	int playerBScore;

	MatchResult(char whoWon, int playerAScore, int playerBScore) : whoWon(whoWon), playerAScore(playerAScore), playerBScore(playerBScore) {}
	MatchResult() : whoWon(' '), playerAScore(0), playerBScore(0) {}
};

#endif