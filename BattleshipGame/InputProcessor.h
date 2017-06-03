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

	const std::string boardSuffix = ".sboard";
	const std::string dllSuffix = ".dll";

	
public:
	
	std::string folderPath = "";
	int threads = NUM_THREADS;
	std::vector<std::string> boardFilesPaths;
	std::vector<std::string> dllFilesPaths;

	InputProcessor(int argc, char* argv[]);

	bool validateFolderPath();
	bool validateInput(); //Validates files exist.
	bool tryExtractBoardFileNames();
	bool tryExtractDllFileNames();

private:
	void updateThreadsParamIfNeeded(const std::string& arg, size_t find);


};

#endif