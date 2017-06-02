#ifndef PlayerStatusH
#define PlayerStatusH
#include <string>

class PlayerStatus
{
public:
	int round;
	std::string name;
	int wins;
	int loses;
	double percent;
	int pointsFor;
	int pointsAgainst;

	PlayerStatus(std::string name) : round(0), name(name), wins(0), loses(0), percent(0), pointsFor(0), pointsAgainst(0) {};
};

#endif
