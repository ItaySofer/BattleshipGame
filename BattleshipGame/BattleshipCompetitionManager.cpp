#include "BattleshipCompetitionManager.h"

bool BattleshipCompetitionManager::initCompetition()
{
	bool boardFilesExists = inputProcessor.tryExtractBoardFileNames();

	bool dllFilesExist = inputProcessor.tryExtractDllFileNames();

	if (!boardFilesExists || !dllFilesExist)
	{
		return false;
	}

	readBoards();
	std::cout << "Number of legal boards: " << gameBoards.size() << std::endl;

	readPlayers();
	std::cout << "Number of legal players: " << players.size() << std::endl;

	if (gameBoards.size() == 0 || players.size() < 2)
	{
		return false;
	}

	buildCompetition();

	return true;
}

void BattleshipCompetitionManager::startCompetition()
{
}

void BattleshipCompetitionManager::readBoards()
{
	std::vector<std::string> boardFilesPaths = inputProcessor.boardFilesPaths;
	for (int i = 0; i < boardFilesPaths.size(); i++)
	{
		BattleBoard board;
		if (readBoardFromFile(boardFilesPaths.at(i), board) && isValid(board))
		{
			gameBoards.push_back(board);
		}
	}
}

bool BattleshipCompetitionManager::readBoardFromFile(const std::string& boardFile, BattleBoard& output)
{
	//reads board from boardFile to output. true\ false if reading succeeded or failed.
}

bool BattleshipCompetitionManager::isValid(BattleBoard& board)
{
	//return if board is valid
}

void BattleshipCompetitionManager::readPlayers()
{
	//reads all dll into players vector (as unique_ptrs)
}

void BattleshipCompetitionManager::buildCompetition()
{
	std::vector<std::pair<int, int>> pairs = computePairsForBoard();
	for (int i = 0; i < gameBoards.size(); i++)
	{
		for (int j = 0; j < pairs.size(); j++)
		{
			std::pair<int, int> pair = pairs.at(j);
			matches.push_back(Match(i, pair.first, pair.second));
		}
	}
}

std::vector<std::pair<int, int>> BattleshipCompetitionManager::computePairsForBoard()
{

	std::vector<std::pair<int, int>> pairs = getPairsOneOrder();

	addPairsOtherOrder(pairs);

	return pairs;
}

std::vector<std::pair<int, int>> BattleshipCompetitionManager::getPairsOneOrder()
{
	std::vector<int> playersNum;
	int size = players.size();
	for (int i = 0; i < size; i++)
	{
		playersNum.push_back(i);
	}

	std::vector<std::pair<int, int>> pairs;
	while (playersNum.at(1) != 1 || pairs.size() == 0)
	{
		int curr = 0;
		while (curr < size / 2)
		{
			pairs.push_back(std::make_pair(playersNum.at(curr), playersNum.at(size - 1 - curr)));
			curr++;
		}
		barrleshiftClockwise(playersNum, size % 2 == 0);
	}

	return pairs;
}

void BattleshipCompetitionManager::barrleshiftClockwise(std::vector<int>& vec, bool firstFixed)
{
	int firstIndex = firstFixed ? 1 : 0;
	int last = vec.at(vec.size() - 1);
	for (int i = vec.size() - 1; i > firstIndex; i--)
	{
		vec.at(i) = vec.at(i - 1);
	}
	vec.at(firstIndex) = last;
}

void BattleshipCompetitionManager::addPairsOtherOrder(std::vector<std::pair<int, int>>& pairs)
{
	int firstHalfSize = pairs.size();
	for (int i = 0; i < firstHalfSize; i++)
	{
		std::pair<int, int> pair = pairs.at(i);
		pairs.push_back(std::make_pair(pair.second, pair.first));
	}
}




