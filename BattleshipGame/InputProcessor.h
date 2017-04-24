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
	std::string attackFiles[NUM_PLAYERS] = { "", "" };
	std::string dllFiles[NUM_PLAYERS] = { "", "" };
	int delayMs = DEFAULT_DELAY_MS;
	bool quiet = false;

	
public:
	InputProcessor(int argc, char* argv[]);
	~InputProcessor() = default;

	bool tryExtractFileNames();
	bool validateInput(); //Validates files exist.
	bool validateBoardFileExists();
	bool validateDllFilesExist();

	std::string getPlayerAAttackFilePath();
	std::string getPlayerBAttackFilePath();
	std::string getPlayerADllFilePath();
	std::string getPlayerBDllFilePath();
	virtual std::string getBoardFilePath();
	int getDelayMs();
	bool getQuiet();

	friend class InputProcessorTest;

private:
	const std::string boardSuffix = ".sboard";
	const std::string attackSuffix = ".attack";
	const std::string dllSuffix = ".dll";

	static std::string concatenateAbsolutePath(const std::string& dirPath, const std::string& fileName);

};

#endif