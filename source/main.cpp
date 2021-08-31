#include "DirectoryUtils.hpp"


int main(int argc, char** argv)
{
	std::string directory = argv[1];

	CopyDirectoryFiles(directory, "test");

	//std::vector<std::string> contains = CreateSupportSuffix();
	//std::vector<std::string> filenames;
	//GetFilenames(directory, filenames, contains);
	return 0;
}
