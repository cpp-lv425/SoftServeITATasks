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

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service ){	
	global_stream_lock.lock();
	std::cout << "[" << boost::this_thread::get_id() << "] Thread Start" << std::endl;
	global_stream_lock.unlock();
	io_service->run();
	global_stream_lock.lock();
	std::cout << "[" << boost::this_thread::get_id() << "] Thread Finish" << std::endl;
	global_stream_lock.unlock();
}

class Counter {
	//boost::mutex global_stream_lock;
public:
	Counter(path dir, string outFile) : _dir {dir}, _outFile {outFile}, finishedSearching {false}, bunchFilesToPush {2}
		, bunchFilesToProcess {2} {
	}
	//~Counter();
	void searchFiles() {
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
	struct Statistics {
		size_t linesTotal = 0;
		size_t linesBlank = 0;
		size_t linesComment = 0;
		size_t linesSource = 0;
	};
	void countStatistic() {
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
		//cout << "[" << boost::this_thread::get_id() << "] countStatistic thread statistics";
		//cout << "\n\tlinesTotal:" << _statistic.linesTotal << "\n\tlinesSource:" << _statistic.linesSource
		//	<< "\n\tlinesComment:" << _statistic.linesComment << "\n\tlinesBlank:" << _statistic.linesBlank
		// 	<< endl;
		statistic.linesTotal += _statistic.linesTotal;
		statistic.linesSource += _statistic.linesSource;
		statistic.linesComment += _statistic.linesComment;
		statistic.linesBlank += _statistic.linesBlank;
		global_stream_lock.unlock();
	}
	void processFile(path & pathCurr, Statistics & stat) {
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
	void simplify(string & s) {
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

	void getStatistic() {
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
	string stat;
private:
	path _dir;
	string _outFile;
	std::queue<path> files;
	bool finishedSearching;
	int bunchFilesToPush;				// temporarely keeping this number of files in [_files]
	int bunchFilesToProcess;			// poping from [files] this number of files to process
	
	Statistics statistic;
};

int main( int argc, char * argv[] ){
	path dir {argc > 1 ? argv[1] : current_path()};				//	getting initial path
        if (!exists(dir)) {
        	cout << "Path " << dir << " does not exist.\n";
        	return 1;
	}
	string outFile {argc > 2 ? argv[2] : "statistics.txt"};		//	getting output file name and # of threads
	size_t threads {argc > 3 ? (std::stoi(argv[3]) > 8) ? 8 : std::stoi(argv[3]) : boost::thread::hardware_concurrency()};
	cout << "dir:" << dir << endl;
	cout << "file:" << outFile << endl;
	cout << "threads:" << threads << endl;
	
	auto start = std::chrono::steady_clock::now();	

	boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
	boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

	std::cout << "[" << boost::this_thread::get_id() << "] main thread" << std::endl;

	boost::thread_group worker_threads;
	for( size_t x = 0; x < threads; ++x ){
		worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
	}

	Counter counter(dir, outFile);
	io_service->post( boost::bind( &Counter::searchFiles, &counter) );
	for( size_t x = 0; x < threads; ++x )
		io_service->post( boost::bind( &Counter::countStatistic, &counter) );

	work.reset();

	worker_threads.join_all();

	counter.getStatistic();

	std::ofstream file;
	file.open (outFile, std::ofstream::ate);
	if ( (file.rdstate() & std::ifstream::failbit ) != 0 ) {
		global_stream_lock.lock();
		std::cerr << "[" << boost::this_thread::get_id() << "] couldn't open file <" << outFile 
			<< ">. Skipping." << endl;
		global_stream_lock.unlock();
		return 1;
	}
	
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;

	global_stream_lock.lock();
	file << counter.stat;
	global_stream_lock.unlock();
	file << "\ntime: \t" << std::chrono::duration <double, std::milli> (diff).count() << " ms" << endl;
	file.close();

	return 0;
}
