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
#include <memory>

class BattleshipGameManager {

public:
	BattleshipGameManager(const BattleBoard& board, IBattleshipGameAlgo* playerA, IBattleshipGameAlgo* playerB) : gameBoard(board), playerABoard(board), playerBBoard(board), playerA(playerA), playerB(playerB)
	{
		modifyBoard(playerABoard, true);
		modifyBoard(playerBBoard, false);
	}

	~BattleshipGameManager() = default;

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
	void modifyBoard(BattleBoard& board, bool isPlayerA);

	int getSinkScoreByChar(char c);
	bool isActivePlayer(int playerIndex) const;
	static bool isLonely(const BattleBoard& gameBoard, const Coordinate& coor);
	int handleMove(int currPlayer, BattleBoard& gameBoard, const Coordinate& coor);
	int numActivePlayers() const;

	//Variables

	char typeArr[NUM_OF_SHIP_TYPES * 2] = { 'D', 'M', 'P', 'B', 'd', 'm', 'p' ,'b' };

	int sinkScoreArr[NUM_OF_SHIP_TYPES] = { 8,7,3,2 };

	BattleBoard gameBoard;
	BattleBoard playerABoard;
	BattleBoard playerBBoard;
	std::unique_ptr<IBattleshipGameAlgo> playerA;
	std::unique_ptr<IBattleshipGameAlgo> playerB;
	int numShips[NUM_PLAYERS] = { VALID_SHIP_NUM , VALID_SHIP_NUM };
	int scores[NUM_PLAYERS] = {0, 0};
};

#endif