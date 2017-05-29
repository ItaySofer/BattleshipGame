#include <iostream>
#include <string>
#include "BattleshipGameManager.h"
#include "MatchResult.h"

/*#include "BattleshipGameAlgoFromFile.h"
#include "BattleshipGameAlgoNaive.h"
#include "BattleshipGameAlgoSmart.h"*/

void BattleshipGameManager::initGame(){
	
	initPlayers();
};

void BattleshipGameManager::initPlayers()
{
//	HANDLE dir;
//	WIN32_FIND_DATAA fileData;
//	typedef IBattleshipGameAlgo*(*GetAlgorithmFuncType)();
//	std::string path = inputProcessor.getFolderPath();
//	std::string s = "\\*.dll";
//	int dllCounter = 0;
//	GetAlgorithmFuncType dllArr[NUM_PLAYERS] = { NULL, NULL };
//	dir = FindFirstFileA((path + s).c_str(), &fileData);
//	do {
//		if (dir != INVALID_HANDLE_VALUE) {
//			std::string fileName = fileData.cFileName;
//			std::string fullFileName = path + "\\" + fileName;
//
//			// Load dynamic library
//			HINSTANCE hDll = LoadLibraryA(fullFileName.c_str());
//			if (!hDll) {
//				std::cout << "1 - Cannot load dll: " << fullFileName << std::endl;
//				return false;
//			}
//			// Get function pointer
//			dllArr[dllCounter] = (GetAlgorithmFuncType)GetProcAddress(hDll, "GetAlgorithm");
//			hInstances[dllCounter] = hDll;
//			if (!dllArr[dllCounter++])
//			{
//				std::cout << "2 - Cannot load dll: " << fullFileName << std::endl;
//				return false;
//			}
//		}
//	} while (dllCounter < NUM_PLAYERS && FindNextFileA(dir, &fileData));
//	if (dllCounter < NUM_PLAYERS) {
//		std::cout << "Missing an algorithm (dll) file looking in path: " << path << std::endl;
//		return false;
//	}
//	playerA = dllArr[0]();
//	playerB = dllArr[1]();
	
	//Send each player his board
	sendBoard(true);//player A

	sendBoard(false);//player B	
	
}

void BattleshipGameManager::sendBoard(bool isPlayerA){
	 char** board = new char*[NUM_ROWS];
	for (int i = 0; i < gameBoard.R; i++)
	{
		board[i] = new char[NUM_COLS];
		std::memcpy(board[i], gameBoard.matrix[i].c_str(), NUM_COLS);
	}
	modifyBoard(board, isPlayerA);
	IBattleshipGameAlgo* player = isPlayerA ? playerA : playerB;
	int playerNum = isPlayerA ? 0 : 1;
	player->setBoard(playerNum, const_cast<const char**>(board), gameBoard.R, gameBoard.C);

	//Delete board
	for (int i = 0; i < gameBoard.R; i++)
	{
		delete[] board[i];
	}
	delete[] board;
}

void BattleshipGameManager::modifyBoard(char** board, bool isPlayerA){
	for (int i = 0; i < gameBoard.R; i++){
		for (int j = 0; j < gameBoard.R; j++){
			if (board[i][j] != ' '){
				char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);
				if (((std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) && !isPlayerA) ||
					((std::distance(typeArr, pos) >= NUM_OF_SHIP_TYPES) && isPlayerA)){
					board[i][j] = ' ';
				}
			}
		}
	}
}

