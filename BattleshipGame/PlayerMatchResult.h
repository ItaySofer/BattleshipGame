#ifndef PlayerMatchResultH
#define PlayerMatchResultH

enum class MatchResultEnum
{
	Won,
	Lost,
	None
};

class PlayerMatchResult
{
public:
	MatchResultEnum res = MatchResultEnum::None;
	int pointsFor = 0;
	int pointsAgainst = 0;

	PlayerMatchResult() {};
};

#endif
