
#include "BattleshipGameManager.h"

bool BattleshipGameManager::initGame(const std::string boardFilePath){
	//Read board file to matrix
	readBoardFileToMatrix(boardFilePath);

	//Validate board
	if (!validateBoard()) return false;

	//Send each player his board
	sendBoard(true);//player A
	sendBoard(false);//player B

	return true;
};

void BattleshipGameManager::sendBoard(bool isPlayerA){
	char** board;
	board = new char*[NUM_ROWS];
	for (int i = 0; i < gameBoard.R; i++)
	{
		board[i] = new char[NUM_COLS];
		std::memcpy(board[i], gameBoard.matrix[i].c_str(), NUM_COLS);
	}
	modifyBoard(board, isPlayerA);
	IBattleshipGameAlgo& player = isPlayerA ? playerA : playerB;
	player.setBoard(const_cast<const char**>(board), gameBoard.R, gameBoard.C);

	//Delete board
	for (int i = 0; i < gameBoard.R; i++)
	{
		delete[] board[i];
	}
	delete[] board;
}

void BattleshipGameManager::modifyBoard(char** board, bool isPlayerA){
	char* pos;
	for (int i = 0; i < gameBoard.R; i++){
		for (int j = 0; j < gameBoard.R; j++){
			if (board[i][j] != ' '){
				pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);
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
	while (numActivePlayers() > 1 && doneAttackingPlayers < NUM_PLAYERS) {
		std::pair<int, int> currAttack = (currPlayer % 2 == 0) ? playerA.attack() : playerB.attack();
		if (currAttack.first < 0) {
			// currPlayer is done attacking
			doneAttackingPlayers += 1;
			currPlayer += 1;
			continue;
		}
		int roundResult = handleMove(currPlayer, gameBoard, currAttack.first, currAttack.second);
		if (roundResult > 1) {
			// currPlayer sink
			playerA.notifyOnAttackResult(currPlayer, currAttack.first, currAttack.second, AttackResult::Sink);
			playerB.notifyOnAttackResult(currPlayer, currAttack.first, currAttack.second, AttackResult::Sink);
		}
		else if (roundResult == 1) {
			// currPlayer hit
			playerA.notifyOnAttackResult(currPlayer, currAttack.first, currAttack.second, AttackResult::Hit);
			playerB.notifyOnAttackResult(currPlayer, currAttack.first, currAttack.second, AttackResult::Hit);
		}
		else if (roundResult == 0) {
			// currPlayer miss
			playerA.notifyOnAttackResult(currPlayer, currAttack.first, currAttack.second, AttackResult::Miss);
			playerB.notifyOnAttackResult(currPlayer, currAttack.first, currAttack.second, AttackResult::Miss);
			currPlayer += 1;
		}
	}
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
};

void BattleshipGameManager::readBoardFileToMatrix(const std::string boardFile){

	std::ifstream fin(boardFile);
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
	char* pos;
	for (int i = 0; i < gameBoard.R; i++){
		for (int j = 0; j < gameBoard.C; j++){
			if (gameBoard.matrix[i][j] != ' ') {
				pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);
				if (pos == std::end(typeArr)) { //element not found
					gameBoard.matrix[i][j] = ' ';
				}
			}
		}
	}
}

bool BattleshipGameManager::validateBoard(){
	int Acount = 0;
	int Bcount = 0;
	bool isValidRight;
	bool isValidBottom;

	//Search for valid ships
	char* pos;
	for (int i = 0; i < gameBoard.R; i++){
		for (int j = 0; j < gameBoard.C; j++){
			pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);	
			if (pos != std::end(typeArr)) { //element found
				isValidRight = true;
				isValidBottom = true;
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

bool BattleshipGameManager::isValidShipRight(int x, int y){
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

bool BattleshipGameManager::isValidShipBottom(int x, int y){
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

bool BattleshipGameManager::isActivePlayer(int playerIndex) {
	return numShips[playerIndex] > 0;
}

bool BattleshipGameManager::isLonely(BattleBoard& gameBoard, int row, int col) {
	int rowIndexDown = row + 1, rowIndexUp = row - 1, colIndexLeft = col - 1, colIndexRight = col + 1;
	while (rowIndexDown < NUM_ROWS && gameBoard.matrix[rowIndexDown][col] != ' ') {
		if (gameBoard.matrix[rowIndexDown][col] != '*') {
			return false;
		}
		rowIndexDown += 1;
	}
	while (rowIndexUp >= 0 && gameBoard.matrix[rowIndexUp][col] != ' ') {
		if (gameBoard.matrix[rowIndexUp][col] != '*') {
			return false;
		}
		rowIndexUp -= 1;
	}
	while (colIndexLeft >= 0 && gameBoard.matrix[row][colIndexLeft] != ' ') {
		if (gameBoard.matrix[row][colIndexLeft] != '*') {
			return false;
		}
		colIndexLeft -= 1;
	}
	while (colIndexRight < NUM_COLS && gameBoard.matrix[row][colIndexRight] != ' ') {
		if (gameBoard.matrix[row][colIndexRight] != '*') {
			return false;
		}
		colIndexRight += 1;
	}
	return true;
}

int BattleshipGameManager::handleMove(int currPlayer, BattleBoard& gameBoard, int row, int col) {
	char c = gameBoard.matrix[row][col];
	bool lowerFlag = islower(c), lonelyFlag = isLonely(gameBoard, row, col);
	int sinkScore = getSinkScoreByChar(c);
	if (sinkScore == 0) return 0;
	// if we reached here, that means a ship was hit
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
		// report miss
		if (lonelyFlag && ((currPlayer % 2 == 0 && !lowerFlag) || (currPlayer % 2 == 1 && lowerFlag))) {
			// Own goal
			numShips[currPlayer % 2] -= 1;
			scores[(currPlayer + 1) % 2] += sinkScore;
		}
		return 0;
	}
}

int BattleshipGameManager::numActivePlayers() {
	int count = 0;
	for (int i = 0; i < NUM_PLAYERS; i++) {
		if (isActivePlayer(i)) {
			count += 1;
		}
	}
	return count;
}