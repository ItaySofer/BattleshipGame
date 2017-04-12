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
	~InputProcessor() = default;

	bool tryExtractFileNames();
	bool validateInput(); //Validates files exist.

	std::string getPlayerAAttackFilePath();
	std::string getPlayerBAttackFilePath();
	std::string getBoardFilePath();


private:
	const std::string attackASuffix = ".attack-a";
	const std::string attackBSuffix = ".attack-b";
	const std::string boardSuffix = ".sboard";
	static std::string concatenateAbsolutePath(const std::string& dirPath, const std::string& fileName);

};

#endif