MatchResult BattleshipGameManager::playGame() {
	int currPlayer = STARTING_PLAYER;
	int doneAttackingPlayers = 0;
	while (numActivePlayers() > 1 && doneAttackingPlayers < NUM_PLAYERS) {
		std::pair<int, int> currAttack = (currPlayer % 2 == 0) ? playerA->attack() : playerB->attack();
		
		if (currAttack.first == -1 && currAttack.second == -1) {
			// currPlayer is done attacking
			doneAttackingPlayers += 1;
			currPlayer += 1;
			continue;
		}
		currAttack.first--;
		currAttack.second--;
		if (currAttack.first < 0 || currAttack.second < 0 || currAttack.first >= NUM_ROWS || currAttack.second >= NUM_COLS) {
			// illegal attack
			continue;
		}
		int roundResult = handleMove(currPlayer, gameBoard, currAttack.first, currAttack.second);
		if (std::abs(roundResult) > 1) {
			// currPlayer sink
			int rowIndexDown = currAttack.first + 1, rowIndexUp = currAttack.first - 1, colIndexLeft = currAttack.second - 1, colIndexRight = currAttack.second + 1;
			while (rowIndexDown < NUM_ROWS && gameBoard.matrix[rowIndexDown][currAttack.second] != ' ') {
				if (gameBoard.matrix[rowIndexDown][currAttack.second] != '*') {
					break;
				}
				rowIndexDown++;
			}
			while (rowIndexUp >= 0 && gameBoard.matrix[rowIndexUp][currAttack.second] != ' ') {
				if (gameBoard.matrix[rowIndexUp][currAttack.second] != '*') {
					break;
				}
				rowIndexUp--;
			}
			while (colIndexLeft >= 0 && gameBoard.matrix[currAttack.first][colIndexLeft] != ' ') {
				if (gameBoard.matrix[currAttack.first][colIndexLeft] != '*') {
					break;
				}
				colIndexLeft--;
			}
			while (colIndexRight < NUM_COLS && gameBoard.matrix[currAttack.first][colIndexRight] != ' ') {
				if (gameBoard.matrix[currAttack.first][colIndexRight] != '*') {
					break;
				}
				colIndexRight++;
			}
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Sink);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Sink);
			currPlayer += (roundResult > 0 || doneAttackingPlayers > 0) ? 0 : 1;
		}
		else if (std::abs(roundResult) == 1) {
			// currPlayer hit
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Hit);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Hit);
			currPlayer += (roundResult > 0 || doneAttackingPlayers > 0) ? 0 : 1;
		}
		else if (roundResult == 0) {
			// currPlayer miss
			if (gameBoard.matrix[currAttack.first][currAttack.second] == ' ') {
			}
			else if (gameBoard.matrix[currAttack.first][currAttack.second] == '*') {
				if (isLonely(gameBoard, currAttack.first, currAttack.second)) {
				}
				else {
				}
			}
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Miss);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Miss);
			currPlayer += doneAttackingPlayers > 0 ? 0 : 1;
		}
	}
	system("cls");
	// game ended, announce winner if exists and scores
	MatchResult res;
	if (isActivePlayer(0) && !isActivePlayer(1)) {
		res.whoWon = 'A';
	}
	else if (isActivePlayer(1) && !isActivePlayer(0)) {
		res.whoWon = 'B';
	}

	res.playerAScore = scores[0];
	res.playerBScore = scores[1];

	// free dynamic libs
	/*for (int i = 0; i < NUM_PLAYERS; i++) {
		FreeLibrary(hInstances[i]);
	}*/

	return res;
	
};

bool BattleshipGameManager::readBoardFileToMatrix(const std::string& boardFile){

	std::ifstream fin(boardFile);
	if (!fin.is_open())
	{
		std::cout << "Error while open internal file" << std::endl;
		return false;
	}

	gameBoard.R = NUM_ROWS;
	gameBoard.C = NUM_COLS;
	gameBoard.matrix = new std::string[gameBoard.R];
	for (int i = 0; i < gameBoard.R; i++)
	{
		std::getline(fin, gameBoard.matrix[i]);
		StringUtils::replaceAll(gameBoard.matrix[i], "\r", "");
		//resize each matrix row to size=gameBoard.C, add ' ' if needed 
		gameBoard.matrix[i].resize(gameBoard.C, ' ');
	}
	fin.close();

	//replace all unknown characters with ' '
	for (int i = 0; i < gameBoard.R; i++){
		for (int j = 0; j < gameBoard.C; j++){
			if (gameBoard.matrix[i][j] != ' ') {
				char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);
				if (pos == std::end(typeArr)) { //element not found
					gameBoard.matrix[i][j] = ' ';
				}
			}
		}
	}
	return true;
}

