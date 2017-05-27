#include <iostream>
#include <string>
#include "BattleshipGameManager.h"
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
	char*** board = new char**[gameBoard.depth()];
	for (int k = 0 ; k < gameBoard.depth() ; k++) {
		for (int i = 0; i < gameBoard.rows(); i++)
		{
			board[k][i] = new char[gameBoard.cols()];
			std::memcpy(board[k][i], gameBoard.matrix[k][i].c_str(), gameBoard.cols());
		}
	}
	
	modifyBoard(board, isPlayerA);
	IBattleshipGameAlgo* player = isPlayerA ? playerA : playerB;
	int playerNum = isPlayerA ? 0 : 1;
	player->setPlayer(playerNum);

	BattleBoard battleBoard;//TODO: consider init battleBoard with copy constructor from gameBoard, and dont use char*** board
	for (int k = 0; k < gameBoard.depth(); k++){
		for (int i = 0; i < gameBoard.rows(); i++) {
			for (int j = 0; j < gameBoard.cols(); j++) {
				battleBoard.matrix[k][i][j] = board[k][i][j];
			}
		}
	}
	player->setBoard(battleBoard);

	//Delete board
	for (int k = 0; k < gameBoard.depth(); k++)
	{
		for (int i = 0; i < gameBoard.rows(); i++)
		{
			delete[] board[k][i];
		}
		delete[] board[k];
	}
	delete[] board;

}

void BattleshipGameManager::modifyBoard(char*** board, bool isPlayerA) {
	for (int k = 0; k < gameBoard.depth(); k++) {
		for (int i = 0; i < gameBoard.rows(); i++) {
			for (int j = 0; j < gameBoard.cols(); j++) {
				if (board[k][i][j] != ' ') {
					char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[k][i][j]);
					if (((std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) && !isPlayerA) ||
						((std::distance(typeArr, pos) >= NUM_OF_SHIP_TYPES) && isPlayerA)) {
						board[k][i][j] = ' ';
					}
				}
			}
		}
	}
}

void BattleshipGameManager::playGame() {
	int currPlayer = STARTING_PLAYER;
	int doneAttackingPlayers = 0;
	graphicPrintBoard(gameBoard);
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
			goSetPrintSleep(currAttack.first, currAttack.second, SINK_COLOR, '@', currPlayer);
			int rowIndexDown = currAttack.first + 1, rowIndexUp = currAttack.first - 1, colIndexLeft = currAttack.second - 1, colIndexRight = currAttack.second + 1;
			while (rowIndexDown < NUM_ROWS && gameBoard.matrix[rowIndexDown][currAttack.second] != ' ') {
				if (gameBoard.matrix[rowIndexDown][currAttack.second] != '*') {
					break;
				}
				goSetPrintSleep(rowIndexDown, currAttack.second, SINK_COLOR, '@', -1);
				rowIndexDown++;
			}
			while (rowIndexUp >= 0 && gameBoard.matrix[rowIndexUp][currAttack.second] != ' ') {
				if (gameBoard.matrix[rowIndexUp][currAttack.second] != '*') {
					break;
				}
				goSetPrintSleep(rowIndexUp, currAttack.second, SINK_COLOR, '@', -1);
				rowIndexUp--;
			}
			while (colIndexLeft >= 0 && gameBoard.matrix[currAttack.first][colIndexLeft] != ' ') {
				if (gameBoard.matrix[currAttack.first][colIndexLeft] != '*') {
					break;
				}
				goSetPrintSleep(currAttack.first, colIndexLeft, SINK_COLOR, '@', -1);
				colIndexLeft--;
			}
			while (colIndexRight < NUM_COLS && gameBoard.matrix[currAttack.first][colIndexRight] != ' ') {
				if (gameBoard.matrix[currAttack.first][colIndexRight] != '*') {
					break;
				}
				goSetPrintSleep(currAttack.first, colIndexRight, SINK_COLOR, '@', -1);
				colIndexRight++;
			}
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Sink);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Sink);
			currPlayer += (roundResult > 0 || doneAttackingPlayers > 0) ? 0 : 1;
		}
		else if (std::abs(roundResult) == 1) {
			// currPlayer hit
			goSetPrintSleep(currAttack.first, currAttack.second, HIT_COLOR, '*', currPlayer);
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Hit);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Hit);
			currPlayer += (roundResult > 0 || doneAttackingPlayers > 0) ? 0 : 1;
		}
		else if (roundResult == 0) {
			// currPlayer miss
			goSetPrintSleep(currAttack.first, currAttack.second, MISS_COLOR, 'o', currPlayer);
			if (gameBoard.matrix[currAttack.first][currAttack.second] == ' ') {
				goSetPrintSleep(currAttack.first, currAttack.second, MISS_COLOR, ' ', currPlayer);
			}
			else if (gameBoard.matrix[currAttack.first][currAttack.second] == '*') {
				if (isLonely(gameBoard, currAttack.first, currAttack.second)) {
					goSetPrintSleep(currAttack.first, currAttack.second, SINK_COLOR, '@', currPlayer);
				}
				else {
					goSetPrintSleep(currAttack.first, currAttack.second, HIT_COLOR, '*', currPlayer);
				}
			}
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Miss);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack.first + 1, currAttack.second + 1, AttackResult::Miss);
			currPlayer += doneAttackingPlayers > 0 ? 0 : 1;
		}
	}
	system("cls");
	gotoxy(0, 0);
	setTextColor(MISS_COLOR);
	// game ended, announce winner if exists and scores
	if (isActivePlayer(0) && !isActivePlayer(1)) {
		std::cout << "Player A won\n";
	}
	else if (isActivePlayer(1) && !isActivePlayer(0)) {
		std::cout << "Player B won\n";
	}
	std::cout << "Points:\n";
	std::cout << "Player A: " << scores[0] << "\n";
	std::cout << "Player B: " << scores[1] << "\n";

	// free dynamic libs
	/*for (int i = 0; i < NUM_PLAYERS; i++) {
		FreeLibrary(hInstances[i]);
	}*/

	Sleep(2000);	// see results
};

