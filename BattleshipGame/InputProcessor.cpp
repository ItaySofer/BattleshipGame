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

		 if (StringUtils::endsWith(line, boardSuffix) && boardFilePath.empty())
		 {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 boardFilePath = filePath.str();
		 }

		 if (StringUtils::endsWith(line, dllSuffix)) {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 if (dllFiles[0].empty()) {
				 dllFiles[0] = filePath.str();
			 }
			 else if (dllFiles[1].empty()) {
				 dllFiles[1] = filePath.str();
			 }
		 }

		 if (StringUtils::endsWith(line, attackSuffix)) {
			 filePath << concatenateAbsolutePath(folderPath, line);
			 if (attackFiles[0].empty()) {
				 attackFiles[0] = filePath.str();
			 }
			 else if (attackFiles[1].empty()) {
				 attackFiles[1] = filePath.str();
			 }
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

	if (dllFiles[0].empty() || dllFiles[1].empty())
	{
		std::cout << "Missing an algorithm (dll) file looking in path: " << wrongPath << std::endl;
	}

	return !dllFiles[0].empty() && !dllFiles[1].empty() && !boardFilePath.empty();
}

std::string InputProcessor::getPlayerAAttackFilePath()
{
	return attackFiles[0];
}

std::string InputProcessor::getPlayerBAttackFilePath()
{
	return attackFiles[1].empty() ? attackFiles[0] : attackFiles[1];
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
