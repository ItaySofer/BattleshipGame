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
	std::ifstream fin(boardFile);
	if (!fin.is_open())
	{
		std::cout << "Error while open internal file" << std::endl;
		return false;
	}

	//read dimensions
	std::string dimensions;
	std::getline(fin, dimensions);//get dimentions line (first line)
	std::vector<std::string> dimResult;//
	StringUtils::split(dimensions, "x", dimResult);
	output.setCols(std::stoi(dimResult[0]));
	output.setRows(std::stoi(dimResult[1]));
	output.setDepth(std::stoi(dimResult[2]));

	output.matrix = new std::string*[output.depth()];

	//read each layer
	for (int k = 0; k < output.depth(); k++)
	{
		output.matrix[k] = new std::string[output.rows()];
		//Read space line first
		std::getline(fin, output.matrix[k][0]);
		//Read the k'th layer matrix
		for (int i = 0; i < output.rows(); i++)
		{
			std::getline(fin, output.matrix[k][i]);
			StringUtils::replaceAll(output.matrix[k][i], "\r", "");
			//resize each matrix row to size=gameBoard.C, add ' ' if needed 
			output.matrix[k][i].resize(output.cols(), ' ');
		}
	}

	fin.close();

	//replace all unknown characters with ' '
	for (int k = 0; k < output.depth(); k++) {
		for (int i = 0; i < output.rows(); i++) {
			for (int j = 0; j < output.cols(); j++) {
				if (output.matrix[k][i][j] != ' ') {
					char* pos = std::find(std::begin(typeArr), std::end(typeArr), output.matrix[k][i][j]);
					if (pos == std::end(typeArr)) { //element not found
						output.matrix[k][i][j] = ' ';
					}
				}
			}
		}
	}
	return true;
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




