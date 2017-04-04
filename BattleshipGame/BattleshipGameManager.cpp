
#include "BattleshipGameManager.h"

void initGame(){
	//TODO: IMPLEMENT
};

void playGame(){
	//TODO: IMPLEMENT
};

void BattleshipGameManager::readBoardFileToMatrix(const std::string boardFile, BattleBoard& gameBoard){

	std::ifstream fin(boardFile);
	gameBoard.R = ROW;
	gameBoard.C = COL;
	gameBoard.matrix = new std::string[gameBoard.R];
	for (int i = 0; i < gameBoard.R; ++i)
	{
		std::getline(fin, gameBoard.matrix[i]);
	}
	fin.close();
}

void BattleshipGameManager::validateBoard(const BattleBoard& gameBoard){
	int Acount = 0;
	int Bcount = 0;
	bool isValidRight;
	bool isValidBottom;

	//Search for valid ships
	for (int i = 0; i < gameBoard.R; ++i){
		for (int j = 0; j < gameBoard.C; ++j){
			char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);	
			if (pos != std::end(typeArr)) { //element found
				isValidRight = true;
				isValidBottom = true;
				//check if need to check ship to the right
				if ((i == 0) || (gameBoard.matrix[i - 1][j] != gameBoard.matrix[i][j])){
					//Check to the right
					isValidRight = isValidShipRight(i, j, gameBoard);
					if (isValidRight){
						(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
					}
				}
				//check if need to check ship to the bottom
				if ((j == 0) || (gameBoard.matrix[i][j - 1] != gameBoard.matrix[i][j])){
					//Check to the bottom
					isValidBottom = isValidShipBottom(i, j, gameBoard);
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
	char* bottomPos;
	char* rightPos;
	for (int i = 0; i < gameBoard.R; ++i){
		for (int j = 0; j < gameBoard.C; ++j){
			//Check different to the bottom
			if ((j != gameBoard.R - 1) && (gameBoard.matrix[i][j + 1] != gameBoard.matrix[i][j])){
				bottomPos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j + 1]);
				if (bottomPos != std::end(typeArr)) { // fount adjacent
					errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
					brk = true;
					break;
				}
			}
			//Check different to the right
			if ((i != gameBoard.C - 1) && (gameBoard.matrix[i + 1][j] != gameBoard.matrix[i][j])){
				rightPos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i + 1][j]);
				if (rightPos != std::end(typeArr)) { // fount adjacent
					errMsgArr[int(ErrorMsg::AJACENT_ON_BOARD)].first = true;
					brk = true;
					break;
				}
			}
		}
		if (brk) break;
	}

	//Check for error messages - print if needed
	for (int i = 0; i < int(ErrorMsg::ERR_MGS_MAX); i++){
		if (errMsgArr[i].first == true){
			std::cout << errMsgArr[i].second << std::endl;
		}
	}
}

bool BattleshipGameManager::isValidShipRight(int x, int y, const BattleBoard& gameBoard){
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

bool BattleshipGameManager::isValidShipBottom(int x, int y, const BattleBoard& gameBoard){
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