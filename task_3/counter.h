#pragma once

#include <experimental/filesystem>
#include <queue>
#include <set>

class Counter {
	struct Statistics {					// store statistic data
		size_t linesTotal = 0;
		size_t linesBlank = 0;
		size_t linesComment = 0;
		size_t linesSource = 0;
	};
	Statistics statistic;
public:
	Counter(std::experimental::filesystem::path dir, std::string outFile);
	~Counter() = default;

	void addExtention(std::string);			// add extention to std::set [extentions]
	void searchFiles();				// recursively searching decent files and store them in std::queue [files]
	void countStatistic();				// main method to count statistics
	void getStatistic();				// summarizing statistic
	std::string stat;				// all statistics
private:
	bool compareExtention(std::string ext);				// compare [ext] with any in std::set [extentions]
									// helper method count statistics for one file
	void processFile(std::experimental::filesystem::path & pathCurr, Statistics & stat);	
	static void simplify(std::string & s, bool & inMultiline);	// simplify input string 

	std::experimental::filesystem::path _dir;		// input directory
	std::string _outFile;					// output file
	std::queue<std::experimental::filesystem::path> files;	// files to handle
	bool finishedSearching;					// Counter::searchFiles() signalize finish of it tasks
	int bunchFilesToPush;					// temporarely keeping this number of files in std::queue [_files]
	int bunchFilesToProcess;				// poping from std::queue [files] this number of files to handle
	std::set<std::string> extentions;			// contain file extentions
};
