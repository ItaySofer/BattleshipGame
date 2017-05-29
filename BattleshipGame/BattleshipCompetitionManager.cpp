#include "BattleshipCompetitionManager.h"
#include "ThreadPool.h"
#include "BattleshipGameManager.h"


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

	if (gameBoards.size() == 0 || players.size() < NUM_PLAYERS)
	{
		return false;
	}

	buildCompetition();

	initRoundsTracking();

	return true;
}

void BattleshipCompetitionManager::startCompetition()
{
	ThreadPool resultsPool(1);
	ThreadPool gamesPool(inputProcessor.threads);
	for (int i = 0; i < matches.size(); i++)
	{
		Match match = matches.at(i);
		gamesPool.enqueue([match, &resultsPool, this]()
		{
			BattleshipGameManager manager(this->gameBoards.at(match.boardIndex), this->players.at(match.playerAIndex), this->players.at(match.playerBIndex));//TODO: init game manager with correct types of board and players
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

	//Check valid number of ships
	if (Acount < VALID_SHIP_NUM) {
		errMsgArr[int(ErrorMsg::TOO_FEW_A)].first = true;
	}
	else if (Acount > VALID_SHIP_NUM) {
		errMsgArr[int(ErrorMsg::TOO_MANY_A)].first = true;
	}
	if (Bcount < VALID_SHIP_NUM) {
		errMsgArr[int(ErrorMsg::TOO_FEW_B)].first = true;
	}
	else if (Bcount > VALID_SHIP_NUM) {
		errMsgArr[int(ErrorMsg::TOO_MANY_B)].first = true;
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
			////Print the relevant error message
			//std::cout << errMsgArr[i].second << std::endl;
			valid = false;
		}
	}
	return (valid);
}

bool BattleshipCompetitionManager::isValidShipRight(const BattleBoard& board, int d, int x, int y) const {
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

bool BattleshipCompetitionManager::isValidShipDown(const BattleBoard& board, int d, int x, int y) const {
	char type = board.matrix[d][x][y];
	int size = 0;

	while ((x < board.rows()) && (board.matrix[d][x][y] == type)) {
		size++;
		//if same type up/down - return false
		if (((x != 0) && (board.matrix[d][x - 1][y] == type)) ||
			((x != board.rows() - 1) && (board.matrix[d][x + 1][y] == type))) {
			return false;
		}
		//if same type left/right - return false
		if (((y != 0) && (board.matrix[d][x][y - 1] == type)) ||
			((y != board.cols() - 1) && (board.matrix[d][x][y + 1] == type))) {
			return false;
		}
		d++;
	}
	return (size == getSize(type));
}

bool BattleshipCompetitionManager::isValidShipUnder(const BattleBoard& board, int d, int x, int y) const {
	char type = board.matrix[d][x][y];
	int size = 0;

	while ((d < board.depth()) && (board.matrix[d][x][y] == type)) {
		size++;
		//if same type left/right - return false
		if (((y != 0) && (board.matrix[d][x][y - 1] == type)) ||
			((y != board.cols() - 1) && (board.matrix[d][x][y + 1] == type))) {
			return false;
		}
		//if same type above/under - return false
		if (((d != 0) && (board.matrix[d - 1][x][y] == type)) ||
			((d != board.depth() - 1) && (board.matrix[d + 1][x][y] == type))) {
			return false;
		}
		x++;
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

void BattleshipCompetitionManager::initRoundsTracking()
{
	currRound = 1;
	for (int i = 0; i < players.size(); i++)
	{
		playerRounds.push_back(0);
	}
}

void BattleshipCompetitionManager::handleGameResult(Match match, MatchResult matchResult)
{
	playerRounds.at(match.playerAIndex)++;
	playerRounds.at(match.playerBIndex)++;
	//TODO: save players scores
	if (allPlayersPlayedInCurrentRound())
	{
		printCurrentScores();
		currRound++;
	}
}

bool BattleshipCompetitionManager::allPlayersPlayedInCurrentRound()
{
	for (int i = 0; i < playerRounds.size(); i++)
	{
		if (playerRounds.at(i) < currRound)
		{
			return false;
		}
	}

	return true;
}

void BattleshipCompetitionManager::printCurrentScores()
{
	//TODO: print players scores
}




