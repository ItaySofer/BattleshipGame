#ifndef FileUtilsH
#define FileUtilsH
#include <string>
#include <vector>
#include <iostream>
#include "StringUtils.h"
#include <sstream>


class FileUtils
{
public:
	static bool isFolderPathValid(const std::string& folderPath);
	static std::vector<std::string> getFilesPathsBySuffix(const std::string& folder_path, const std::string& fileSuffix);
	static std::string FileUtils::concatenateAbsolutePath(const std::string& dirPath, const std::string& fileName);
};

#endif