//TODO: remove
bool BattleshipGameManager::readBoardFileToMatrix(const std::string& boardFile){

//	std::ifstream fin(boardFile);
//	if (!fin.is_open())
//	{
//		std::cout << "Error while open internal file" << std::endl;
//		return false;
//	}
//
//	gameBoard.R = NUM_ROWS;
//	gameBoard.C = NUM_COLS;
//	gameBoard.matrix = new std::string[gameBoard.R];
//	for (int i = 0; i < gameBoard.R; i++)
//	{
//		std::getline(fin, gameBoard.matrix[i]);
//		StringUtils::replaceAll(gameBoard.matrix[i], "\r", "");
//		//resize each matrix row to size=gameBoard.C, add ' ' if needed 
//		gameBoard.matrix[i].resize(gameBoard.C, ' ');
//	}
//	fin.close();
//
//	//replace all unknown characters with ' '
//	for (int i = 0; i < gameBoard.R; i++){
//		for (int j = 0; j < gameBoard.C; j++){
//			if (gameBoard.matrix[i][j] != ' ') {
//				char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);
//				if (pos == std::end(typeArr)) { //element not found
//					gameBoard.matrix[i][j] = ' ';
//				}
//			}
//		}
//	}
	return true;
}

bool BattleshipGameManager::validateBoard(){
	int Acount = 0;
	int Bcount = 0;

	//Search for valid ships
	for (int k = 0; k < gameBoard.depth(); k++) {
		for (int i = 0; i < gameBoard.rows(); i++) {
			for (int j = 0; j < gameBoard.cols(); j++) {
				char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[k][i][j]);
				if (pos != std::end(typeArr)) { //element found
					bool isValidRight = true;
					bool isValidDown = true;
					bool isValidUnder = true;
					//check if need to check ship to the right
					if ((j == 0) || (gameBoard.matrix[k][i][j - 1] != gameBoard.matrix[k][i][j])) {
						//Check to the right
						isValidRight = isValidShipRight(k, i, j);
						if (isValidRight) {
							(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
						}
					}
					//check if need to check ship down
					if (!isValidRight) {
						if ((i == 0) || (gameBoard.matrix[k][i - 1][j] != gameBoard.matrix[k][i][j])) {
							//Check down
							isValidDown = isValidShipDown(k, i, j);
							if (isValidDown) {
								(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
							}
						}
					}
					//check if need to check ship under
					if (!isValidRight && !isValidDown) {
						if ((k == 0) || (gameBoard.matrix[k - 1][i][j] != gameBoard.matrix[k][i][j])) {
							//Check to the bottom
							isValidUnder = isValidShipUnder(k, i, j);
							if (isValidUnder) {
								(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
							}
						}
					}
					if (!isValidRight && !isValidDown && !isValidUnder) {
						updateErrMsgArrWrongSize(gameBoard.matrix[k][i][j]);
					}
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
	for (int k = 0; k < gameBoard.depth(); k++) {
		for (int i = 0; i < gameBoard.rows(); i++) {
			for (int j = 0; j < gameBoard.cols(); j++) {
				if (gameBoard.matrix[k][i][j] != ' ') {
					//Check different Down
					if ((i != gameBoard.rows() - 1) &&
						(gameBoard.matrix[k][i + 1][j] != gameBoard.matrix[k][i][j]) && (gameBoard.matrix[k][i + 1][j] != ' ')) {
						errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
						brk = true;
						break;
					}
					//Check different to the right
					if ((j != gameBoard.cols() - 1) &&
						(gameBoard.matrix[k][i][j + 1] != gameBoard.matrix[k][i][j]) && (gameBoard.matrix[k][i][j + 1] != ' ')) {
						errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
						brk = true;
						break;
					}
					//Check different under
					if ((k != gameBoard.depth() - 1) &&
						(gameBoard.matrix[k + 1][i][j] != gameBoard.matrix[k][i][j]) && (gameBoard.matrix[k + 1][i][j] != ' ')) {
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
	for (int i = 0; i < int(ErrorMsg::ERR_MGS_MAX); i++){
		if (errMsgArr[i].first == true){
			////Print the relevant error message
			//std::cout << errMsgArr[i].second << std::endl;
			valid = false;
		}
	}
	return (valid);
}

bool BattleshipGameManager::isValidShipRight(int d, int x, int y) const{
	char type = gameBoard.matrix[d][x][y];
	int size = 0;

	while ((y < gameBoard.cols()) && (gameBoard.matrix[d][x][y] == type)){
		size++;
		//if same type up/down - return false
		if (((x != 0) && (gameBoard.matrix[d][x - 1][y] == type)) ||
			((x != gameBoard.rows()-1) && (gameBoard.matrix[d][x + 1][y] == type))){
			return false;
		}
		//if same type above/under - return false
		if (((d != 0) && (gameBoard.matrix[d - 1][x][y] == type)) ||
			((d != gameBoard.depth() - 1) && (gameBoard.matrix[d + 1][x][y] == type))) {
			return false;
		}
		y++;
	}
	return (size == getSize(type));
}

bool BattleshipGameManager::isValidShipDown(int d, int x, int y) const{
	char type = gameBoard.matrix[d][x][y];
	int size = 0;

	while ((x < gameBoard.rows()) && (gameBoard.matrix[d][x][y] == type)){
		size++;
		//if same type up/down - return false
		if (((x != 0) && (gameBoard.matrix[d][x - 1][y] == type)) ||
			((x != gameBoard.rows() - 1) && (gameBoard.matrix[d][x + 1][y] == type))) {
			return false;
		}
		//if same type left/right - return false
		if (((y != 0) && (gameBoard.matrix[d][x][y - 1] == type)) ||
			((y != gameBoard.cols() - 1) && (gameBoard.matrix[d][x][y + 1] == type))){
			return false;
		}
		d++;
	}
	return (size == getSize(type));
}

bool BattleshipGameManager::isValidShipUnder(int d, int x, int y) const {
	char type = gameBoard.matrix[d][x][y];
	int size = 0;

	while ((d < gameBoard.depth()) && (gameBoard.matrix[d][x][y] == type)) {
		size++;
		//if same type left/right - return false
		if (((y != 0) && (gameBoard.matrix[d][x][y - 1] == type)) ||
			((y != gameBoard.cols() - 1) && (gameBoard.matrix[d][x][y + 1] == type))) {
			return false;
		}
		//if same type above/under - return false
		if (((d != 0) && (gameBoard.matrix[d - 1][x][y] == type)) ||
			((d != gameBoard.depth() - 1) && (gameBoard.matrix[d + 1][x][y] == type))) {
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
//TODO: remove
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

void BattleshipGameManager::graphicPrintBoard(const BattleBoard& gameBoard) const{
	if (inputProcessor.getQuiet()) {
		// no printing is required
		return;
	}

	// disable blinking cursor
	ShowConsoleCursor(false);

	// clear console
	system("cls");

	// go to (0,0)
	gotoxy(0, 0);

	// print board with animations
	std::cout << "   | ";
	for (int k = 0; k < 9; k++) {
		std::cout << k + 1 << " | ";
	}
	for (int k = 9; k < gameBoard.C - 1; k++) {
		std::cout << k + 1;
	}
	std::cout << gameBoard.C << "|\n";
	std::cout << "   |";
	for (int k = 0; k < gameBoard.C; k++) {
		std::cout <<"---|";
	}
	std::cout << "\n";
	for (int i = 0; i < gameBoard.R; i++) {
		//setTextColor(MISS_COLOR);
		if (i < 9) {
			std::cout << (i + 1) << "  | ";
		}
		else {
			std::cout << (i + 1) << " | ";
		}
		for (int j = 0; j < gameBoard.C; j++) {
			if (islower(gameBoard.matrix[i][j])) {
				setTextColor(B_COLOR);
			}
			else {
				setTextColor(A_COLOR);
			}
			std::cout << gameBoard.matrix[i][j];
			setTextColor(MISS_COLOR);
			std::cout << " | ";
		}
		std::cout << "\n";
	}
}

void BattleshipGameManager::gotoxy(int x, int y)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { (short)x, (short)y };
	SetConsoleCursorPosition(h, c);
}

void BattleshipGameManager::setTextColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void BattleshipGameManager::ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void BattleshipGameManager::goSetPrintSleep(int row, int col, int color, char output, int player) const{
	// below constants are used only for this function, thus MACROS definitions are redundant
	if (player >= 0) {
		gotoxy(48, 3);
		setTextColor(MISS_COLOR);
		std::cout << "PLAYER " << (player % 2 == 0 ? "A" : "B") << ": (" << (row + 1) << "," << (col + 1) << ")  ";
	}
	else {
		gotoxy(48, 3);
		std::cout << "                         ";
	}
	gotoxy(5 + 4*col, row+2);
	setTextColor(color);
	std::cout << output;
	Sleep(inputProcessor.getDelayMs());
}