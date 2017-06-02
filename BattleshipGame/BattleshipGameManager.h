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
		
//		// Free dynamic libs //TODO: move to the right place in code
//		for (int i = 0; i < NUM_PLAYERS; i++) {
//			FreeLibrary(hInstances[i]);
//		}
	};

	BattleshipGameManager(const BattleshipGameManager&) = delete;
	BattleshipGameManager& operator=(const BattleshipGameManager&) = delete;

	void initGame(); //initiates board, calls players' "setBoard" methods
	MatchResult playGame(); //runs game, notifies players about move results. Continiusly checks game status and ends it when needed.
					 //Output propper messages ("Player _ won. Points: ...")

	friend class BattleshipGameManagerTest;

private:

	//Functions
	void initPlayers();
	void sendBoard(bool isPlayerA);
	void modifyBoard(char*** board, bool isPlayerA);

	int getSinkScoreByChar(char c);
	bool isActivePlayer(int playerIndex) const;
	static bool isLonely(const BattleBoard& gameBoard, Coordinate coor);
	int handleMove(int currPlayer, BattleBoard& gameBoard, Coordinate coor);
	int numActivePlayers() const;

	//Variables

	char typeArr[NUM_OF_SHIP_TYPES * 2] = { 'D', 'M', 'P', 'B', 'd', 'm', 'p' ,'b' };

	int sinkScoreArr[NUM_OF_SHIP_TYPES] = { 8,7,3,2 };

	BattleBoard gameBoard;
	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;
	int numShips[NUM_PLAYERS] = { VALID_SHIP_NUM , VALID_SHIP_NUM };
	int scores[NUM_PLAYERS] = {0, 0};
};

#endif