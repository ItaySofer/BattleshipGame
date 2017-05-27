#ifndef BattleshipCompetitionManagerH
#define BattleshipCompetitionManagerH

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
#include <memory>
#include "Match.h"

class BattleshipCompetitionManager {

public:
	BattleshipCompetitionManager(InputProcessor& inputProc) : inputProcessor(inputProc) {}
	~BattleshipCompetitionManager() {}

	BattleshipCompetitionManager(const BattleshipCompetitionManager&) = delete;
	BattleshipCompetitionManager& operator=(const BattleshipCompetitionManager&) = delete;

	bool initCompetition();
	void startCompetition(); 

private:

	void readBoards();
	bool readBoardFromFile(const std::string& boardFile, BattleBoard& output);
	bool isValid(BattleBoard& board);
	void readPlayers();
	void buildCompetition();
	std::vector<std::pair<int, int>> computePairsForBoard();
	std::vector<std::pair<int, int>> getPairsOneOrder();
	void barrleshiftClockwise(std::vector<int>& vec, bool firstFixed);
	void addPairsOtherOrder(std::vector<std::pair<int, int>>& pairs);


	InputProcessor& inputProcessor;
	std::vector<BattleBoard> gameBoards;
	std::vector<IBattleshipGameAlgo*> players;
	std::vector<Match> matches;

};

#endif