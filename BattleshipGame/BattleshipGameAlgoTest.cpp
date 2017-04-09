#include "BattleshipGameAlgoTest.h"

void BattleshipGameAlgoTest::attackTest()
{
	BattleshipGameAlgo player("a.attack-a");
	std::pair<int, int> emptyAttack(-1, -1);

	std::vector<std::pair<int, int>> expected = { std::pair<int, int>(0, 1) ,
		std::pair<int, int>(1, 2) , 
		std::pair<int, int>(2, 3) ,
		std::pair<int, int>(3, 4) ,
		std::pair<int, int>(1, 3) };
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

void BattleshipGameAlgoTest::attackTestFromOtherFolder()
{
	BattleshipGameAlgo player("C:/Users/Itay/Desktop/examples/a.attack-a");
	std::pair<int, int> emptyAttack(-1, -1);

	std::vector<std::pair<int, int>> expected = { std::pair<int, int>(0, 1) ,
		std::pair<int, int>(1, 2) ,
		std::pair<int, int>(2, 3) ,
		std::pair<int, int>(3, 4) ,
		std::pair<int, int>(1, 3) };
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
