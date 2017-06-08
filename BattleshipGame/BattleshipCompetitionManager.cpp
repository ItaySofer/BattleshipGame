#include "BattleshipCompetitionManager.h"
#include "ThreadPool.h"
#include "BattleshipGameManager.h"
#include "PlayerStatus.h"
#include <iomanip>
#include <algorithm>
//#include "BattleshipGameAlgoSmart.h"


bool BattleshipCompetitionManager::initCompetition()
{
	if (!tryExtractInputFilesNames())
	{
		return false;
	}

	if (!readInputFilesToObjects())
	{
		return false;
	}

	buildCompetition();

	currRound = 1;

	return true;
}

void BattleshipCompetitionManager::startCompetition()
{

	std::cout << "Number of legal players: " << players.size() << std::endl;

	std::cout << "Number of legal boards: " << gameBoards.size() << std::endl;

	int matchesThreads = matches.size() < inputProcessor.threads ? matches.size() : inputProcessor.threads;
	ThreadPool resultsPool(1);
	ThreadPool gamesPool(matchesThreads);
	for (int i = 0; i < matches.size(); i++)
	{
		Match match = matches.at(i);
		gamesPool.enqueue([match, &resultsPool, this]()
		{
			BattleshipGameManager manager(this->gameBoards.at(match.boardIndex), this->players.at(match.playerAIndex)(), this->players.at(match.playerBIndex)());
			manager.initGame();
			MatchResult matchResult = manager.playGame();
			resultsPool.enqueue([match, matchResult, this]()
			{
				this->handleGameResult(match, matchResult);
			});
		});
	}

	gamesPool.finishCurentTasksAndStop();
	resultsPool.finishCurentTasksAndStop();
	
	printCurrentScores();
	
}

bool BattleshipCompetitionManager::tryExtractInputFilesNames() const
{
	bool boardFilesExists = inputProcessor.tryExtractBoardFileNames();

	bool dllFilesExist = inputProcessor.tryExtractDllFileNames();

	return boardFilesExists && dllFilesExist;

}

bool BattleshipCompetitionManager::readInputFilesToObjects()
{
	readPlayers();
	if (players.size() < NUM_PLAYERS)
	{
		std::cout << "Not enough leagal player" << std::endl;
		return false;
	}

	readBoards();
	if (gameBoards.size() < 1)
	{
		std::cout << "Not enough leagal boards" << std::endl;
		return false;
	}
	
	return true;

}

void BattleshipCompetitionManager::readBoards()
{
	std::vector<std::string> boardFilesPaths = inputProcessor.boardFilesPaths;
	for (int i = 0; i < boardFilesPaths.size(); i++)
	{
		BattleBoard board;
		if (readBoardFromFile(boardFilesPaths.at(i), board))
		{
			if (isValid(board))
			{
				gameBoards.push_back(board);
			}
		}
	}
}

