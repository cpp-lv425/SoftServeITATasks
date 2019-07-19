#include "counter.h"

#include <iostream>
#include <boost/thread/thread.hpp>
#include <experimental/filesystem>
#include <queue>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using namespace std::experimental::filesystem;

//#define DEBUG_THREADS		//	print id of current thread
//#define DEBUG_STRINGS		//	print info about current string
//#define DEBUG_FILES		//	print info about current file

#define BUNCH_FILES_TO_PUSH 2	//	Counter::searchFiles() store this # of files before push them to std::queue<path> files
#define BUNCH_FILES_TO_PROCESS 2//	Counter::countStatistic() pop this # of files from std::queue<path> files to handle

boost::mutex global_stream_lock;

	Counter::Counter(path dir, string outFile) : _dir {dir}, _outFile {outFile}, finishedSearching {false}
		, bunchFilesToPush {BUNCH_FILES_TO_PUSH}, bunchFilesToProcess {BUNCH_FILES_TO_PROCESS} {	}

	void Counter::addExtention(std::string extention) {
		extentions.insert(extention);
	}

	void Counter::searchFiles() {
		#ifdef DEBUG_THREADS
			global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id() << "] searchFiles thread" << std::endl;
			global_stream_lock.unlock();
		#endif

		std::queue<path> _files;	// temporarely keep this # of files before relocate to global std::queue<path> files
		int n = 0;			// number files in [_files]
		// cycle recursively analizing input directory and pushing decent files to [_files]
		for(auto& p: std::experimental::filesystem::recursive_directory_iterator(_dir)) {
			if (std::experimental::filesystem::is_regular_file(p) && 
				extentions.find(path(p).extension()) != extentions.end() )
        		{
				_files.push(p);
				++n;
				if (n == bunchFilesToPush) {
					global_stream_lock.lock();	// relocate files from local [_files] to global [files]
					for (size_t i = 0; i < bunchFilesToPush; ++i) {
						files.push(_files.front());
						#ifdef DEBUG_FILES
							cout << "[" << boost::this_thread::get_id() << "] ";
							cout << "pushed file " << _files.front() << endl;
						#endif
						_files.pop();
					}
					n = 0;
					global_stream_lock.unlock();
				}
			}
		}
		if (_files.size() != 0) {				// relocate rest files from local [_files] to global [files]
			global_stream_lock.lock();	
			while (_files.size() != 0) {
				files.push(_files.front());
				_files.pop();
			}
			global_stream_lock.unlock();
		}
		global_stream_lock.lock();
		finishedSearching = true;				// flag signalize finish searching of decent files
		global_stream_lock.unlock();
	}
	
	void Counter::countStatistic() {
		#ifdef DEBUG_THREADS
			global_stream_lock.lock();
			cout << "[" << boost::this_thread::get_id() << "] countStatistic thread" << endl;
			global_stream_lock.unlock();
		#endif
		global_stream_lock.lock();
			bool _finishedSearching = finishedSearching;
			size_t filesSize = files.size();
		global_stream_lock.unlock();
		
		std::queue<path> _files;	
		Statistics _statistic;
		
		while (!_finishedSearching || filesSize) {
			if (filesSize > 0) {			// relocate files from global queue [files] to local queue [_files]
				global_stream_lock.lock();
				int bunch {(int)files.size() > bunchFilesToProcess ? bunchFilesToProcess : (int)files.size()};
				for (size_t i = 0; i < bunch; ++i) {
					_files.push(files.front());
					files.pop();
					#ifdef DEBUG_FILES
						std::cout << "[" << boost::this_thread::get_id() << "] ";
						cout << "popped file to handle " << _files.back() << endl;
					#endif
				}
				global_stream_lock.unlock();
				
				while (_files.size()) {		// handeling all files from local queue [_files]
					path pathCurr = _files.front();
					_files.pop();
					processFile(pathCurr, _statistic);
				}
			}
			global_stream_lock.lock();
				_finishedSearching = finishedSearching;
				filesSize = files.size();
			global_stream_lock.unlock();
		}

		global_stream_lock.lock();	// add statistic info from current thread to global struct Statistic
			statistic.linesTotal += _statistic.linesTotal;
			statistic.linesSource += _statistic.linesSource;
			statistic.linesComment += _statistic.linesComment;
			statistic.linesBlank += _statistic.linesBlank;
		global_stream_lock.unlock();
	}
	void Counter::simplify(string & s, bool & inMultiline) {
		#ifdef DEBUG_STRINGS
		cout << "string in simplify:" << s << ":" << endl;
		#endif

		// delete quoted "text" and 'char'			case [1]	
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
		
		// delete 1-lined cpp-style  comments			case [2]	
		found1 = 0;
		found2 = 0;
		while (true) {
			found1 = s.find("/*", found2);
			if (found1 != std::string::npos) {
				found2 = s.find("*/", found1 + 1);
				if (found2 != std::string::npos) {
					s.erase (found1 + 2, found2 - found1 - 2);
						found1 += 2;
						found2 = found1;
					}
			} else
				break;
			
		}
		
		// handle empty line					case [3]	
		s.erase (remove_if(s.begin(), s.end(), isspace), s.end());
		if (!s.size()) {
			s = (inMultiline ? "bs" : "b");
			#ifdef DEBUG_STRINGS
			cout << "case 3" << endl;
			#endif
			return;
		}
		
		// handle situation when only code or unly comment. Both depend on prev line	 case [4] 
		found1 = s.find("/*");
		found2 = s.find("*/");
		std::size_t found3 = s.find("//");							
		if (found1 == std::string::npos && found2 == std::string::npos && found3 == std::string::npos){	
			s = (inMultiline ? "s" : "c");
			#ifdef DEBUG_STRINGS
			cout << "case 4" << endl;
			#endif
			return;
		}
		
		// handle situation when only // comment sign in line	[case 5]		
		if (found1 == std::string::npos && found2 == std::string::npos && found3 != std::string::npos){	 
			s.erase (found3, s.size() - found3);	
			if (inMultiline)
				s = "s";
			else
				s = (s.size() ? "cs" : "s");
			#ifdef DEBUG_STRINGS	
			cout << "case 5" << endl;	
			#endif
			return;
		}
		
		#ifdef DEBUG_STRINGS
		cout << "string filtered:" << s << ":" << endl;
		#endif
		
		if (!inMultiline) {	
			std::size_t found31 = s.find("*///");				//      [case 12a]
			if (found31 != std::string::npos) {
				#ifdef DEBUG_STRINGS
				cout << "case 12a" << endl;
				#endif
				s.erase (found31 + 2, s.size() - found31 - 2);
			}

			// handle situation when // comment sign and other (/* or */ or both)	[case 12]
			found31 = s.find("//");
			if (found31 != std::string::npos) {
				#ifdef DEBUG_STRINGS
				cout << "case 12" << endl;
				#endif
				s.erase (found31, s.size() - found31);
			}
		}

		int n1 = 0;		// number of /* in row
		int n2 = 0;		// number of */ in row
		std::size_t found11;	// last /* in row
		std::size_t found12;	// last */ in row
		std::size_t found1_0;	// first /* in row or npos
		std::size_t found2_0;	// first */ in row or npos

		// counting number of /* and */
		found1   = s.find("/*");
		found1_0 = found1;
		found2   = s.find("*/");
		found2_0 = found2;
		while (found1 != std::string::npos) {
			++n1;
			found11 = found1;
			found1 = s.find("/*", found1 + 2);
		}

		while (found2 != std::string::npos) {
			++n2;
			found12 = found2;
			found2 = s.find("*/", found2 + 2);
		}

		// handle all situations in !inMultiline mode
		if (!inMultiline) {	//cout << "!inMultiline" << endl;
			if (n1 == n2) {// stay in !inMultiline - case 13
				found1 = s.find_first_not_of("/*");
				if (found1 != std::string::npos)
					s = "cs";
				else
					s = "s";
				#ifdef DEBUG_STRINGS
				cout << "case 13" << endl;
				#endif
			} else {	// change to inMultiline - case 14
				inMultiline = true;
				s.erase (found11 + 2, s.size() - found11 - 2);	// erase last comment after /*
				found1 = s.find_first_not_of("/*");
				if (found1 != std::string::npos)
					s = "cs";
				else
					s = "s";
				#ifdef DEBUG_STRINGS
				cout << "case 14 s:" << s << ": return" << endl;
				#endif
			}
		} else { 		
			#ifdef DEBUG_STRINGS
			cout << "inMultiline" << endl;
			#endif
			if (n1 != n2) {// 					 	case 20
				if (found2_0 < found1_0) {// change to !inMultiline 	case 21
					s.erase (0, found2_0);	// erase all before */
					inMultiline = false;
					found1 = s.find_first_not_of("/*");
					if (found1 != std::string::npos)
						s = "cs";
					else
						s = "s";
					#ifdef DEBUG_STRINGS
					cout << "case 21" << endl;
					#endif
				} else {	// stay inMultiline 		case 22
					found1 = s.find_first_not_of("/*");
					if (found1 != std::string::npos)
						s = "cs";
					else
						s = "s";
					#ifdef DEBUG_STRINGS
					cout << "case 22" << endl;
					#endif
				}
			} else {				// case 30
				if (found2_0 < found1_0) { 	// stay inMultiline 	case 31
					s.erase (found11 + 2, s.size() - found11 - 2);	// erase comment after /*
					s.erase (0, found2_0);	// erase comment before */
					found1 = s.find_first_not_of("/*");
					if (found1 != std::string::npos)
						s = "cs";
					else
						s = "s";
					#ifdef DEBUG_STRINGS
					cout << "case 31" << endl;
					#endif
				} else {		// change to !inMultiline case 32
					inMultiline = false;
					found1 = s.find_first_not_of("/*");
					if (found1 != std::string::npos)
						s = "cs";
					else
						s = "s";
					#ifdef DEBUG_STRINGS
					cout << "case 32" << endl;
					#endif
				}
			}
			
		}
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
			simplify(s, inMultiline);

			++stat.linesTotal;
			
			if (!s.compare("b")) {
				++stat.linesBlank;	
				continue;
			}
			if (!s.compare("bs")) {
				++stat.linesBlank;
				++stat.linesComment;	
				continue;
			}
			if (!s.compare("s")) {
				++stat.linesComment;	
				continue;
			}
			if (!s.compare("c")) {
				++stat.linesSource;	
				continue;
			}
			if (!s.compare("cs")) {
				++stat.linesSource;	
				++stat.linesComment;
			}
		}

		file.close();
	}

	void Counter::getStatistic() {
		#ifdef DEBUG_THREADS
			global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id() << "] getStatistic thread" << std::endl;
			global_stream_lock.unlock();
		#endif
	
		stat = string{"Total lines: \t" + std::to_string(statistic.linesTotal) +
			"\nSource lines:\t" + std::to_string(statistic.linesSource) +
			"\nComment lines:\t" + std::to_string(statistic.linesComment) +
			"\nBlank lines: \t" + std::to_string(statistic.linesBlank) };
	}
