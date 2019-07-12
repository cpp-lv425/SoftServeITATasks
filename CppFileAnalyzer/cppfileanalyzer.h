#ifndef CPPFILEANALYZER_H
#define CPPFILEANALYZER_H

#include "threadspool.h"

#include <deque>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
namespace ch = std::chrono;

class CppFileAnalyzer
{
	fs::path initDirectory;		
	std::deque< fs::path> files;
	std::mutex mtx;
	std::atomic<int> codeLinesNum;
	std::atomic<int> blankLinesNum;
	std::atomic<int> filesNum;
	std::atomic<int> commLinesNum;	
	ch::time_point<ch::steady_clock> startTime;	
	ch::duration<double> duration;		

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
	CppFileAnalyzer();	
	// prompts directory from user
	void promptPath();
	// iterates through dir & subdirs looking form *.c *.cpp *.h *.hpp files
	void browseForFiles();
	void start();

private:	
	// prompts string from user
	std::string promptString()const;
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



