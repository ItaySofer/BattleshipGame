#include "InputProcessor.h"
#include "BattleshipCompetitionManager.h"

int main(int argc, char* argv[])
{
	InputProcessor inputProcessor(argc, argv);
	if (!inputProcessor.validateFolderPath())
	{
		return -1;
	}
	BattleshipCompetitionManager battleshipCompetitionManager(inputProcessor);

	if (!battleshipCompetitionManager.initCompetition())
	{
		return -1;
	}

	battleshipCompetitionManager.startCompetition();
	std::getchar();
}


