
#include "BattleshipGameManager.h"
#include "BattleshipGameAlgoFromFile.h"
#include "BattleshipGameAlgoNaive.h"
#include "BattleshipGameAlgoSmart.h"

bool BattleshipGameManager::initGame(){
	
	bool boardFileExists = inputProcessor.tryExtractBoardFileName();

	bool boardValid = false;
	if (boardFileExists) {
		boardValid = initBoard();
	}

	bool dllFilesExist = inputProcessor.tryExtractDllFileNames();

	if (!boardFileExists || !boardValid || !dllFilesExist)
	{
		return false;
	}

	if (!initPlayers())
	{
		return false;
	}

	return true;
};

bool BattleshipGameManager::initBoard()
{
	//Read board file to matrix
	if (!readBoardFileToMatrix(inputProcessor.getBoardFilePath())) return false;

	//Validate board
	if (!validateBoard())
	{
		return false;
	}

	return true;
}

bool BattleshipGameManager::initPlayers()
{
	//Send each player his board
	playerA = new BattleshipGameAlgoFromFile();
	sendBoard(true);//player A
	if (!playerA->init(inputProcessor.getFolderPath()))
	{
		std::cout << "Algorithm initialization failed for dll: " << inputProcessor.getPlayerADllFilePath() << std::endl;
		return false;
	}

	playerB = new BattleshipGameAlgoSmart();
	sendBoard(false);//player B
	if (!playerB->init(inputProcessor.getFolderPath()))
	{
		std::cout << "Algorithm initialization failed for dll: " << inputProcessor.getPlayerBDllFilePath() << std::endl;
		return false;
	}

	return true;
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
	Sleep(2000);	// see results before quitting
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
				return 1;
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
		std::cout << "PLAYER " << (player % 2 == 0 ? "A" : "B") << ": (" << (row + 1) << "," << (col + 1) << ")";
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