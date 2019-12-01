#include "FileDialog.hpp"
#include "tinyfiledialogs.h"


std::string FileDialog::chooseFile(const std::vector<const char*>& fileFilterPatterns)
{
	return chooseFile("Choose File", fileFilterPatterns);
}

std::string FileDialog::chooseFile(std::string_view title, const std::vector<const char*>& fileFilterPatterns)
{
	return valOrEmpty(tinyfd_openFileDialog(
		title.data(), "", fileFilterPatterns.size(),
		fileFilterPatterns.data(),
		nullptr, 0));
}

std::string FileDialog::saveFile(std::string_view defaultFileName, const std::vector<const char*>& fileFilterPatterns)
{
	return saveFile("Save File", defaultFileName, fileFilterPatterns);
}

std::string FileDialog::saveFile(std::string_view title, std::string_view defaultFileName, const std::vector<const char*>& fileFilterPatterns)
{
	return valOrEmpty(tinyfd_saveFileDialog(
		title.data(), defaultFileName.data(),
		fileFilterPatterns.size(),
		fileFilterPatterns.data(),
		nullptr));
}

const char* FileDialog::valOrEmpty(const char* s) 
{
	return s == nullptr ? "" : s;
}