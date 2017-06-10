#include "BattleshipGameManager.h"
#include "MatchResult.h"

void BattleshipGameManager::initGame(){
	
	initPlayers();
};

void BattleshipGameManager::initPlayers()
{	
	//Send each player his board
	sendBoard(true);//player A

	sendBoard(false);//player B	
	
}

void BattleshipGameManager::sendBoard(bool isPlayerA){

	std::unique_ptr<IBattleshipGameAlgo>& player = isPlayerA ? playerA : playerB;
	int playerNum = isPlayerA ? 0 : 1;
	player->setPlayer(playerNum);
	if (isPlayerA)
	{
		player->setBoard(playerABoard);
	} else
	{
		player->setBoard(playerBBoard);
	}
}

void BattleshipGameManager::modifyBoard(BattleBoard& board, bool isPlayerA) {
	for (int k = 0; k < gameBoard.depth(); k++) {
		for (int i = 0; i < gameBoard.rows(); i++) {
			for (int j = 0; j < gameBoard.cols(); j++) {
				if (board.matrix[k][i][j] != ' ') {
					char* pos = std::find(std::begin(typeArr), std::end(typeArr), gameBoard.matrix[k][i][j]);
					if (((std::distance(typeArr, pos) < NUM_OF_SHIP_TYPES) && !isPlayerA) ||
						((std::distance(typeArr, pos) >= NUM_OF_SHIP_TYPES) && isPlayerA)) {
						board.matrix[k][i][j] = ' ';
					}
				}
			}
		}
	}
}

MatchResult BattleshipGameManager::playGame() {
	int currPlayer = STARTING_PLAYER;
	int doneAttackingPlayers = 0;
	while (numActivePlayers() > 1 && doneAttackingPlayers < NUM_PLAYERS) {
		Coordinate currAttack = (currPlayer % 2 == 0) ? playerA->attack() : playerB->attack();
		if (currAttack.row == -1 && currAttack.col == -1 && currAttack.depth == -1) {
			// currPlayer is done attacking
			doneAttackingPlayers += 1;
			currPlayer += 1;
			continue;
		}

		if (currAttack.row < 1 || currAttack.col < 1 || currAttack.depth < 1 || 
			currAttack.row > gameBoard.rows() || currAttack.col > gameBoard.cols() || currAttack.depth > gameBoard.depth()) {
			// illegal attack
			continue;
		}

		int roundResult = handleMove(currPlayer, gameBoard, currAttack);

		if (std::abs(roundResult) > 1) {
			// currPlayer sink
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack, AttackResult::Sink);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack, AttackResult::Sink);
			currPlayer += (roundResult > 0 || doneAttackingPlayers > 0) ? 0 : 1;
		}
		else if (std::abs(roundResult) == 1) {
			// currPlayer hit
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack, AttackResult::Hit);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack, AttackResult::Hit);
			currPlayer += (roundResult > 0 || doneAttackingPlayers > 0) ? 0 : 1;
		}
		else if (roundResult == 0) {
			// currPlayer miss
			playerA->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack, AttackResult::Miss);
			playerB->notifyOnAttackResult(currPlayer % NUM_PLAYERS, currAttack, AttackResult::Miss);
			currPlayer += doneAttackingPlayers > 0 ? 0 : 1;
		}
	}
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

	return res;
};

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

bool BattleshipGameManager::isLonely(const BattleBoard& gameBoard, const Coordinate& coor) {
	int row = coor.row, col = coor.col, depth = coor.depth;
	int rowIndexDown = row + 1, rowIndexUp = row - 1, colIndexLeft = col - 1, colIndexRight = col + 1;
	int depthIndexBack = depth - 1, depthIndexFront = depth + 1;
	Coordinate travel = Coordinate(row, col, depth);
	char travelChar;
	while (rowIndexDown <= gameBoard.rows()) {
		travel.row = rowIndexDown;
		travelChar = gameBoard.charAt(travel);
		if (travelChar == ' ') {
			break;
		}
		if (travelChar != '*') {
			return false;
		}
		rowIndexDown++;
	}
	while (rowIndexUp > 0) {
		travel.row = rowIndexUp;
		travelChar = gameBoard.charAt(travel);
		if (travelChar == ' ') {
			break;
		}
		if (travelChar != '*') {
			return false;
		}
		rowIndexUp--;
	}
	travel.row = row;
	while (colIndexLeft > 0) {
		travel.col = colIndexLeft;
		travelChar = gameBoard.charAt(travel);
		if (travelChar == ' ') {
			break;
		}
		if (travelChar != '*') {
			return false;
		}
		colIndexLeft--;
	}
	while (colIndexRight <= gameBoard.cols()) {
		travel.col = colIndexRight;
		travelChar = gameBoard.charAt(travel);
		if (travelChar == ' ') {
			break;
		}
		if (travelChar != '*') {
			return false;
		}
		colIndexRight++;
	}
	travel.col = col;
	while (depthIndexBack > 0) {
		travel.depth = depthIndexBack;
		travelChar = gameBoard.charAt(travel);
		if (travelChar == ' ') {
			break;
		}
		if (travelChar != '*') {
			return false;
		}
		depthIndexBack--;
	}
	while (depthIndexFront <= gameBoard.depth()) {
		travel.depth = depthIndexFront;
		travelChar = gameBoard.charAt(travel);
		if (travelChar == ' ') {
			break;
		}
		if (travelChar != '*') {
			return false;
		}
		depthIndexFront++;
	}
	return true;
}

int BattleshipGameManager::handleMove(int currPlayer, BattleBoard& gameBoard, const Coordinate& coor) {
	char c = gameBoard.charAt(coor);
	bool starFlag = c == '*';
	bool lonelyFlag = isLonely(gameBoard, coor);
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
	gameBoard.matrix[coor.depth - 1][coor.row - 1][coor.col - 1] = '*';		// mark coordinate as a hit
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
			count++;
		}
	}
	return count;
}
