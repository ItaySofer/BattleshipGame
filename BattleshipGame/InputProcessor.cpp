#include "InputProcessor.h"

 const std::string InputProcessor::attackASuffix = ".attack-a";
 const std::string InputProcessor::attackBSuffix = ".attack-b";
 const std::string InputProcessor::boardSuffix = ".sboard";

 bool InputProcessor::tryExtractFileNames()
 {
	 std::string fileNamesFilePath = "fileNames.txt";
	 std::stringstream command;
	 command << "dir /b /a-d " << folderPath << " > " << fileNamesFilePath;
	 const std::string comandS = command.str();
	 const char* comandC = comandS.c_str();
	 if (!system(comandC))
	 {
		 std::cout << "Wrong path: " << folderPath;
		 return false;
	 }

	 std::ifstream fileNamesFile = std::ifstream(fileNamesFilePath);
	 std::string line;
	 while (fileNamesFile.good())
	 {
		 std::getline(fileNamesFile, line);
		 StringUtils::replaceAll(line, "\r", "");

		 std::stringstream filePath;
		 if (StringUtils::endsWith(line, InputProcessor::attackASuffix) && !playerAAttackFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 playerAAttackFilePath = filePath.str();
		 }

		 if (StringUtils::endsWith(line, InputProcessor::attackBSuffix) && !playerBAttackFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 playerBAttackFilePath = filePath.str();
		 }

		 if (StringUtils::endsWith(line, InputProcessor::boardSuffix) && !boardFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 boardFilePath = filePath.str();
		 }
	 }

	 return true;
 }

bool InputProcessor::validateInput()
{
	if (boardFilePath.empty())
	{
		std::cout << "Missing board file (*.sboard) looking in path: " << folderPath;
	}

	if (playerAAttackFilePath.empty())
	{
		std::cout << "Missing attack file for player A (*.attack-a) looking in path: " << folderPath;
	}

	if (playerBAttackFilePath.empty())
	{
		std::cout << "Missing attack file for player B (*.attack-b) looking in path: " << folderPath;
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
	if (dirPath.back() == '/') //dir path includes '/'
	{
		return dirPath + fileName;
	}

	return dirPath + "/" + fileName;
}
