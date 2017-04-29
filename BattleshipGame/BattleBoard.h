#ifndef BattleBoardH
#define BattleBoardH

#include <string>

class BattleBoard
{
public:
	int R;
	int C;
	std::string* matrix;

	BattleBoard() : matrix(NULL) {}

	//BattleBoard(const BattleBoard&) = delete;
	//BattleBoard& operator=(const BattleBoard&) = delete;

	~BattleBoard()
	{
		if (matrix != NULL)
		{
			delete[] matrix;
		}
	}
};

#endif