bool BattleshipGameManager::validateBoard(){
	int Acount = 0;
	int Bcount = 0;

	//Search for valid ships
	for (int i = 0; i < gameBoard.R; i++){
		for (int j = 0; j < gameBoard.C; j++){
			char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);
			if (pos != std::end(typeArr)) { //element found
				bool isValidRight = true;
				bool isValidBottom = true;
				//check if need to check ship to the right
				if ((j == 0) || (gameBoard.matrix[i][j - 1] != gameBoard.matrix[i][j])){
					//Check to the right
					isValidRight = isValidShipRight(i, j);
					if (isValidRight){
						(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
					}
				}
				//check if need to check ship to the bottom
				if (!isValidRight) {
					if ((i == 0) || (gameBoard.matrix[i - 1][j] != gameBoard.matrix[i][j])) {
						//Check to the bottom
						isValidBottom = isValidShipBottom(i, j);
						if (isValidBottom) {
							(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
						}
					}
				}
				if (!isValidRight && !isValidBottom){
					updateErrMsgArrWrongSize(gameBoard.matrix[i][j]);
				}
			}
		}		
	}
	//Check valid number of ships
	if (Acount < VALID_SHIP_NUM){
		errMsgArr[int(ErrorMsg::TOO_FEW_A)].first = true;
	}
	else if (Acount > VALID_SHIP_NUM){
		errMsgArr[int(ErrorMsg::TOO_MANY_A)].first = true;
	}
	if (Bcount < VALID_SHIP_NUM){
		errMsgArr[int(ErrorMsg::TOO_FEW_B)].first = true;
	}
	else if (Bcount > VALID_SHIP_NUM){
		errMsgArr[int(ErrorMsg::TOO_MANY_B)].first = true;
	}

	//Check for adjacent
	bool brk = false;
	for (int i = 0; i < gameBoard.R; i++){
		for (int j = 0; j < gameBoard.C; j++){
			if (gameBoard.matrix[i][j] != ' ') {
				//Check different to the bottom
				if ((i != gameBoard.R - 1) &&
					(gameBoard.matrix[i + 1][j] != gameBoard.matrix[i][j]) && (gameBoard.matrix[i + 1][j] != ' ')) {
					errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
					brk = true;
					break;
				}
				//Check different to the right
				if ((j != gameBoard.C - 1) &&
					(gameBoard.matrix[i][j + 1] != gameBoard.matrix[i][j]) && (gameBoard.matrix[i][j + 1] != ' ')) {
					errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
					brk = true;
					break;
				}
			}
		}
		if (brk) break;
	}

	//Check for error messages - print error message if found
	bool valid = true;
	for (int i = 0; i < int(ErrorMsg::ERR_MGS_MAX); i++){
		if (errMsgArr[i].first == true){
			//Print the relevant error message
			std::cout << errMsgArr[i].second << std::endl;
			valid = false;
		}
	}
	return (valid);
}

bool BattleshipGameManager::isValidShipRight(int x, int y) const{
	char type = gameBoard.matrix[x][y];
	int size = 0;

	while ((y < gameBoard.C) && (gameBoard.matrix[x][y] == type)){
		size++;
		//if same type above/bottom - return false
		if (((x != 0) && (gameBoard.matrix[x - 1][y] == type)) ||
			((x != gameBoard.R-1) && (gameBoard.matrix[x + 1][y] == type))){
			return false;
		}
		y++;
	}
	return (size == getSize(type));
}

bool BattleshipGameManager::isValidShipBottom(int x, int y) const{
	char type = gameBoard.matrix[x][y];
	int size = 0;

	while ((x < gameBoard.R) && (gameBoard.matrix[x][y] == type)){
		size++;
		//if same type left/right - return false
		if (((y != 0) && (gameBoard.matrix[x][y - 1] == type)) ||
			((y != gameBoard.C - 1) && (gameBoard.matrix[x][y + 1] == type))){
			return false;
		}
		x++;
	}
	return (size == getSize(type));
}

int BattleshipGameManager::getSize(char type){

	switch (type){
		case 'D':{
					 return int(shipSize::dSize);
					 break;
		}
		case 'M':{
					 return int(shipSize::mSize);
					 break;
		}
		case 'P':{
					 return int(shipSize::pSize);
					 break;
		}
		case 'B':{
					 return int(shipSize::bSize);
					 break;
		}
		case 'd':{
					 return int(shipSize::dSize);
					 break;
		}
		case 'm':{
					 return int(shipSize::mSize);
					 break;
		}
		case 'p':{
					 return int(shipSize::pSize);
					 break;
		}
		case 'b':{
					 return int(shipSize::bSize);
					 break;
		}
		default:{
					return 0;
					break;
		}
	}
}

void BattleshipGameManager::updateErrMsgArrWrongSize(char type){
	switch (type){
		case 'D':{
					 errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_D_A)].first = true;
					 break;
		}
		case 'M':{
					 errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_M_A)].first = true;
					 break;
		}
		case 'P':{
					 errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_P_A)].first = true;
					 break;
		}
		case 'B':{
					 errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_B_A)].first = true;
					 break;
		}
		case 'd':{
					 errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_D_B)].first = true;
					 break;
		}
		case 'm':{
					 errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_M_B)].first = true;
					 break;
		}
		case 'p':{
					 errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_P_B)].first = true;
					 break;
		}
		case 'b':{
					 errMsgArr[int(ErrorMsg::WRONG_SIZE_SHAPE_B_B)].first = true;
					 break;
		}
		default:{
					break;
		}
	}
}

