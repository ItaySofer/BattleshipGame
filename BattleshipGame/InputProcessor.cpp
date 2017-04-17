#include "InputProcessor.h"



 bool InputProcessor::tryExtractFileNames()
 {
	 std::string fileNamesFilePath = "fileNames.txt";
	 std::stringstream command;
	 command << "2>NUL dir /b /a-d \"" << folderPath << "\" > " << fileNamesFilePath;
	 const std::string comandS = command.str();
	 const char* comandC = comandS.c_str();
	 if (system(comandC))
	 {
		 std::stringstream testCommand;
		 testCommand << "2>NUL cd \"" << folderPath << "\"";
		 const std::string testCommandS = testCommand.str();
		 const char* testCommandC = testCommandS.c_str();
		 if (system(testCommandC))
		 {
			 std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;
			 std::cout << "Wrong path: " << wrongPath << std::endl;
			 return false;
		 }
	 }

	 std::ifstream fileNamesFile(fileNamesFilePath);
	 std::string line;
	 while (std::getline(fileNamesFile, line))
	 {
		 StringUtils::replaceAll(line, "\r", "");

		 std::stringstream filePath;
		 if (StringUtils::endsWith(line, attackASuffix) && playerAAttackFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 playerAAttackFilePath = filePath.str();
		 }

		 if (StringUtils::endsWith(line, attackBSuffix) && playerBAttackFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 playerBAttackFilePath = filePath.str();
		 }

		 if (StringUtils::endsWith(line, boardSuffix) && boardFilePath.empty())
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
