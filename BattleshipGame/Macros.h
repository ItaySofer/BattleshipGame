#ifndef MacrosH
#define MacrosH

#define NUM_PLAYERS 2
#define NUM_ROWS 10
#define NUM_COLS 10
#define NUM_DEPTH 10
#define STARTING_PLAYER 0

#define NUM_OF_ERR_MESSAGE 13
#define NUM_OF_SHIP_TYPES 4
#define VALID_SHIP_NUM 5

#define DEFAULT_DELAY_MS 1000

#define A_COLOR 10
#define B_COLOR 11
#define HIT_COLOR 14
#define SINK_COLOR 12
#define MISS_COLOR 15

#define NUM_THREADS 4
#define THREADS_PARAM "-threads "
#define TEST false

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