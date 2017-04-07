#ifndef BattleshipGameManagerH
#define BattleshipGameManagerH

#include "IBattleshipGameAlgo.h"
#include "StringUtils.h"
#include "Macros.h"
#include <iostream>
#include <string>
#include <fstream>

class BattleshipGameManager {
	IBattleshipGameAlgo& playerA;
	IBattleshipGameAlgo& playerB;

	enum class shipSize {
		bSize = 1,
		pSize,
		mSize,
		dSize
	};

	enum class ErrorMsg {
		WRONG_SIZE_SHAPE_D_A = 0,	//"Wrong size or shape for ship d for player A"
		WRONG_SIZE_SHAPE_M_A,		//"Wrong size or shape for ship m for player A"
		WRONG_SIZE_SHAPE_P_A,		//"Wrong size or shape for ship p for player A"
		WRONG_SIZE_SHAPE_B_A,		//"Wrong size or shape for ship b for player A"
		WRONG_SIZE_SHAPE_D_B,		//"Wrong size or shape for ship d for player B"
		WRONG_SIZE_SHAPE_M_B,		//"Wrong size or shape for ship m for player B"
		WRONG_SIZE_SHAPE_P_B,		//"Wrong size or shape for ship p for player B"
		WRONG_SIZE_SHAPE_B_B,		//"Wrong size or shape for ship b for player B"
		TOO_MANY_A,					//"Too many ships for player A"
		TOO_FEW_A,					//"Too few ships for player A"
		TOO_MANY_B,					//"Too many ships for player B"
		TOO_FEW_B,					//"Too few ships for player B"
		AJACENT_ON_BOARD,			//"Adjacent Ships on Board"
		ERR_MGS_MAX
	};
		
	std::pair<bool, std::string> errMsgArr[NUM_OF_ERR_MESSAGE] ={	std::make_pair(false, "Wrong size or shape for ship d for player A"),
																	std::make_pair(false, "Wrong size or shape for ship m for player A"),
																	std::make_pair(false, "Wrong size or shape for ship p for player A"),
																	std::make_pair(false, "Wrong size or shape for ship b for player A"),
																	std::make_pair(false, "Wrong size or shape for ship d for player B"),
																	std::make_pair(false, "Wrong size or shape for ship m for player B"),
																	std::make_pair(false, "Wrong size or shape for ship p for player B"),
																	std::make_pair(false, "Wrong size or shape for ship b for player B"),
																	std::make_pair(false, "Too many ships for player A"),
																	std::make_pair(false, "Too few ships for player A"),
																	std::make_pair(false, "Too many ships for player B"),
																	std::make_pair(false, "Too few ships for player B"),
																	std::make_pair(false, "Adjacent Ships on Board")
																};

	char typeArr[NUM_OF_SHIP_TYPES*2] = {'D', 'M', 'P', 'B', 'd', 'm', 'p' ,'b'};

public:
	BattleshipGameManager(IBattleshipGameAlgo& plrA, IBattleshipGameAlgo& plrB) : playerA(plrA), playerB(plrB) {};
	~BattleshipGameManager() = default;

	bool initGame(const std::string boardFile); //initiates board, calls players' "setBoard" methods
	void sendBoard(bool isPlayerA);
	void modifyBoard(char** board, bool isPlayerA);
	void playGame(); //runs game, notifies players about move results. Continiusly checks game status and ends it when needed.
	//Output propper messages ("Player _ won. Points: ...")

	void readBoardFileToMatrix(const std::string boardFile);//initiate game board from file.
	bool validateBoard();//check if board is valid according to game specifications
	bool isValidShipRight(int x, int y);//check if a valid ship starts at (x,y) position to the right
	bool isValidShipBottom(int x, int y);//check if a valid ship starts at (x,y) position to the bottom
	int getSize(char type);//return ship valid size by given type
	void updateErrMsgArrWrongSize(char type);//updates wrong size error for given type in errMsgArr

	BattleBoard gameBoard;
};

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