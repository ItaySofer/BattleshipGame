
#include "BattleshipGameManager.h"

void initGame(){
	//TODO: IMPLEMENT
};

void playGame(){
	//TODO: IMPLEMENT
};

void BattleshipGameManager::readBoardFileToMatrix(const std::string boardFile, BattleBoard& gameBoard){
	//cout << "Reading battleBoard from file: simpleBattleBoard.txt into class BattleBoard" << endl;
	std::ifstream fin(boardFile);
	gameBoard.R = RAW;
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

	//Search for valid ships
	for (int i = 0; i < gameBoard.R; ++i){
		for (int j = 0; j < gameBoard.C; ++j){
			char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[i][j]);	
			if (pos != std::end(typeArr)) { //element found
				if (((i == 0) || (gameBoard.matrix[i - 1][j] != gameBoard.matrix[i][j])) && 
					((j == 0) || (gameBoard.matrix[i][j-1] != gameBoard.matrix[i][j]))){//if both left and top != gameBoard.matrix[i][j]
					if (isValidShip(gameBoard.matrix[i][j], i, j, gameBoard)){
						(std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) ? Acount++ : Bcount++;
					}
				}
			}
		}		
	}
	//TODO: check for adjacent
	//TODO: go through errMsgArr array and print error messages if needed
}

bool BattleshipGameManager::isValidShip(char type, int x, int y, const BattleBoard& gameBoard){
	//TODO: save ship positions to a vector when done checking a ship (both valid or invalid)
	//TODO: set errMsgArr[i].first=true if found error of type i
	int size;
	int i;
	bool validRight = true;
	bool validBottom = true;

	switch (gameBoard.matrix[x][y]){
		case 'D':{
					 size = int(shipSize::dSize);
					 break;
		}
		case 'M':{
					 size = int(shipSize::mSize);
					 break;
		}
		case 'P':{
					 size = int(shipSize::pSize);
					 break;
		}
		case 'B':{
					 size = int(shipSize::bSize);
					 break;
		}
		case 'd':{
					 size = int(shipSize::dSize);
					 break;
		}
		case 'm':{
					 size = int(shipSize::mSize);
					 break;
		}
		case 'p':{
					 size = int(shipSize::pSize);
					 break;
		}
		case 'b':{
					 size = int(shipSize::bSize);
					 break;
		}
		default:{
					size = 0;
					break;
		}
	}
	//Check to the right - TODO: consider replacing this test with while loop + check if counter=size in the end.
	for (i = 0; i < size; i++){
		if (y + i > gameBoard.R){
			validRight = false;
			break;
		}
		else if (gameBoard.matrix[x][y + i] != gameBoard.matrix[x][y]){
			validRight = false;
			break;
		}
	}
	if (validRight){//if found valid length, check that the next cell != your type
		if (y + size < gameBoard.R){
			if (gameBoard.matrix[x][y + size] == gameBoard.matrix[x][y]){
				validRight = false;
			}
		}
	}

	//check to the bottom - TODO: consider replacing this test with while loop + check if counter=size in the end.
	for (i = 0; i < size; i++){
		if (x + i > gameBoard.C){
			validBottom = false;
			break;
		}
		else if (gameBoard.matrix[x+i][y] != gameBoard.matrix[x][y]){
			validBottom = false;
			break;
		}
	}
	if (validBottom){//if found valid length, check that the next cell != your type
		if (x + size < gameBoard.C){
			if (gameBoard.matrix[x + size][y] == gameBoard.matrix[x][y]){
				validBottom = false;
			}
		}
	}

	return ((validRight || validBottom) ? false : true);
}
