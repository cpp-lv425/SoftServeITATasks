#ifndef CPPFILEANALYZER_H
#define CPPFILEANALYZER_H

#include "threadspool.h"

#include <filesystem>
#include <deque>

#include "directorybrowser.h"

namespace fs = std::filesystem;
namespace ch = std::chrono;

class CppFileAnalyzer
{	
	std::deque< fs::path> files;
	std::mutex mtx;
	std::atomic<int> codeLinesNum;
	std::atomic<int> blankLinesNum;
	std::atomic<int> filesNum;
	std::atomic<int> commLinesNum;	
	ch::time_point<ch::steady_clock> startTime;	
	ch::duration<double> duration;	
	DirectoryBrowser dirBrowser;

	enum class TextType
	{
		Unknown,
		BlankLine,
		CStyleComment,
		CppStyleComment,
		Code,
		CodeAndComment
	};

public:	
	void promptDirectory();
	// prompts directory from user	
	void start();

private:		
	// creates threads pool & adds tasks to tasks queue
	void allocateWork();	
	// processes single file
	void processFile();
	// processes test from single file
	void textAnalyzer(std::vector<std::pair<TextType, 
		std::string>>& text);
	// processes CStyleComment text blocks
	void processCStyleComment(std::vector
		<std::pair<TextType, std::string>>& text,
		int& i, int pos = 2);
	// processes text put in quotes
	void processQuotes(std::vector<std::pair<TextType,
		std::string>>&text, int& i);
	// prints results to console
	void outputResults();
	void writeResultsToFile()const;
};

#endif // !CPPFILEANALYZER_H



