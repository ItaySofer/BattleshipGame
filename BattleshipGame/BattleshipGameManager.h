#ifndef BattleshipGameManagerH
#define BattleshipGameManagerH

#include "IBattleshipGameAlgo.h"
#include "BattleBoard.h"
#include "StringUtils.h"
#include "Macros.h"
#include <iostream>
#include <cstring>
#include <fstream>

class BattleshipGameManager {

public:
	BattleshipGameManager(IBattleshipGameAlgo& plrA, IBattleshipGameAlgo& plrB) : playerA(plrA), playerB(plrB) {};
	~BattleshipGameManager() = default;

	bool initGame(const std::string boardFilePath); //initiates board, calls players' "setBoard" methods
	void playGame(); //runs game, notifies players about move results. Continiusly checks game status and ends it when needed.
					 //Output propper messages ("Player _ won. Points: ...")

//private:
	//ENUMs
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

	//Functions
	void readBoardFileToMatrix(const std::string boardFile);//initiate game board from file.
	bool validateBoard();//check if board is valid according to game specifications
	void sendBoard(bool isPlayerA);
	void modifyBoard(char** board, bool isPlayerA);

	bool isValidShipRight(int x, int y);//check if a valid ship starts at (x,y) position to the right
	bool isValidShipBottom(int x, int y);//check if a valid ship starts at (x,y) position to the bottom
	int getSize(char type);//return ship valid size by given type
	void updateErrMsgArrWrongSize(char type);//updates wrong size error for given type in errMsgArr

	//Variables
	std::pair<bool, std::string> errMsgArr[NUM_OF_ERR_MESSAGE] = { std::make_pair(false, "Wrong size or shape for ship D for player A"),
																   std::make_pair(false, "Wrong size or shape for ship M for player A"),
																   std::make_pair(false, "Wrong size or shape for ship P for player A"),
																   std::make_pair(false, "Wrong size or shape for ship B for player A"),
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
	char typeArr[NUM_OF_SHIP_TYPES * 2] = { 'D', 'M', 'P', 'B', 'd', 'm', 'p' ,'b' };

	IBattleshipGameAlgo& playerA;
	IBattleshipGameAlgo& playerB;
	BattleBoard gameBoard;
};

#endif