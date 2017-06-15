#include "InputProcessor.h"
#include "FileUtils.h"
#include "StringUtils.h"
#include <iostream>
#include <sstream>

InputProcessor::InputProcessor(int argc, char* argv[])
{
	std::string arg1 = argc > 1 ? argv[1] : "";
	std::string arg2 = argc > 2 ? argv[2] : "";
	std::string arg3 = argc > 3 ? argv[3] : "";
	size_t find;

	if (arg1.find(THREADS_PARAM) == std::string::npos)
	{
		folderPath = arg1;
	}
	
	find = arg1.find(THREADS_PARAM);
	if (find != std::string::npos)
	{
		updateThreadsParamIfNeeded(arg2);
	} else
	{
		find = arg2.find(THREADS_PARAM);
		if (find != std::string::npos)
		{
			updateThreadsParamIfNeeded(arg3);
		}
	}
}

void InputProcessor::updateThreadsParamIfNeeded(const std::string& arg)
{
	if (StringUtils::isNumber(arg) && std::stoi(arg) > 0)
	{
		threads = std::stoi(arg);
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

bool InputProcessor::tryExtractBoardFileNames()
{
	boardFilesPaths = FileUtils::getFilesPathsBySuffix(folderPath, boardSuffix);

	std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;
	if (boardFilesPaths.size() == 0)
	{
		std::cout << "No board files (*.sboard) looking in path: " << wrongPath << std::endl;
	}

	return boardFilesPaths.size() > 0;
}

bool InputProcessor::tryExtractDllFileNames()
{
	dllFilesPaths = FileUtils::getFilesPathsBySuffix(folderPath, dllSuffix);

	std::string wrongPath = folderPath.empty() ? "Working Directory" : folderPath;

	if (dllFilesPaths.size() < NUM_PLAYERS)
	{
		std::cout << "Missing algorithm (dll) files looking in path: " << wrongPath << " (needs at least two)" << std::endl;
	}

	return dllFilesPaths.size() >= NUM_PLAYERS;
}

