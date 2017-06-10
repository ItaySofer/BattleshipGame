#ifndef BattleBoardH
#define BattleBoardH

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

	BattleBoard(const BattleBoard& board) {
		_rows = board.rows();
		_cols = board.cols();
		_depth = board.depth();

		matrix = new std::string*[_depth];
		for (int k = 0; k < _depth; k++) {
			matrix[k] = new std::string[_rows];
			for (int i = 0; i < _rows; i++) {
				matrix[k][i] = board.matrix[k][i];
			}
		}
	}

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
			delete[] matrix;
		}
	}

	char charAt(Coordinate c) const override //returns only selected players' chars
	{
		if (c.depth < 1 || c.row < 1 || c.col < 1
			|| c.depth > depth() || c.row > rows() || c.col > cols())
		{
			return ' ';
		} else
		{
			return matrix[c.depth - 1][c.row - 1][c.col - 1];
		}
	}

	void setRows(int rows) { _rows = rows; }
	void setCols(int cols) { _cols = cols; }
	void setDepth(int depth) { _depth = depth; }
};

#endif
