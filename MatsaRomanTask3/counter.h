#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <experimental/filesystem>
#include <chrono>
#include <queue>
#include <fstream>
using std::string;
using namespace std::experimental::filesystem;

class Counter {
	
public:
	Counter(path dir, string outFile);
	~Counter() = default;

	struct Statistics {					// store statistic data
		size_t linesTotal = 0;
		size_t linesBlank = 0;
		size_t linesComment = 0;
		size_t linesSource = 0;
	};

	void searchFiles();					// recursively searching decent files and store it in [stat]
	void countStatistic();					// main method to count statistics
	void processFile(path & pathCurr, Statistics & stat);	// helper method counting statistics for one file
	static void simplify(string & s);			// simplifying input string 

	void getStatistic();					// summarizing statistic
	string stat;						// all statistics
private:
	path _dir;						// input directory
	string _outFile;					// output file
	std::queue<path> files;					// files to handle
	bool finishedSearching;					// Counter::searchFiles() signalize finish of it tasks
	int bunchFilesToPush;					// temporarely keeping this number of files in std::queue [_files]
	int bunchFilesToProcess;				// poping from std::queue [files] this number of files to handle
	
	Statistics statistic;
};
