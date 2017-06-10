#ifndef FileUtilsH
#define FileUtilsH
#include <vector>

class FileUtils
{
public:
	static bool isFolderPathValid(const std::string& folderPath);
	static std::vector<std::string> getFilesPathsBySuffix(const std::string& folder_path, const std::string& fileSuffix);
	static std::string FileUtils::concatenatedirPathWithFileName(const std::string& dirPath, const std::string& fileName);
	static std::string FileUtils::convertToAbsolutePath(const std::string& path);
};

#endif

