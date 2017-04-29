#ifndef BattleBoardH
#define BattleBoardH

#include <string>

class BattleBoard
{
public:
	int R;
	int C;
	std::string* matrix;

	BattleBoard() : R(0), C(0), matrix(nullptr) {}

	BattleBoard(const BattleBoard&) = delete;
	BattleBoard& operator=(const BattleBoard&) = delete;

	~BattleBoard()
	{
		if (matrix != nullptr)
		{
			delete[] matrix;
		}
	}
};

#endif
