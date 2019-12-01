#pragma once
#include <string>
#include <vector>
#include <memory>

class FileDialog {
public:
	static std::string chooseFile(const std::vector<const char*>& fileFilterPatterns = { "*" });
	static std::string chooseFile(std::string_view title, const std::vector<const char*>& fileFilterPatterns = { "*" });

	static std::string saveFile(std::string_view defaultFileName, const std::vector<const char*>& fileFilterPatterns = { "*" });
	static std::string saveFile(std::string_view title, std::string_view defaultFileName, const std::vector<const char*>& fileFilterPatterns = { "*" });
private:
	static const char* valOrEmpty(const char* s);
};