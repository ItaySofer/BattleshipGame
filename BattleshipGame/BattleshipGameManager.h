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

class BattleshipGameManager {

public:
	BattleshipGameManager(InputProcessor& inputProc) : inputProcessor(inputProc), playerA(nullptr), playerB(nullptr) {}
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
	void playGame(); //runs game, notifies players about move results. Continiusly checks game status and ends it when needed.
					 //Output propper messages ("Player _ won. Points: ...")

	friend class BattleshipGameManagerTest;

private:

	//Functions
	void initPlayers();
	//bool readBoardFileToMatrix(const std::string& boardFile);//initiate game board from file.
	//bool validateBoard();//check if board is valid according to game specifications
	void sendBoard(bool isPlayerA);
	void modifyBoard(char*** board, bool isPlayerA);

	//bool isValidShipRight(int d, int x, int y) const;//check if a valid ship starts at depth d at (x,y) position to the right
	//bool isValidShipDown(int d, int x, int y) const;//check if a valid ship starts at depth d at (x,y) position to the bottom
	//bool isValidShipUnder(int d, int x, int y) const;//check if a valid ship starts at depth d at (x,y) position to the deaper layers
	//static int getSize(char type);//return ship valid size by given type
	//void updateErrMsgArrWrongSize(char type);//updates wrong size error for given type in errMsgArr

	int getSinkScoreByChar(char c);
	bool isActivePlayer(int playerIndex) const;
	static bool isLonely(const BattleBoard& gameBoard, int row, int col);
	int handleMove(int currPlayer, BattleBoard& gameBoard, int row, int col);
	int numActivePlayers() const;
	void graphicPrintBoard(const BattleBoard& gameBoard)const;
	static void gotoxy(int x, int y);
	static void setTextColor(int color);
	static void ShowConsoleCursor(bool showFlag);

	void goSetPrintSleep(int row, int col, int color, char output, int player) const;

	//Variables

	char typeArr[NUM_OF_SHIP_TYPES * 2] = { 'D', 'M', 'P', 'B', 'd', 'm', 'p' ,'b' };

	int sinkScoreArr[NUM_OF_SHIP_TYPES] = { 8,7,3,2 };

	InputProcessor& inputProcessor;
	BattleBoard gameBoard;
	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;
	int numShips[NUM_PLAYERS] = { VALID_SHIP_NUM , VALID_SHIP_NUM };
	int scores[NUM_PLAYERS] = {0, 0};
	HINSTANCE hInstances[NUM_PLAYERS] = { NULL, NULL };
};

#endif