#pragma once
#include "IBattleshipGameAlgo.h"

class BattleshipGameManager {
	IBattleshipGameAlgo& playerA;
	IBattleshipGameAlgo& playerB;

public:
	BattleshipGameManager(IBattleshipGameAlgo* plrA, IBattleshipGameAlgo* plrB) : playerA(*plrA), playerB(*plrB) {};
	~BattleshipGameManager() = default;
	
	void initGame(); //initiates board, calls players' "setBoard" methods

	void playGame(); //runs game, notifies players about move results. Continiusly checks game status and ends it when needed. 
					//Output propper messages ("Player _ won. Points: ...")
};
