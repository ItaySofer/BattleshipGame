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
		return true;
	}
 }

bool InputProcessor::validateInput()
{
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

std::string InputProcessor::getPlayerADllFilePath()
{
	return dllFilesPaths[0];
}

std::string InputProcessor::getPlayerBDllFilePath()
{
	return dllFilesPaths[1];
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

std::string InputProcessor::getFolderPath()
{
	return folderPath;
}
