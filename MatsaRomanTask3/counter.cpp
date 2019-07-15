#include "counter.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <experimental/filesystem>
#include <chrono>
#include <queue>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using namespace std::experimental::filesystem;

boost::mutex global_stream_lock;

	Counter::Counter(path dir, string outFile) : _dir {dir}, _outFile {outFile}, finishedSearching {false}, bunchFilesToPush {2}
		, bunchFilesToProcess {2} {	}

	void Counter::searchFiles() {
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id() << "] searchFiles thread" << std::endl;
		global_stream_lock.unlock();

		std::queue<path> _files;
		int n = 0;
		// cycle analizing input directory and pushing decent files to queue
		for(auto& p: std::experimental::filesystem::recursive_directory_iterator(_dir)) {
			if (std::experimental::filesystem::is_regular_file(p) &&
				(path(p).extension() == ".c" || path(p).extension() == ".cpp" ||
				 path(p).extension() == ".h" || path(p).extension() == ".hpp") )
        		{
				_files.push(p);
				++n;
				if (n == bunchFilesToPush) {
					global_stream_lock.lock();	// relocate files from [_files] to [files]
					for (size_t i = 0; i < bunchFilesToPush; ++i) {
						files.push(_files.front());
						cout << "[" << boost::this_thread::get_id() << "] ";
						cout << "pushed file " << _files.front() << endl;
						_files.pop();
					}
					n = 0;
					global_stream_lock.unlock();
				}
			}
		}
		if (_files.size() != 0) {				// relocate rest files from [_files] to [files]
			global_stream_lock.lock();	
			while (_files.size() != 0) {
				files.push(_files.front());
				_files.pop();
			}
			global_stream_lock.unlock();
		}
		global_stream_lock.lock();
		finishedSearching = true;
		/*cout << "[" << boost::this_thread::get_id() << "] searchFiles thread";
		cout << "files.size() " << files.size() << ", finishedSearching:" << finishedSearching << endl;*/
		global_stream_lock.unlock();
	}
	
	void Counter::countStatistic() {
		global_stream_lock.lock();
		cout << "[" << boost::this_thread::get_id() << "] countStatistic thread" << endl;
		global_stream_lock.unlock();
		
		std::queue<path> _files;	
		Statistics _statistic;

		while (!finishedSearching /*&& files.size()*/) {
			/*global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id() << "] countStatistic thread in while";
			cout << ", files.size() " << files.size() << ", finishedSearching:" << finishedSearching << endl;//getchar();
			global_stream_lock.unlock();*/
			if (files.size() > 0) {				// relocate files from queue to process
				global_stream_lock.lock();
				int bunch {(int)files.size() > bunchFilesToProcess ? bunchFilesToProcess : (int)files.size()};
				for (size_t i = 0; i < bunch; ++i) {
					_files.push(files.front());
					files.pop();
					std::cout << "[" << boost::this_thread::get_id() << "] ";
					cout << "popped file to process " << _files.back() << endl;
				}
				global_stream_lock.unlock();
				
				while (_files.size()) {
					path pathCurr = _files.front();
					_files.pop();
					processFile(pathCurr, _statistic);
					//cout << "current file <" << _files.back() << endl;
				}
			}
		}
		global_stream_lock.lock();
		statistic.linesTotal += _statistic.linesTotal;
		statistic.linesSource += _statistic.linesSource;
		statistic.linesComment += _statistic.linesComment;
		statistic.linesBlank += _statistic.linesBlank;
		global_stream_lock.unlock();
	}
	void Counter::simplify(string & s) {
		//cout << "string in:" << s << ":" << endl;
		
		// delete quoted "text" and 'char'
		std::size_t found1 = s.find_first_of("\"");
		std::size_t found2 = s.find_first_of("\"", found1 + 1);
		while (found1 != std::string::npos || found2 != std::string::npos) {
			s.erase (found1, found2 - found1 + 1);
			found1 = s.find_first_of("\"");
			found2 = s.find_first_of("\"", found1 + 1);
	  	}
		found1 = s.find_first_of("'");
		found2 = s.find_first_of("'", found1 + 1);
		while (found1 != std::string::npos || found2 != std::string::npos) {
			s.erase (found1, found2 - found1 + 1);
			found1 = s.find_first_of("'");
			found2 = s.find_first_of("'", found1 + 1);
	  	}

		// delete 1-lined cpp-style  comments
		found1 = 0;
		found2 = 0;
		while (true) {
			found1 = s.find("/*", found2);
			if (found1 != std::string::npos) {
				found2 = s.find("*/", found1 + 1);
				if (found2 != std::string::npos)
					s.erase (found1 + 2, found2 - found1 - 2);
			} else
				break;
			
		}
		
		// handle empty line
		s.erase (remove_if(s.begin(), s.end(), isspace), s.end());
		if (!s.size()) {
			s = "b";
			return;
		}
		found1 = s.find("/*");
		found2 = s.find("*/");
		std::size_t found3 = s.find("//");							// return if only code
		if (found1 == std::string::npos && found2 == std::string::npos && found3 == std::string::npos){	// or only comment
			s = "u";
			return;
		}

		if (found1 == std::string::npos && found2 == std::string::npos && found3 != std::string::npos){	// return 
			s.erase (found3, s.size() - found3);							// if only C-style com
			s.erase (remove_if(s.begin(), s.end(), isspace), s.end());
			if (s.size())
				s = "u//";
			else
				s = "//";
			return;
		}

		if (found1 != std::string::npos && found2 != std::string::npos && found3 != std::string::npos)
			if (found1 < found3 || found2 < found3)	
				s.erase (found3, s.size() - found3);
			else {	
				s.erase (found3, s.size() - found3);
				s = s.size() > 2 ? "u//" : "//";
			}

		if (found1 != std::string::npos && found2 == std::string::npos && found3 != std::string::npos){
			if (found1 < found3)
				s.erase (found3, s.size() - found3);
			else {
				s.erase (found3, s.size() - found3);
				s = s.size() > 2 ? "u//" : "//";
			}
		}
		
		if (found1 == std::string::npos && found2 != std::string::npos && found3 != std::string::npos){
			if (found2 < found3)
				s.erase (found3, s.size() - found3);
			else {
				s.erase (found3, s.size() - found3);
				s = s.size() > 2 ? "u//" : "//";
			}
		}

		s.erase (remove_if(s.begin(), s.end(), isspace), s.end());
		if (!s.size())
			return;

		// check if multiline and count number off comment blocks
		int n1 = 0;
		int n2 = 0;
		std::size_t found11;

		found1 = s.find("/*");
		found2 = s.find("*/");
		bool nextLineOfMultilineComment {found1 > found2 ? true : false};

		while (found1 != std::string::npos) {
			++n1;
			found11 = found1;
			found1 = s.find("/*", found1 + 2);
		}

		while (found2 != std::string::npos) {
			++n2;
			found2 = s.find("*/", found2 + 2);
		}

		if (!nextLineOfMultilineComment) {
			bool multiline {n1 != n2 ? true : false};	
			if (multiline)	{	
				s.erase (found11 + 2, s.size() - found11 - 2);	// erase last comment after /*
				found1 = s.find_first_not_of("/*");
				if (found1 != std::string::npos)
					s = "c/*";
				else
					s = "/*";
			} else {
				found1 = s.find_first_not_of("/*");
				if (found1 != std::string::npos)
					s = "c//";
				else
					s = "//";
			}
		} else {
			bool multiline {n1 == n2 ? true : false};
			if (multiline)	{
				s.erase (found11 + 2, s.size() - found11 - 2);	// erase last comment after /*
				found1 = s.find_first_not_of("/*");
				if (found1 != std::string::npos)
					s = "c/*";
				else
					s = "/*";
			} else {
				found1 = s.find_first_not_of("/*");
				if (found1 != std::string::npos)
					s = "c//";
				else
					s = "//";
			}
		}
		//cout << "string out:" << s << ":" << endl;
	}
	void Counter::processFile(path & pathCurr, Statistics & stat) {
		std::ifstream file;
		file.open (pathCurr, std::ofstream::out);
		if ( (file.rdstate() & std::ifstream::failbit ) != 0 ) {
			global_stream_lock.lock();
			std::cerr << "[" << boost::this_thread::get_id() << "] couldn't open file <" << pathCurr 
				<< ">. Skipping." << endl;
			global_stream_lock.unlock();
			return;
		}

		string s;
		bool inMultiline = false;
		for (size_t line {1}; getline(file, s); ++line) {
			//global_stream_lock.lock();cout << "string read:" << s << ":." << endl;global_stream_lock.unlock();
			simplify(s);
			//global_stream_lock.lock();cout << "string simplify:" << s << ":." << endl;global_stream_lock.unlock();

			++stat.linesTotal;
			
			if (s.compare("b")) {
				++stat.linesBlank;
				continue;
			}
			if (s.compare("c//")) {
				++stat.linesComment;
				++stat.linesSource;
				inMultiline = false;
				continue;
			}
			if (s.compare("//")) {
				++stat.linesComment;
				inMultiline = false;
				continue;
			}
			if (s.compare("u"))
				if (!inMultiline) {
					++stat.linesSource;
					continue;
				} else {
					++stat.linesComment;
					continue;
				}
			if (s.compare("c/*")) {
				inMultiline = true;
				++stat.linesSource;
				++stat.linesComment;
			}
			if (s.compare("/*")) {
				inMultiline = true;
				++stat.linesComment;
			}
			if (s.compare("u//")) {
				if (!inMultiline) {
					++stat.linesSource;
					++stat.linesComment;
					continue;
				} else {
					++stat.linesComment;
					continue;
				}
			}
		}
		file.close();
	}

	void Counter::getStatistic() {
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id() << "] getStatistic thread" << std::endl;
		/*cout << "\tlinesTotal:" << statistic.linesTotal << "\n\tlinesSource:" << statistic.linesSource
			<< "\n\tlinesComment:" << statistic.linesComment << "\n\tlinesBlank:" << statistic.linesBlank
		 	<< endl;*/
		global_stream_lock.unlock();
	
		stat = string{"Total lines: \t" + std::to_string(statistic.linesTotal) +
			"\nSource lines:\t" + std::to_string(statistic.linesSource) +
			"\nComment lines:\t" + std::to_string(statistic.linesComment) +
			"\nBlank lines: \t" + std::to_string(statistic.linesBlank) };
	}
