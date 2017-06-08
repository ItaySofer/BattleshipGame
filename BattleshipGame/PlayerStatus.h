#ifndef PlayerStatusH
#define PlayerStatusH
#include <string>
#include "PlayerMatchResult.h"

class PlayerStatus
{
public:
	std::string name;
	int wins;
	int loses;
	double percent;
	int pointsFor;
	int pointsAgainst;
	std::vector<PlayerMatchResult> matches;

	PlayerStatus(std::string name) : name(name), wins(0), loses(0), percent(0), pointsFor(0), pointsAgainst(0) {};
};

#endif
