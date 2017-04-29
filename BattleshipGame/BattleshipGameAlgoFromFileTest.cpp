#include "BattleshipGameAlgoFromFileTest.h"

void BattleshipGameAlgoFromFileTest::attackTest()
{
	BattleshipGameAlgoFromFile player;
	player.setBoard(0, NULL, 0, 0);
	player.init("");
	std::pair<int, int> emptyAttack(-1, -1);

	std::vector<std::pair<int, int>> expected = { std::pair<int, int>(1, 2) ,
		std::pair<int, int>(2, 3) ,
		std::pair<int, int>(3, 4) ,
		std::pair<int, int>(4, 5) ,
		std::pair<int, int>(2, 4) };
	std::vector<std::pair<int, int>> actual;

	std::pair<int, int> attack = player.attack();
	while (attack != emptyAttack)
	{
		actual.push_back(attack);
		attack = player.attack();
	}

	attack = player.attack(); //to check calling atack() after player ended his attacks doesn't crash
	attack = player.attack();
	attack = player.attack();

	if (actual != expected) {
		std::cout << "attackTest Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}

void BattleshipGameAlgoFromFileTest::attackTestFromOtherFolder()
{
	BattleshipGameAlgoFromFile player;
	player.setBoard(0, NULL, 0, 0);
	player.init("C:/Users/Itay/Desktop/examples");
	std::pair<int, int> emptyAttack(-1, -1);

	std::vector<std::pair<int, int>> expected = { std::pair<int, int>(1, 2) ,
		std::pair<int, int>(2, 3) ,
		std::pair<int, int>(3, 4) ,
		std::pair<int, int>(4, 5) ,
		std::pair<int, int>(2, 4) };
	std::vector<std::pair<int, int>> actual;

	std::pair<int, int> attack = player.attack();
	while (attack != emptyAttack)
	{
		actual.push_back(attack);
		attack = player.attack();
	}

	attack = player.attack(); //to check calling atack() after player ended his attacks doesn't crash
	attack = player.attack();
	attack = player.attack();

	if (actual != expected) {
		std::cout << "attackTestFromOtherFolder Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}

void BattleshipGameAlgoFromFileTest::attackTestFromExamples()
{
	BattleshipGameAlgoFromFile player;
	player.setBoard(0, NULL, 0, 0);
	player.init("C:/Users/Itay/Desktop/TestFiles/moves/ilegal");
	std::pair<int, int> emptyAttack(-1, -1);

	std::vector<std::pair<int, int>> actual;

	std::pair<int, int> attack = player.attack();
	while (attack != emptyAttack)
	{
		if (attack.first > NUM_ROWS || attack.first <= 0 || attack.second > NUM_COLS || attack.second <= 0)
		{
			std::cout << "attackTestFromExamples Failed: invalid attack returned - ";
			std::cout << attack << std::endl;		
		}
		actual.push_back(attack);
		attack = player.attack();
	}

	std::cout << std::endl;
}

void BattleshipGameAlgoFromFileTest::sameAttackFile()
{
	BattleshipGameAlgoFromFile player;
	player.setBoard(1, NULL, 0, 0);
	player.init("C:/Users/Itay/Desktop/examples");
	std::pair<int, int> emptyAttack(-1, -1);

	std::vector<std::pair<int, int>> expected = { std::pair<int, int>(1, 2) ,
		std::pair<int, int>(2, 3) ,
		std::pair<int, int>(3, 4) ,
		std::pair<int, int>(4, 5) ,
		std::pair<int, int>(2, 4) };
	std::vector<std::pair<int, int>> actual;

	std::pair<int, int> attack = player.attack();
	while (attack != emptyAttack)
	{
		actual.push_back(attack);
		attack = player.attack();
	}

	attack = player.attack(); //to check calling atack() after player ended his attacks doesn't crash
	attack = player.attack();
	attack = player.attack();

	if (actual != expected) {
		std::cout << "sameAttackFile Failed: Expected is ";
		std::cout << expected;
		std::cout << " but was ";
		std::cout << actual;
		std::cout << std::endl;
	}
}
