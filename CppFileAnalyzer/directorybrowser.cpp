#include "directorybrowser.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

using std::cout;

void DirectoryBrowser::promptPath()
{
	std::string input;
	while (true)
	{
		input = promptString();

		initDirectory = (input.size()) ? input :
			fs::current_path();

		// converts directory separators in path to satify
		// requirement of current OS
		initDirectory.make_preferred();

		if (fs::exists(initDirectory))
		{
			cout << "\nProcessed directory: " << initDirectory.string() << '\n';
			break;
		}
		cout << "Such directory doesn't exist.\n";
		initDirectory.clear();
	}
}

void DirectoryBrowser::browseForFiles()
{
	fs::recursive_directory_iterator begin(initDirectory);
	fs::recursive_directory_iterator end;

	// enlists *.c *.h *.hpp *.cpp files
	std::copy_if(begin, end, std::back_inserter(files),
		[](const fs::path& path)
		{
			return fs::is_regular_file(path) &&
				(
				(path.extension() == ".c") ||
					(path.extension() == ".cpp") ||
					(path.extension() == ".h") ||
					(path.extension() == ".hpp")
					);
		});

	std::cout << "\nList of found files: " << std::endl;
	std::copy(files.begin(), files.end(),
		std::ostream_iterator<fs::path>(std::cout, "\n"));
	std::cout << '\n';
}

std::deque<fs::path> DirectoryBrowser::getFiles() const
{
	return files;
}

std::string DirectoryBrowser::promptString() const
{
	cout << "\nEnter project root folder in format \"Drive:/dir/subdir\"\n"
		<< "OR \nenter empty line to process current work"
		<< " directory\n>> ";

	std::string input;
	std::getline(std::cin, input);
	return input;
}
