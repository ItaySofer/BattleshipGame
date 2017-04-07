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
	~BattleBoard()
	{
		if (matrix != NULL)
		{
			delete[] matrix;
		}
	}
};

#endif
