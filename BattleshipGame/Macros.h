#ifndef MacrosH
#define MacrosH

#define NUM_PLAYERS 2
#define NUM_ROWS 10
#define NUM_COLS 10
#define STARTING_PLAYER 0

#define NUM_OF_ERR_MESSAGE 13
#define NUM_OF_SHIP_TYPES 4
#define VALID_SHIP_NUM 5

#define DEFAULT_DELAY_MS 1000

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
	os << '{';
	for (auto& el : vec)
	{
		os << el << ", ";
	}
	os << '}';
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::pair<T, T>& p)
{
	os << '(' << p.first << ", " << p.second << ')';
	return os;
}

#endif