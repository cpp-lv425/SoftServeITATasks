#include"FilesDirectory.h"

vector<fs::path> ListFilesInDirectory(const fs::path& dir)// get list of files we'll parse
{
	vector<fs::path> files;
	if (fs::exists(dir) && fs::is_directory(dir))// while we have files in the directory
	{
		for (const auto& entry : fs::recursive_directory_iterator(dir))// move recoursive throught all directories
		{
			if(PropriateExtension(entry))// if our condition is ok (.cpp, .h. etc)
				files.emplace_back(entry.path());//add this path
		}
	}
	return files;
}

bool PropriateExtension(fs::path path)//condition
{
	vector<fs::path>extension = { ".cpp",".c",".hpp",".h" };
	for (auto &i : extension)
	{
		if (path.extension() == i)
			return true;
	}
	return false;
}


std::string InputPath()
{
	std::string path;
	std::cout << "Input full path: ";
	std::cin >> path;
	return path;
}

void PrintResultInTheFile(std::queue<CodeFile> codeFileQ, std::chrono::microseconds runningDuration)// sum the amount of any type, create file, show it
{
	struct LinesCount
	{
		int m_processedCount = 0;
		int m_blankCount = 0;
		int m_commentCount = 0;
		int m_physicalCount = 0;

		LinesCount& operator +=(CodeFile &codeFileObj)
		{
			this->m_blankCount += codeFileObj.GetBlankCount();
			this->m_commentCount += codeFileObj.GetCommentCount();
			this->m_physicalCount += codeFileObj.GetPhysicalCount();
			this->m_processedCount += codeFileObj.GetProcessedCount();
			return *this;
		}
	}linesCount;
	
	int fileParsed = codeFileQ.size();

	while (!codeFileQ.empty())
	{
		linesCount += codeFileQ.front();
		codeFileQ.pop();
	}
	std::ofstream outline("Result.txt");
	outline << "An Amount of total parsed files = " << fileParsed << "\n"
		<< "An Amount of total processed lines = " << linesCount.m_processedCount << "\n"
		<< "An Amount of total physical lines = " << linesCount.m_physicalCount << "\n"
		<< "An Amount of total comment lines = " << linesCount.m_commentCount << "\n"
		<< "An Amount of total blank lines = " << linesCount.m_blankCount << "\n"
		<< "Time of running programm =" << runningDuration.count() / 1000000.0 << " seconds\n";
	outline.close();
	system("Result.txt");
}