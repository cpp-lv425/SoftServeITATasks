#include "counter.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <experimental/filesystem>
#include <chrono>
#include <fstream>
#include <string>

using std::string;
using std::cout;
using std::endl;
using namespace std::experimental::filesystem;

//#define DEBUG									//	print debugging info

extern boost::mutex global_stream_lock;
void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service );

int main( int argc, char * argv[] ) {
	path dir {argc > 1 ? argv[1] : current_path()};				//	getting initial path
        if (!exists(dir)) {
        	cout << "Path " << dir << " does not exist.\n";
        	return 1;
	}
	string outFile {argc > 2 ? argv[2] : "statistics.txt"};			//	getting output file name and # of threads
	size_t threads {argc > 3 ? (std::stoi(argv[3]) > 8) ? 8 : std::stoi(argv[3]) : boost::thread::hardware_concurrency()};
	
	if (argc == 1) {
		cout << "Usage: ./run <initial_directory> <outfile> <#_threads>\n";
		cout << "You runnig with default parameters:\n";
		cout << "\tdir: " << dir << endl;
		cout << "\tfile: ./" << outFile << endl;
		cout << "\tthreads: " << threads << "\tcontinue? y/n: "; 
		char character = getchar();
		if (character != 'y')
			return 0;
	}

	#ifdef DEBUG	
		cout << "dir:" << dir << endl;
		cout << "file:" << outFile << endl;
		cout << "threads:" << threads << endl;
	#endif

	auto start = std::chrono::steady_clock::now();	

	boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
	boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

	#ifdef DEBUG
		std::cout << "[" << boost::this_thread::get_id() << "] main thread" << std::endl;
	#endif
	boost::thread_group worker_threads;
	for( size_t x = 0; x < threads; ++x ){
		worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
	}

	Counter counter(dir, outFile);

	counter.addExtention(".h");
	counter.addExtention(".hpp");
	counter.addExtention(".c");
	counter.addExtention(".cpp");

	io_service->post( boost::bind( &Counter::searchFiles, &counter) );
	for( size_t x = 0; x < threads; ++x )
		io_service->post( boost::bind( &Counter::countStatistic, &counter) );

	work.reset();

	worker_threads.join_all();

	counter.getStatistic();

	std::ofstream file;
	file.open (outFile, std::ofstream::out);
	if ( (file.rdstate() & std::ifstream::failbit ) != 0 ) {
		global_stream_lock.lock();
		std::cerr << "[" << boost::this_thread::get_id() << "] couldn't open file <" << outFile 
			<< ">. Skipping." << endl;
		global_stream_lock.unlock();
		return 1;
	}
	
	auto end = std::chrono::steady_clock::now();
	auto execMillis = std::chrono::duration <double, std::milli> (end - start);

	global_stream_lock.lock();
	file << counter.stat;
	global_stream_lock.unlock();
	file << "\nexecution time: " << execMillis.count() << " ms" << endl;
	file.close();

	return 0;
}

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service ){
	#ifdef DEBUG	
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id() << "] Thread Start" << std::endl;
		global_stream_lock.unlock();
	#endif
	io_service->run();
	#ifdef DEBUG
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id() << "] Thread Finish" << std::endl;
		global_stream_lock.unlock();
	#endif
}
