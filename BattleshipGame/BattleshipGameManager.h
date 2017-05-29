#ifndef BattleshipGameManagerH
#define BattleshipGameManagerH

#include "IBattleshipGameAlgo.h"
#include "BattleBoard.h"
#include "StringUtils.h"
#include "Macros.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include "InputProcessor.h"
#include <windows.h>
#include <stdlib.h>
#include "MatchResult.h"

class BattleshipGameManager {

public:
	BattleshipGameManager(BattleBoard board, IBattleshipGameAlgo* playerA, IBattleshipGameAlgo* playerB) : gameBoard(board), playerA(playerA), playerB(playerB) {}
	~BattleshipGameManager() {
		delete playerA;
		delete playerB;
		
		// Free dynamic libs
		for (int i = 0; i < NUM_PLAYERS; i++) {
			FreeLibrary(hInstances[i]);
		}
	};

	BattleshipGameManager(const BattleshipGameManager&) = delete;
	BattleshipGameManager& operator=(const BattleshipGameManager&) = delete;

	void initGame(); //initiates board, calls players' "setBoard" methods
	MatchResult playGame(); //runs game, notifies players about move results. Continiusly checks game status and ends it when needed.
					 //Output propper messages ("Player _ won. Points: ...")

	friend class BattleshipGameManagerTest;

private:
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
	void initPlayers();
	bool readBoardFileToMatrix(const std::string& boardFile);//initiate game board from file.
	bool validateBoard();//check if board is valid according to game specifications
	void sendBoard(bool isPlayerA);
	void modifyBoard(char** board, bool isPlayerA);

	bool isValidShipRight(int x, int y) const;//check if a valid ship starts at (x,y) position to the right
	bool isValidShipBottom(int x, int y) const;//check if a valid ship starts at (x,y) position to the bottom
	static int getSize(char type);//return ship valid size by given type
	void updateErrMsgArrWrongSize(char type);//updates wrong size error for given type in errMsgArr

	int getSinkScoreByChar(char c);
	bool isActivePlayer(int playerIndex) const;
	static bool isLonely(const BattleBoard& gameBoard, int row, int col);
	int handleMove(int currPlayer, BattleBoard& gameBoard, int row, int col);
	int numActivePlayers() const;

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

	int sinkScoreArr[NUM_OF_SHIP_TYPES] = { 8,7,3,2 };

	BattleBoard gameBoard;
	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;
	int numShips[NUM_PLAYERS] = { VALID_SHIP_NUM , VALID_SHIP_NUM };
	int scores[NUM_PLAYERS] = {0, 0};
	HINSTANCE hInstances[NUM_PLAYERS] = { NULL, NULL };
};

#endif