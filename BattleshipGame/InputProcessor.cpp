#include "InputProcessor.h"


InputProcessor::InputProcessor(int argc, char* argv[])
{

	std::string arg1 = argc > 1 ? argv[1] : "";
	std::string arg2 = argc > 2 ? argv[2] : "";
	std::string arg3 = argc > 3 ? argv[3] : "";
	size_t find;

	if (arg1.find("-delay") == std::string::npos && arg1.find("-quiet") == std::string::npos)
	{
		folderPath = arg1;
	} else
	{
		find = arg1.find("-delay");
		if (find != std::string::npos)
		{
			delayMs = std::stoi(arg1.substr(find + 6));
		}

		find = arg1.find("-quiet");
		if (find != std::string::npos)
		{
			quiet = true;
		}
	}

	find = arg2.find("-delay");
	if (find != std::string::npos)
	{
		delayMs = std::stoi(arg2.substr(find + 6));
	}

	find = arg2.find("-quiet");
	if (find != std::string::npos)
	{
		quiet = true;
	}

	find = arg3.find("-delay");
	if (find != std::string::npos)
	{
		delayMs = std::stoi(arg3.substr(find + 6));
	}

	find = arg3.find("-quiet");
	if (find != std::string::npos)
	{
		quiet = true;
	}
}

 bool InputProcessor::tryExtractFileNames()
 {
	char buffer[4096];
	std::string line;
	std::stringstream command;
	command << "2>NUL dir /b /a-d \"" << folderPath << "\"";
	const std::string comandS = command.str();
	const char* comandC = comandS.c_str();
	FILE* fp = _popen(comandC, "r");
	if (fp->_Placeholder == NULL)
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

	while (fgets(buffer, 4095, fp))
	{
	 line = std::string(buffer);
	 StringUtils::replaceAll(line, "\r", "");
	 StringUtils::replaceAll(line, "\n", "");

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
	_pclose(fp);

	 return true;
 }

bool InputProcessor::validateInput()
{
	bool boardFileExists = validateBoardFileExists();
	bool dllFilesExist = validateDllFilesExist();
	return boardFileExists && dllFilesExist;
}


bool InputProcessor::validateBoardFileExists()
{
	std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;

	if (boardFilePath.empty())
	{
		std::cout << "Missing board file (*.sboard) looking in path: " << wrongPath << std::endl;
	}

	return !boardFilePath.empty();
}

bool InputProcessor::validateDllFilesExist()
{
	std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;

	if (dllFiles[0].empty() || dllFiles[1].empty())
	{
		std::cout << "Missing an algorithm (dll) file looking in path: " << wrongPath << std::endl;
	}

	return !dllFiles[0].empty() && !dllFiles[1].empty();
}

std::string InputProcessor::getPlayerAAttackFilePath()
{
	return attackFiles[0];
}

std::string InputProcessor::getPlayerBAttackFilePath()
{
	return attackFiles[1].empty() ? attackFiles[0] : attackFiles[1];
}

std::string InputProcessor::getPlayerADllFilePath()
{
	return dllFiles[0];
}

std::string InputProcessor::getPlayerBDllFilePath()
{
	return dllFiles[1];
}

std::string InputProcessor::getBoardFilePath()
{
	return boardFilePath;
}

int InputProcessor::getDelayMs()
{
	return delayMs;
}

bool InputProcessor::getQuiet()
{
	return quiet;
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
