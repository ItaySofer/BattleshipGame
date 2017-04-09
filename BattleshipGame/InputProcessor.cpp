#include "InputProcessor.h"

 const std::string InputProcessor::attackASuffix = ".attack-a";
 const std::string InputProcessor::attackBSuffix = ".attack-b";
 const std::string InputProcessor::boardSuffix = ".sboard";

 bool InputProcessor::tryExtractFileNames()
 {
	 std::string fileNamesFilePath = "fileNames.txt";
	 std::stringstream command;
	 command << "2>NUL dir /b /a-d \"" << folderPath << "\" > " << fileNamesFilePath;
	 const std::string comandS = command.str();
	 const char* comandC = comandS.c_str();
	 if (system(comandC))
	 {
		 std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;
		 std::cout << "Wrong path: " << wrongPath << std::endl;
		 return false;
	 }

	 std::ifstream fileNamesFile = std::ifstream(fileNamesFilePath);
	 std::string line;
	 while (std::getline(fileNamesFile, line))
	 {
		 StringUtils::replaceAll(line, "\r", "");

		 std::stringstream filePath;
		 if (StringUtils::endsWith(line, InputProcessor::attackASuffix) && playerAAttackFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 playerAAttackFilePath = filePath.str();
		 }

		 if (StringUtils::endsWith(line, InputProcessor::attackBSuffix) && playerBAttackFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 playerBAttackFilePath = filePath.str();
		 }

		 if (StringUtils::endsWith(line, InputProcessor::boardSuffix) && boardFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 boardFilePath = filePath.str();
		 }
	 }

	 if (fileNamesFile.is_open()) {
		 fileNamesFile.close();
	 }

	 return true;
 }

 bool InputProcessor::validateInput()
 {

	std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;

	if (boardFilePath.empty())
	{
		std::cout << "Missing board file (*.sboard) looking in path: " << wrongPath << std::endl;
	}

	if (playerAAttackFilePath.empty())
	{
		std::cout << "Missing attack file for player A (*.attack-a) looking in path: " << wrongPath << std::endl;
	}

	if (playerBAttackFilePath.empty())
	{
		std::cout << "Missing attack file for player B (*.attack-b) looking in path: " << wrongPath << std::endl;
	}

	return !playerAAttackFilePath.empty() && !playerBAttackFilePath.empty() && !boardFilePath.empty();
}

std::string InputProcessor::getPlayerAAttackFilePath()
{
	return playerAAttackFilePath;
}

std::string InputProcessor::getPlayerBAttackFilePath()
{
	return playerBAttackFilePath;
}

std::string InputProcessor::getBoardFilePath()
{
	return boardFilePath;
}

std::string InputProcessor::concatenateAbsolutePath(const std::string& dirPath, const std::string& fileName)
{
	if (dirPath.empty()) //no dir path is given
	{
		return fileName;
	}
	if (dirPath.back() == '\\' || dirPath.back() == '/') //dir path includes '\'
	{
		return dirPath + fileName;
	}

	return dirPath + "/" + fileName;
}
