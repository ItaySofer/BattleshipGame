#ifndef BattleBoardH
#define BattleBoardH

#include <string>

class BattleBoard : public BoardData
{
public:

	std::string** matrix;

	BattleBoard() : matrix(nullptr)
	{
		_rows = 0;
		_cols = 0;
		_depth = 0;
	}

	BattleBoard(const BattleBoard&) = delete;
	BattleBoard& operator=(const BattleBoard&) = delete;

	~BattleBoard() override
	{
		if (matrix != nullptr)
		{
			for (int i = 0; i < _depth; i++)
			{
				if (matrix[i] != nullptr)
				{
					delete[] matrix[i];
				}
			}
		}
	}

	char charAt(Coordinate c) const override //returns only selected players' chars
	{
		return matrix[c.depth][c.row][c.col];//TODO: do we need to check witch player called charAt first?
	}

	void setRows(int rows) { _rows = rows; }
	void setCols(int cols) { _cols = cols; }
	void setDepth(int depth) { _depth = depth; }
};

#endif
