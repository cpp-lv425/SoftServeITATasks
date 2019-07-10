#include <boost/filesystem.hpp>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

// The function for counting the lines
void ConcurrentCodeLinesCount()
{
    // Shared global variables to store the statistics

    static std::atomic_uint64_t blankLinesCount     { 0 };
    static std::atomic_uint64_t codeLinesCount      { 0 };
    static std::atomic_uint64_t commentLinesCount   { 0 };

    cout << "CMakeCache.txt" << " " << boost::filesystem::file_size("CMakeCache.txt") << '\n';

}