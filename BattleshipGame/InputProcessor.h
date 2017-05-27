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
	int delayMs = DEFAULT_DELAY_MS;
	bool quiet = false;

	const std::string boardSuffix = ".sboard";
	const std::string dllSuffix = ".dll";

	
public:

	std::vector<std::string> boardFilesPaths;
	std::vector<std::string> dllFilesPaths;

	InputProcessor(int argc, char* argv[]);

	bool validateFolderPath();
	bool validateInput(); //Validates files exist.
	bool tryExtractBoardFileNames();
	bool tryExtractDllFileNames();

	int getDelayMs() const;
	bool getQuiet() const;
	std::string getFolderPath() const;

	friend class InputProcessorTest;

private:
	void updateDelayParamIfNeeded(const std::string& arg, size_t find);


};

#endif