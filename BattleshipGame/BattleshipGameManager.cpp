
#include "BattleshipGameManager.h"

bool BattleshipGameManager::initGame(const std::string boardFile){
	//Read board file to matrix
	readBoardFileToMatrix(boardFile);

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
		std::strcpy(board[i], gameBoard.matrix[i].c_str());
	}
	modifyBoard(board, isPlayerA);
	IBattleshipGameAlgo& player = isPlayerA ? playerA : playerB;
	player.setBoard(board, gameBoard.R, gameBoard.C);

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
				if (((std::distance(typeArr, pos) < VALID_SHIP_NUM) && !isPlayerA) || 
					((std::distance(typeArr, pos) >= VALID_SHIP_NUM) && isPlayerA)){
					board[i][j] = ' ';
				}
			}
		}
	}
}

void BattleshipGameManager::playGame(){
	//TODO: IMPLEMENT
};

void BattleshipGameManager::readBoardFileToMatrix(const std::string boardFile){

	std::ifstream fin(boardFile);
	gameBoard.R = NUM_ROWS;
	gameBoard.C = NUM_COLS;
	gameBoard.matrix = new std::string[gameBoard.R];
	char* temp;
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
			pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);
			if (pos == std::end(typeArr)) { //element not found
				gameBoard.matrix[i][j] = ' ';
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
				if ((i == 0) || (gameBoard.matrix[i - 1][j] != gameBoard.matrix[i][j])){
					//Check to the right
					isValidRight = isValidShipRight(i, j);
					if (isValidRight){
						(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
					}
				}
				//check if need to check ship to the bottom
				if ((j == 0) || (gameBoard.matrix[i][j - 1] != gameBoard.matrix[i][j])){
					//Check to the bottom
					isValidBottom = isValidShipBottom(i, j);
					if (isValidBottom){
						(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
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
			//Check different to the bottom
			if ((j != gameBoard.R - 1) && (gameBoard.matrix[i][j + 1] != gameBoard.matrix[i][j]) && (gameBoard.matrix[i][j + 1] != ' ')){
				errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
				brk = true;
				break;
			}
			//Check different to the right
			if ((i != gameBoard.C - 1) && (gameBoard.matrix[i + 1][j] != gameBoard.matrix[i][j]) && (gameBoard.matrix[i + 1][j] != ' ')){
				errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
				brk = true;
				break;
			}
		}
		if (brk) break;
	}

	//Check for error messages - print if needed
	bool valid = true;
	for (int i = 0; i < int(ErrorMsg::ERR_MGS_MAX); i++){
		if (errMsgArr[i].first == true){
			std::cout << errMsgArr[i].second << std::endl;
			valid = false;
		}
	}
	return valid;
}

bool BattleshipGameManager::isValidShipRight(int x, int y){
	char type = gameBoard.matrix[x][y];
	int size = 0;

	while ((x < gameBoard.C-1) && (gameBoard.matrix[x][y] == type)){
		size++;
		//if same type above/bottom - return false
		if ((y != 0) && (gameBoard.matrix[x][y - 1] == type) ||
			(y != gameBoard.R-1) && (gameBoard.matrix[x][y + 1] == type)){
			return false;
		}
		x++;
	}
	return (size == getSize(type));
}

bool BattleshipGameManager::isValidShipBottom(int x, int y){
	char type = gameBoard.matrix[x][y];
	int size = 0;

	while ((y < gameBoard.R - 1) && (gameBoard.matrix[x][y] == type)){
		size++;
		//if same type left/right - return false
		if ((x != 0) && (gameBoard.matrix[x-1][y] == type) ||
			(x != gameBoard.C - 1) && (gameBoard.matrix[x+1][y] == type)){
			return false;
		}
		y++;
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