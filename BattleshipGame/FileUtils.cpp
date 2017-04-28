#include "FileUtils.h"

bool FileUtils::isFolderPathValid(const std::string& folderPath)
{
	std::stringstream testCommand;
	testCommand << "2>NUL cd \"" << folderPath << "\"";
	const std::string testCommandS = testCommand.str();
	const char* testCommandC = testCommandS.c_str();
	return system(testCommandC) ? false : true;
}

std::vector<std::string> FileUtils::getFilesPathsBySuffix(const std::string& folderPath, const std::string& fileSuffix)
{
	std::vector<std::string> res;

	char buffer[4096];
	std::string line;
	std::stringstream command;
	command << "2>NUL dir /b /a-d \"" << folderPath << "\"";
	const std::string comandS = command.str();
	const char* comandC = comandS.c_str();
	FILE* fp = _popen(comandC, "r"); //assums folderPath is valid

	while (fgets(buffer, 4095, fp))
	{
		line = std::string(buffer);
		StringUtils::replaceAll(line, "\r", "");
		StringUtils::replaceAll(line, "\n", "");

		std::stringstream filePath;

		if (StringUtils::endsWith(line, fileSuffix))
		{
			filePath << concatenatedirPathWithFileName(folderPath, line);
			res.push_back(filePath.str());
		}
	}
	_pclose(fp);

	return res;
}

std::string FileUtils::concatenatedirPathWithFileName(const std::string& folderPath, const std::string& fileName)
{
	if (folderPath.empty()) //no dir path is given
	{
		return fileName;
	}
	if (folderPath.back() == '\\' || folderPath.back() == '/') //dir path includes '\'
	{
		return folderPath + fileName;
	}

	return folderPath + "/" + fileName;
}

std::string FileUtils::convertToAbsolutePath(const std::string& path)
{
	char fullPath[_MAX_PATH];
	if (_fullpath(fullPath, path.c_str(), _MAX_PATH) != NULL)
	{
		return fullPath;
	} else
	{
		return path;
	}
}