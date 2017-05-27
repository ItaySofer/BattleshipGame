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
	bool boardFilesExists = tryExtractBoardFileNames();
	bool dllFilesExist = tryExtractDllFileNames();
	return boardFilesExists && dllFilesExist;
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

	return dllFilesPaths.size() < NUM_PLAYERS;
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
