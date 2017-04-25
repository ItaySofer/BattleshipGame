#ifndef InputProcessorH
#define InputProcessorH
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "StringUtils.h"
#include "Macros.h"

class InputProcessor
{
	std::string folderPath = "";
	std::string boardFilePath = "";
	std::string dllFilesPaths[NUM_PLAYERS] = { "", "" };
	int delayMs = DEFAULT_DELAY_MS;
	bool quiet = false;

	
public:
	InputProcessor(int argc, char* argv[]);
	~InputProcessor() = default;

	bool validateFolderPath();
	bool validateInput(); //Validates files exist.
	bool tryExtractBoardFileName();
	bool tryExtractDllFileNames();

	std::string getPlayerADllFilePath();
	std::string getPlayerBDllFilePath();
	virtual std::string getBoardFilePath();
	int getDelayMs();
	bool getQuiet();
	std::string getFolderPath();

	friend class InputProcessorTest;

private:
	const std::string boardSuffix = ".sboard";
	const std::string dllSuffix = ".dll";

};

#endif