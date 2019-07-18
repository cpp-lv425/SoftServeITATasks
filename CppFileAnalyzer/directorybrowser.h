#ifndef DIRECTORY_BROWSER_H
#define DIRECTORY_BROWSER_H

#include <filesystem>
#include <deque>
namespace fs = std::filesystem;

class DirectoryBrowser
{
	fs::path initDirectory;
	std::deque<fs::path> files;
public:	
	// prompts directory from user
	void promptPath();
	// iterates through dir & subdirs looking for 
	// *.c *.cpp *.h *.hpp files
	void browseForFiles();	
	std::deque<fs::path> getFiles()const;
private:
	// prompts string from user
	std::string promptString()const;		
};

#endif // !DIRECTORY_BROWSER_H