int BattleshipGameManager::getSinkScoreByChar(char c) {
	for (int i = 0; i < NUM_OF_SHIP_TYPES; i++) {
		if (typeArr[i] == c || typeArr[i + NUM_OF_SHIP_TYPES] == c) {
			return sinkScoreArr[i];
		}
	}
	return 0;
}

bool BattleshipGameManager::isActivePlayer(int playerIndex) const{
	return numShips[playerIndex] > 0;
}

bool BattleshipGameManager::isLonely(const BattleBoard& gameBoard, int row, int col) {
	int rowIndexDown = row + 1, rowIndexUp = row - 1, colIndexLeft = col - 1, colIndexRight = col + 1;
	while (rowIndexDown < NUM_ROWS && gameBoard.matrix[rowIndexDown][col] != ' ') {
		if (gameBoard.matrix[rowIndexDown][col] != '*') {
			return false;
		}
		rowIndexDown++;
	}
	while (rowIndexUp >= 0 && gameBoard.matrix[rowIndexUp][col] != ' ') {
		if (gameBoard.matrix[rowIndexUp][col] != '*') {
			return false;
		}
		rowIndexUp--;
	}
	while (colIndexLeft >= 0 && gameBoard.matrix[row][colIndexLeft] != ' ') {
		if (gameBoard.matrix[row][colIndexLeft] != '*') {
			return false;
		}
		colIndexLeft--;
	}
	while (colIndexRight < NUM_COLS && gameBoard.matrix[row][colIndexRight] != ' ') {
		if (gameBoard.matrix[row][colIndexRight] != '*') {
			return false;
		}
		colIndexRight++;
	}
	return true;
}

int BattleshipGameManager::handleMove(int currPlayer, BattleBoard& gameBoard, int row, int col) {
	char c = gameBoard.matrix[row][col];
	bool starFlag = c == '*';
	bool lonelyFlag = isLonely(gameBoard, row, col);
	int sinkScore = getSinkScoreByChar(c);
	if (sinkScore == 0) {
		if (starFlag) {
			if (lonelyFlag) {
				// second hit on a ship that's sunk - report miss
				return 0;
			}
			else {
				// second hit on a ship that's still afloat - report hit
				return -1;
			}
		}
		else {
			// simply a bad guess
			return 0;
		}
	}

	// if we reached here, that means a ship was hit
	bool lowerFlag = islower(c) == 0 ? false : true;
	gameBoard.matrix[row][col] = '*';		// mark coordinate as a hit
	if (((currPlayer % 2 == 0 && lowerFlag) || (currPlayer % 2 == 1 && !lowerFlag))) {
		if (lonelyFlag) {
			// report sink
			numShips[(currPlayer + 1) % 2] -= 1;
			scores[currPlayer % 2] += sinkScore;
			return sinkScore;
		}
		else {
			// report hit
			return 1;
		}
	}
	else {
		if ((currPlayer % 2 == 0 && !lowerFlag) || (currPlayer % 2 == 1 && lowerFlag)) {
			// Own goal
			if (lonelyFlag) {
				// self sink
				numShips[currPlayer % 2] -= 1;
				scores[(currPlayer + 1) % 2] += sinkScore;
				return -1*sinkScore;
			}
			else {
				// self hit
				return -1;
			}
		}
		else {
			return 0;
		}
	}
}

int BattleshipGameManager::numActivePlayers() const{
	int count = 0;
	for (int i = 0; i < NUM_PLAYERS; i++) {
		if (isActivePlayer(i)) {
			count += 1;
		}
	}
	return count;
}
