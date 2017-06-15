#ifndef InputProcessorH
#define InputProcessorH

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
	bool tryExtractBoardFileNames();
	bool tryExtractDllFileNames();

private:
	void updateThreadsParamIfNeeded(const std::string& arg);

};

#endif