bool BattleshipCompetitionManager::readBoardFromFile(const std::string& boardFile, BattleBoard& output)
{
	//reads board from boardFile to "output". return true\false if reading succeeded or failed.
	std::ifstream fin(boardFile);
	if (!fin.is_open())
	{
		std::cout << "Error while open internal file" << std::endl;
		return false;
	}

	//read dimensions
	std::string dimensions;
	std::getline(fin, dimensions);//get dimentions line (first line)
	std::transform(dimensions.begin(), dimensions.end(), dimensions.begin(), ::tolower);
	std::vector<std::string> dimResult;
	StringUtils::split(dimensions, "x", dimResult);
	//Check that first line contains 3 dimensions in expected format
	if (dimResult.size() != 3)
	{
		std::cout << "Error: first line of board file does not contain dimensions in the right format" << std::endl;
		return false;
	}
	output.setCols(std::stoi(dimResult[0]));
	output.setRows(std::stoi(dimResult[1]));
	output.setDepth(std::stoi(dimResult[2]));

	output.matrix = new std::string*[output.depth()]; // assumes output.matrix == nullptr 

	//Read space line first
	std::string temp;
	std::getline(fin, temp);

	//Help variables
	int remainRows = 0;
	int startIndex = 0;

	//read each layer
	for (int k = 0; k < output.depth(); k++)
	{
		output.matrix[k] = new std::string[output.rows()];
		//Read the k'th layer matrix
		for (int i = 0; i < output.rows(); i++)
		{
			std::getline(fin, output.matrix[k][i]);
			StringUtils::replaceAll(output.matrix[k][i], "\r", "");
			//check if got to empty line before output.rows() rows
			if (output.matrix[k][i].empty())
			{
				output.matrix[k][i].resize(output.cols(), ' ');
				remainRows = output.rows() - (i + 1);
				startIndex = i+1;
				break;
			}
			//resize each matrix row to size=gameBoard.C, add ' ' if needed 
			output.matrix[k][i].resize(output.cols(), ' ');
		}
		//read extra rows at the end of layer+empty line / add rows if got to empty line before reading output.rows() rows
		if ((remainRows == 0) && (startIndex == 0))//read lines until getting to empty line between layers
		{
			std::getline(fin, temp);
			StringUtils::replaceAll(temp, "\r", "");
			while (!temp.empty())
			{
				std::getline(fin, temp);
				StringUtils::replaceAll(temp, "\r", "");
			}
		}
		else //got an empty line before reading output.rows() lines - complete layer with lines of spaces
		{
			for (int j = 0; j < remainRows; j++)
			{
				output.matrix[k][startIndex + j] = " ";
				output.matrix[k][startIndex + j].resize(output.cols(), ' ');
			}
			remainRows = 0;
			startIndex = 0;
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

bool BattleshipCompetitionManager::isValid(const BattleBoard& board)
{
	int Acount = 0;
	int Bcount = 0;

	//Search for valid ships
	for (int k = 0; k < board.depth(); k++) {
		for (int i = 0; i < board.rows(); i++) {
			for (int j = 0; j < board.cols(); j++) {
				char* pos = std::find(std::begin(typeArr), std::end(typeArr), board.matrix[k][i][j]);
				if (pos != std::end(typeArr)) { //element found
					bool isValidRight = true;
					bool isValidDown = true;
					bool isValidUnder = true;
					//check if need to check ship to the right
					if ((j == 0) || (board.matrix[k][i][j - 1] != board.matrix[k][i][j])) {
						//Check to the right
						isValidRight = isValidShipRight(board, k, i, j);
						if (isValidRight) {
							(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
						}
					}
					//check if need to check ship down
					if (!isValidRight) {
						if ((i == 0) || (board.matrix[k][i - 1][j] != board.matrix[k][i][j])) {
							//Check down
							isValidDown = isValidShipDown(board, k, i, j);
							if (isValidDown) {
								(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
							}
						}
					}
					//check if need to check ship under
					if (!isValidRight && !isValidDown) {
						if ((k == 0) || (board.matrix[k - 1][i][j] != board.matrix[k][i][j])) {
							//Check to the bottom
							isValidUnder = isValidShipUnder(board, k, i, j);
							if (isValidUnder) {
								(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
							}
						}
					}
					if (!isValidRight && !isValidDown && !isValidUnder) {
						updateErrMsgArrWrongSize(board.matrix[k][i][j]);
					}
				}
			}
		}
	}

	//Check for adjacent
	bool brk = false;
	for (int k = 0; k < board.depth(); k++) {
		for (int i = 0; i < board.rows(); i++) {
			for (int j = 0; j < board.cols(); j++) {
				if (board.matrix[k][i][j] != ' ') {
					//Check different Down
					if ((i != board.rows() - 1) &&
						(board.matrix[k][i + 1][j] != board.matrix[k][i][j]) && (board.matrix[k][i + 1][j] != ' ')) {
						errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
						brk = true;
						break;
					}
					//Check different to the right
					if ((j != board.cols() - 1) &&
						(board.matrix[k][i][j + 1] != board.matrix[k][i][j]) && (board.matrix[k][i][j + 1] != ' ')) {
						errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
						brk = true;
						break;
					}
					//Check different under
					if ((k != board.depth() - 1) &&
						(board.matrix[k + 1][i][j] != board.matrix[k][i][j]) && (board.matrix[k + 1][i][j] != ' ')) {
						errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
						brk = true;
						break;
					}
				}
			}
			if (brk) break;
		}
		if (brk) break;
	}

	//Check for error messages - print error message if found
	bool valid = true;
	for (int i = 0; i < int(ErrorMsg::ERR_MGS_MAX); i++) {
		if (errMsgArr[i].first == true) {
			errMsgArr[i].first = false; //reset errorMsgArr for next boards
			valid = false;
		}
	}
	 
	return (valid);
}

bool BattleshipCompetitionManager::isValidShipRight(const BattleBoard& board, int d, int x, int y) {
	char type = board.matrix[d][x][y];
	int size = 0;

	while ((y < board.cols()) && (board.matrix[d][x][y] == type)) {
		size++;
		//if same type up/down - return false
		if (((x != 0) && (board.matrix[d][x - 1][y] == type)) ||
			((x != board.rows() - 1) && (board.matrix[d][x + 1][y] == type))) {
			return false;
		}
		//if same type above/under - return false
		if (((d != 0) && (board.matrix[d - 1][x][y] == type)) ||
			((d != board.depth() - 1) && (board.matrix[d + 1][x][y] == type))) {
			return false;
		}
		y++;
	}
	return (size == getSize(type));
}

bool BattleshipCompetitionManager::isValidShipDown(const BattleBoard& board, int d, int x, int y) {
	char type = board.matrix[d][x][y];
	int size = 0;

	while ((x < board.rows()) && (board.matrix[d][x][y] == type)) {
		size++;
		//if same type above/under - return false
		if (((d != 0) && (board.matrix[d - 1][x][y] == type)) ||
			((d != board.depth() - 1) && (board.matrix[d + 1][x][y] == type))) {
			return false;
		}
		//if same type left/right - return false
		if (((y != 0) && (board.matrix[d][x][y - 1] == type)) ||
			((y != board.cols() - 1) && (board.matrix[d][x][y + 1] == type))) {
			return false;
		}
		x++;
	}
	return (size == getSize(type));
}

bool BattleshipCompetitionManager::isValidShipUnder(const BattleBoard& board, int d, int x, int y) {
	char type = board.matrix[d][x][y];
	int size = 0;

	while ((d < board.depth()) && (board.matrix[d][x][y] == type)) {
		size++;
		//if same type left/right - return false
		if (((y != 0) && (board.matrix[d][x][y - 1] == type)) ||
			((y != board.cols() - 1) && (board.matrix[d][x][y + 1] == type))) {
			return false;
		}
		//if same type up/down - return false
		if (((x != 0) && (board.matrix[d][x - 1][y] == type)) ||
			((x != board.rows() - 1) && (board.matrix[d][x + 1][y] == type))) {
			return false;
		}
		d++;
	}
	return (size == getSize(type));
}

int BattleshipCompetitionManager::getSize(char type) {

	switch (type) {
	case 'D': {
		return int(shipSize::dSize);
		break;
	}
	case 'M': {
		return int(shipSize::mSize);
		break;
	}
	case 'P': {
		return int(shipSize::pSize);
		break;
	}
	case 'B': {
		return int(shipSize::bSize);
		break;
	}
	case 'd': {
		return int(shipSize::dSize);
		break;
	}
	case 'm': {
		return int(shipSize::mSize);
		break;
	}
	case 'p': {
		return int(shipSize::pSize);
		break;
	}
	case 'b': {
		return int(shipSize::bSize);
		break;
	}
	default: {
		return 0;
		break;
	}
	}
}

void BattleshipCompetitionManager::updateErrMsgArrWrongSize(char type) {
	switch (type) {
	case 'D': {
		errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_D_A)].first = true;
		break;
	}
	case 'M': {
		errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_M_A)].first = true;
		break;
	}
	case 'P': {
		errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_P_A)].first = true;
		break;
	}
	case 'B': {
		errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_B_A)].first = true;
		break;
	}
	case 'd': {
		errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_D_B)].first = true;
		break;
	}
	case 'm': {
		errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_M_B)].first = true;
		break;
	}
	case 'p': {
		errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_P_B)].first = true;
		break;
	}
	case 'b': {
		errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_B_B)].first = true;
		break;
	}
	default: {
		break;
	}
	}
}

