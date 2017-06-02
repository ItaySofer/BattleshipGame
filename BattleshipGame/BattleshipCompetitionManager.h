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
#include "MatchResult.h"
#include "PlayerStatus.h"

typedef IBattleshipGameAlgo*(*GetAlgorithmFuncType)();

class BattleshipCompetitionManager {

public:
	BattleshipCompetitionManager(InputProcessor& inputProc) : inputProcessor(inputProc), currRound(0) {}
	~BattleshipCompetitionManager()
	{
		//TODO: release all memory if needed
	}

	BattleshipCompetitionManager(const BattleshipCompetitionManager&) = delete;
	BattleshipCompetitionManager& operator=(const BattleshipCompetitionManager&) = delete;

	bool initCompetition();
	void startCompetition(); 

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

	bool tryExtractInputFilesNames() const;
	bool readInputFilesToObjects();
	void readBoards();
	bool readBoardFromFile(const std::string& boardFile, BattleBoard& output);

	bool isValid(const BattleBoard& board);
	static bool isValidShipRight(const BattleBoard& board, int d, int x, int y);
	static bool isValidShipDown(const BattleBoard& board, int d, int x, int y);
	static bool isValidShipUnder(const BattleBoard& board, int d, int x, int y);
	static int getSize(char type);//return ship valid size by given type
	void updateErrMsgArrWrongSize(char type);//updates wrong size error for given type in errMsgArr

	void readPlayers();
	static std::string dllPathToPlayerName(const std::string& dllPath);
	void buildCompetition();
	std::vector<std::pair<int, int>> computePairsForBoard();
	std::vector<std::pair<int, int>> getPairsOneOrder();
	static void barrleshiftClockwise(std::vector<int>& vec, bool firstFixed);
	static void addPairsOtherOrder(std::vector<std::pair<int, int>>& pairs);
	void handleGameResult(Match match, MatchResult matchResult);
	bool allPlayersPlayedInCurrentRound();
	void printCurrentScores();

	InputProcessor& inputProcessor;
	std::vector<BattleBoard> gameBoards;
	std::vector<GetAlgorithmFuncType> players;
	std::vector<Match> matches;
	std::vector<PlayerStatus> playersStatus;
	int currRound;

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
};

#endif