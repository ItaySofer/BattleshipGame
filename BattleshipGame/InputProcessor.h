#ifndef InputProcessorH
#define InputProcessorH
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "StringUtils.h"

class InputProcessor
{
	std::string folderPath;
	std::string playerAAttackFilePath = "";
	std::string playerBAttackFilePath = "";
	std::string boardFilePath = "";

	
public:
	InputProcessor(std::string p) : folderPath(p) {};

	bool tryExtractFileNames();
	bool validateInput(); //Validates files exist and board is OK. Outputs propper messages.

	std::string getPlayerAAttackFilePath();
	std::string getPlayerBAttackFilePath();
	std::string getBoardFilePath();


private:
	static const std::string attackASuffix;
	static const std::string attackBSuffix;
	static const std::string boardSuffix;
	static std::string concatenateAbsolutePath(const std::string& dirPath, const std::string& fileName);

};

#endif