void BattleshipCompetitionManager::readPlayers()
{
	//reads all dll into players vector (as unique_ptrs)

	for (int currDllPath = 0; currDllPath < inputProcessor.dllFilesPaths.size(); currDllPath++) {
		// Load dynamic library
		HINSTANCE hDll = LoadLibraryA(inputProcessor.dllFilesPaths.at(currDllPath).c_str());
		if (!hDll) {
			continue;
		}

		hInstances.push_back(hDll);

		// Get function pointer
		GetAlgorithmFuncType initPlayerFunc = reinterpret_cast<GetAlgorithmFuncType>(GetProcAddress(hDll, "GetAlgorithm"));
		if (!initPlayerFunc) {
			continue;
		}

		players.push_back(initPlayerFunc);

		std::string currPlayerDllPath = inputProcessor.dllFilesPaths.at(currDllPath);
		std::string currPlayerName = dllPathToPlayerName(currPlayerDllPath);
		playersStatus.push_back(PlayerStatus(currPlayerName));
		if (currPlayerName.length() > maxLengthName)
		{
			maxLengthName = currPlayerName.length();
		}
	}
}

std::string BattleshipCompetitionManager::dllPathToPlayerName(const std::string& dllPath)
{
	std::vector<std::string> words;
	StringUtils::split(dllPath, ".", words);

	if (words.size() >= 3)
	{
		return words.at(words.size() - 3);
	} else
	{
		return "MisformattedDllName";
	}

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

std::vector<std::pair<int, int>> BattleshipCompetitionManager::computePairsForBoard() const
{

	std::vector<std::pair<int, int>> pairs = getPairsOneOrder();

	addPairsOtherOrder(pairs);

	return pairs;
}

std::vector<std::pair<int, int>> BattleshipCompetitionManager::getPairsOneOrder() const
{
	std::vector<int> playersNum;
	size_t size = players.size();
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
	for (size_t i = vec.size() - 1; i > firstIndex; i--)
	{
		vec.at(i) = vec.at(i - 1);
	}
	vec.at(firstIndex) = last;
}

void BattleshipCompetitionManager::addPairsOtherOrder(std::vector<std::pair<int, int>>& pairs)
{
	size_t firstHalfSize = pairs.size();
	for (int i = 0; i < firstHalfSize; i++)
	{
		std::pair<int, int> pair = pairs.at(i);
		pairs.push_back(std::make_pair(pair.second, pair.first));
	}
}

void BattleshipCompetitionManager::handleGameResult(Match match, MatchResult matchResult)
{
	playersStatus.at(match.playerAIndex).round++;
	playersStatus.at(match.playerBIndex).round++;

	if (matchResult.whoWon == 'A')
	{
		playersStatus.at(match.playerAIndex).wins++;
		playersStatus.at(match.playerBIndex).loses++;
	}
	else if (matchResult.whoWon == 'B') 
	{
		playersStatus.at(match.playerBIndex).wins++;
		playersStatus.at(match.playerAIndex).loses++;
	}

	playersStatus.at(match.playerAIndex).percent = double(playersStatus.at(match.playerAIndex).wins) / (playersStatus.at(match.playerAIndex).wins + playersStatus.at(match.playerAIndex).loses) * 100;
	playersStatus.at(match.playerBIndex).percent = double(playersStatus.at(match.playerBIndex).wins) / (playersStatus.at(match.playerBIndex).wins + playersStatus.at(match.playerBIndex).loses) * 100;

	playersStatus.at(match.playerAIndex).pointsFor += matchResult.playerAScore;
	playersStatus.at(match.playerBIndex).pointsFor += matchResult.playerBScore;

	playersStatus.at(match.playerAIndex).pointsAgainst += matchResult.playerBScore;
	playersStatus.at(match.playerBIndex).pointsAgainst += matchResult.playerAScore;

	if (allPlayersPlayedInCurrentRound())
	{
		printCurrentScores();
		currRound++;
	}
}

bool BattleshipCompetitionManager::allPlayersPlayedInCurrentRound()
{
	for (int i = 0; i < playersStatus.size(); i++)
	{
		if (playersStatus.at(i).round < currRound)
		{
			return false;
		}
	}

	return true;
}

void BattleshipCompetitionManager::printCurrentScores()
{
	std::vector<PlayerStatus> sorted = playersStatus;
	std::sort(sorted.begin(), sorted.end(), [](PlayerStatus& first, PlayerStatus& second) {return first.percent > second.percent; });

	std::string teamName = std::string("Team Name");
	int nameWidth = maxLengthName > teamName.length() ? maxLengthName + 5 : teamName.length() + 5;

	std::cout << std::endl << std::endl;
	std::cout << std::left << std::setw(8) << "#" << std::setw(nameWidth) << teamName << std::setw(8) << "Wins" << std::setw(8) << "Losses" << std::setw(8) << "%" << std::setw(8) << "Pts For" << std::setw(12) << "Pts Against" << std::endl << std::endl;

	for (int i = 0; i < sorted.size(); i++)
	{
		PlayerStatus currPlayer = sorted.at(i);
		std::string index = std::to_string(i + 1) + ".";
		std::cout << std::left << std::setw(8) << index << std::setw(nameWidth) << currPlayer.name << std::setw(8) << currPlayer.wins << std::setw(8) << currPlayer.loses << std::setw(8) << std::setprecision(4) << currPlayer.percent << std::setw(8) << currPlayer.pointsFor << std::setw(12) << currPlayer.pointsAgainst << std::endl;
	}
}




