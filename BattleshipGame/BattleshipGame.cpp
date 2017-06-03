#include "BattleshipGameManager.h"
#include "BattleshipGameManagerTest.h"
#include "InputProcessor.h"
#include "StringUtilsTest.h"
#include "InputProcessorTest.h"
#include "FileUtilsTest.h"
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
}


