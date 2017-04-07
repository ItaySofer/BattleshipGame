#ifndef InputProcessorH
#define InputProcessorH
#include <string>

class InputProcessor
{
	char* path;
	
public:
	InputProcessor(char* p) : path(p) {};

	bool isInputValid(); //Validates files exist and board is OK. Outputs propper messages.

	std::string getPlayerAAttackFilePath();
	std::string getPlayerBAttackFilePath();
	std::string getBoardFilePath();
};

#endif