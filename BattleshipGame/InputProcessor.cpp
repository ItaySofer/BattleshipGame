#include "InputProcessor.h"
#include "FileUtils.h"


InputProcessor::InputProcessor(int argc, char* argv[])
{
	std::string arg1 = argc > 1 ? argv[1] : "";
	std::string arg2 = argc > 2 ? argv[2] : "";
	std::string arg3 = argc > 3 ? argv[3] : "";
	size_t find;

	if (arg1.find("-delay") == std::string::npos && arg1.find("-quiet") == std::string::npos)
	{
		folderPath = arg1;
	} 

	std::vector<std::string> args = { arg1, arg2, arg3 };
	for (auto arg : args)
	{
		find = arg.find("-delay");
		if (find != std::string::npos)
		{
			updateDelayParamIfNeeded(arg, find);
		}

		find = arg.find("-quiet");
		if (find != std::string::npos)
		{
			quiet = true;
		}
	}
}

void InputProcessor::updateDelayParamIfNeeded(const std::string& arg, size_t find)
{
	std::string str = arg.substr(find + 6);
	StringUtils::replaceAll(str, " ", "");
	if (StringUtils::isNumber(str))
	{
		delayMs = std::stoi(str);
	}
}

 bool InputProcessor::validateFolderPath()
 {
	if (!FileUtils::isFolderPathValid(folderPath))
	{
		std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;
		std::cout << "Wrong path: " << wrongPath << std::endl;
		return false;
	}
	else
	{
		folderPath = FileUtils::convertToAbsolutePath(folderPath);
		return true;
	}
 }

bool InputProcessor::validateInput()
{
	validateFolderPath();
	bool boardFileExists = tryExtractBoardFileName();
	bool dllFilesExist = tryExtractDllFileNames();
	return boardFileExists && dllFilesExist;
}

bool InputProcessor::tryExtractBoardFileName()
{
	std::vector<std::string> boardFiles = FileUtils::getFilesPathsBySuffix(folderPath, boardSuffix);
	boardFilePath = boardFiles.size() > 0 ? boardFiles[0] : "";

	std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;

	if (boardFilePath.empty())
	{
		std::cout << "Missing board file (*.sboard) looking in path: " << wrongPath << std::endl;
	}

	return !boardFilePath.empty();
}

bool InputProcessor::tryExtractDllFileNames()
{
	std::vector<std::string> dllFiles = FileUtils::getFilesPathsBySuffix(folderPath, dllSuffix);
	dllFilesPaths[0] = dllFiles.size() > 0 ? dllFiles[0] : "";
	dllFilesPaths[1] = dllFiles.size() > 1 ? dllFiles[1] : "";

	std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;

	if (dllFilesPaths[0].empty() || dllFilesPaths[1].empty())
	{
		std::cout << "Missing an algorithm (dll) file looking in path: " << wrongPath << std::endl;
	}

	return !dllFilesPaths[0].empty() && !dllFilesPaths[1].empty();
}

std::string InputProcessor::getPlayerADllFilePath() const
{
	return dllFilesPaths[0];
}

std::string InputProcessor::getPlayerBDllFilePath() const
{
	return dllFilesPaths[1];
}

std::string InputProcessor::getBoardFilePath() const
{
	return boardFilePath;
}

int InputProcessor::getDelayMs() const
{
	return delayMs;
}

bool InputProcessor::getQuiet() const
{
	return quiet;
}

std::string InputProcessor::getFolderPath() const
{
	return folderPath;
}
