#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <regex>

// Names of the frequently used types
typedef boost::uint64_t         CounterType;
typedef boost::atomic_uint64_t  ConcurrentCounterType;

class ConcurrentLineCounter {
    // Variable to store the current processes
    std::vector<boost::thread> fileProcesses;

    // Mutex to lock the counters incrementing
    static boost::mutex updateCountersLock;
    // Variables to store the statistics about files
    static CounterType             filesCount;
    static ConcurrentCounterType   blankLinesCount;
    static ConcurrentCounterType   codeLinesCount;
    static ConcurrentCounterType   commentLinesCount;

    static CounterType CountAndRemoveComments(std::string & fileContents)
    {
        CounterType localCommentLinesCount = 0;
        // Count multiline comment instances
        std::regex multilineCommentRegex(R"row(/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)row");
        std::sregex_iterator commentPtr(fileContents.cbegin(), fileContents.cend(), multilineCommentRegex);
        std::sregex_iterator end;
        for (; commentPtr != end; ++commentPtr) {
            if (!std::regex_match(std::string(commentPtr->prefix()), std::regex(R"row((.|\n)*//[^\n]*)row"))) {
                bool isPrecommented = false;
                if (!std::regex_match(std::string(commentPtr->prefix()), std::regex(R"row((?:.|\n)*/\*\*/[^\n]*$)row"))) {
                    isPrecommented = true;
                    std::stringstream commentStream(commentPtr->str());
                    std::string commentLine;
                    while (std::getline(commentStream, commentLine))
                        ++localCommentLinesCount;
                }
                // Remove the multiline comment with the empty multiline comment:
                // /*comment*/  ->  /**/
                auto commentPos = fileContents.find(commentPtr->str());
                fileContents.replace(commentPos, commentPtr->length(), "/**/");
                if (isPrecommented) {
                    if (!std::regex_match(std::string(commentPtr->prefix()), std::regex(R"row((?:.|\n)*\n\s*$)row")))
                        fileContents.insert(commentPos + 4, "\n", 1);
                    if (!std::regex_match(std::string(commentPtr->suffix()), std::regex(R"row(^\s*\n(?:.|\n)*)row")))
                        fileContents.insert(commentPos, "\n", 1);
                }
                commentPtr = std::sregex_iterator(fileContents.cbegin() + commentPos, fileContents.cend(), multilineCommentRegex);
            }
        }
        std::cout << "================================================================================\n" << fileContents << "\n================================================================================" << std::endl;
        //fileContents = std::regex_replace(fileContents, std::regex(R"row(^/\*\*/[ \t]*\n)row"), "");
        if (std::strcmp(fileContents.c_str(), "/**/\n"))
            fileContents.erase(0, 6);
        //fileContents = std::regex_replace(fileContents, std::regex(R"row((?:\n[ \t]*/\*\*/[ \t]*)|(?:[ \t]*/\*\*/[ \t]*\n)|(?:[ \t]*/\*\*/[ \t]*))row"), "");
        fileContents = std::regex_replace(fileContents, std::regex(R"row([ \t]*/\*\*/[ \t]*\n)row"), "");
        return localCommentLinesCount + CountAndRemovePlainComments(fileContents);
    }

    static CounterType CountAndRemovePlainComments(std::string & fileContents)
    {
        if (fileContents[fileContents.length()-1] != '\n')
            fileContents.push_back('\n');
        CounterType localCommentLinesCount = 0;
        std::stringstream contentStream(fileContents);
        std::string contentLine;
        while (std::getline(contentStream, contentLine)) {
            std::regex plainCommentRegex(R"row(//[^\n]*$)row");
            std::smatch m;
            if(std::regex_search(contentLine.cbegin(), contentLine.cend(), m, plainCommentRegex)) {
                std::string pref = m.prefix();
                if (std::regex_match(pref, std::regex(R"row(^\s*$)row")))
                    fileContents.erase(fileContents.find(contentLine), contentLine.length()+1);
                else
                    fileContents.erase(fileContents.find(m.str()), m.length());
                ++localCommentLinesCount;
            }
        }
        return localCommentLinesCount;
    }

    static std::pair<CounterType, CounterType> CountCodeAndBlankLines(std::string & fileContents)
    {
        CounterType localCodeLinesCount = 0;
        CounterType localBlankLinesCount = 0;
        std::stringstream fileContentsStream(fileContents);
        std::string contentLine;
        while (std::getline(fileContentsStream, contentLine)) {
            if (std::regex_match(contentLine, std::regex(R"row(^\s*$)row")))
                ++localBlankLinesCount;
            else
                ++localCodeLinesCount;
        }
        return std::make_pair(localCodeLinesCount, localBlankLinesCount);
    }

    static void CountBlankCodeCommentLines(const std::string & fileName)
    {
        // Convert file with a given filename into a fileContents string. Then replace quoted strings inside
        // with underscores we don't need them and they can contain key symbol sequences:
        // "string"  ->  _
        std::ifstream file(fileName, std::ios::binary);
        std::stringstream fileStream;
        fileStream << file.rdbuf();
        std::string fileContents(fileStream.str());
        fileContents = std::regex_replace(fileContents, std::regex(R"row("(?:\\.|[^\\"\n])*")row"), "_");

        // Count blank lines, comment lines and lines of the code in the fileContents
        auto localCommentLinesCount      = CountAndRemoveComments (fileContents);
        auto localCodeAndBlankLinesCount = CountCodeAndBlankLines (fileContents);

        std::cout << "================================================================================\n" << fileContents << "\n================================================================================" << std::endl;
        std::cout << "commentLines: " << localCommentLinesCount << std::endl;
        std::cout << "code lines: " << localCodeAndBlankLinesCount.first << std::endl;
        std::cout << "blank lines: " << localCodeAndBlankLinesCount.second << std::endl;

        // Lock the execution of thread until the end of this function' scope };
        // to print name of the processed file and to modify shared counters
        boost::mutex::scoped_lock localLock(updateCountersLock);
        std::cout << "processed file: " << fileName << std::endl;
        commentLinesCount += localCommentLinesCount;
        codeLinesCount    += localCodeAndBlankLinesCount.first;
        blankLinesCount   += localCodeAndBlankLinesCount.second;
    };

    void JoinThreads()
    {
        // Join all threads to ensure that all files have been proc
        std::for_each(fileProcesses.begin(), fileProcesses.end(),
                      [](boost::thread & fileProcess) {
                          if(fileProcess.joinable())
                              fileProcess.join();
                      });
    }
public:
    ConcurrentLineCounter() {
        fileProcesses = std::vector<boost::thread>();
        filesCount        = 0;
        blankLinesCount   = 0;
        codeLinesCount    = 0;
        commentLinesCount = 0;
    };

    void operator()(const std::string & fileName)
    {
        // Count the lines in the file with a given filename
        filesCount += 1;
        fileProcesses.emplace_back(ConcurrentLineCounter::CountBlankCodeCommentLines, fileName);
    };

    friend std::ostream & operator << (std::ostream & outStream, ConcurrentLineCounter & linesCounter);

    void PrintToFile(const std::string & fileName)
    {
        JoinThreads();
        CounterType totalLines{ codeLinesCount + commentLinesCount + blankLinesCount };
        // Create stream for the outputting and direct it toward the given file
        std::ofstream outFile;
        outFile.open(fileName.data(), std::ios::trunc | std::ios::binary);
        outFile <<
                "Number of scanned files:\t"                << filesCount <<
                "\nNumber of scanned lines:\t"              << totalLines <<
                "\nwhere:\n\tnumber of code lines is:\t"    << codeLinesCount <<
                "\n\tnumber of comment lines is:\t"         << commentLinesCount <<
                "\n\tnumber of blank lines:\t\t"            << blankLinesCount <<
                std::endl;
        outFile.close();
    }

    ~ConcurrentLineCounter() {
        JoinThreads();
        fileProcesses.clear();
    }
};

std::ostream & operator << (std::ostream & outStream, ConcurrentLineCounter & lineCounter) {
    lineCounter.JoinThreads();
    CounterType totalLines{ ConcurrentLineCounter::codeLinesCount +
                            ConcurrentLineCounter::commentLinesCount +
                            ConcurrentLineCounter::blankLinesCount };
    outStream <<
              "Number of scanned files:\t"                << ConcurrentLineCounter::filesCount <<
              "\nNumber of scanned lines:\t"              << totalLines <<
              "\nwhere:\n\tnumber of code lines is:\t"    << ConcurrentLineCounter::codeLinesCount <<
              "\n\tnumber of comment lines is:\t"         << ConcurrentLineCounter::commentLinesCount <<
              "\n\tnumber of blank lines:\t\t"            << ConcurrentLineCounter::blankLinesCount <<
              std::endl;
    return outStream;
}

boost::mutex ConcurrentLineCounter::updateCountersLock;
CounterType             ConcurrentLineCounter::filesCount;
ConcurrentCounterType   ConcurrentLineCounter::blankLinesCount;
ConcurrentCounterType   ConcurrentLineCounter::codeLinesCount;
ConcurrentCounterType   ConcurrentLineCounter::commentLinesCount;


// The function for counting the lines
void ConcurrentCodeLinesCount()
{
    ConcurrentLineCounter concurrentlyCountLines;

    std::string dirPath;
    std::string outFileName;

    concurrentlyCountLines("C:\\Users\\faitc\\OneDrive - lpnu.ua\\Cplusplus STUFF\\SoftServeITATasks\\tasksCode\\testCases.cpp");

    /*

    std::cout << "===================  Concurrent line counter  ===================" << std::endl;
    do {
        // Ask to user input the directory
        std::cout << "\nInput full path to the directory with code (without quotes)," <<
                  "\ne.g. C:\\Users\\Public\\VS projects\\current: ";
        std::getline(std::cin, dirPath);
    } while (!boost::filesystem::exists(dirPath) || !boost::filesystem::is_directory(dirPath));
    std::cout << "\nOnly files with extensions \".c\" \".cpp\" \".h\" \".hpp\" will be analyzed!" <<
              "\nInput name of the output file (without .txt), e.g. my_out_file: ";
    std::getline(std::cin, outFileName);
    outFileName += ".txt";
    std::cout << std::endl;

    std::vector<std::regex> extensions({
                                               std::regex(R"raw(^.*\.c$)raw"),
                                               std::regex(R"raw(^.*\.cpp$)raw"),
                                               std::regex(R"raw(^.*\.h$)raw"),
                                               std::regex(R"raw(^.*\.hpp$)raw"),
                                       });
    auto ExtMatchingPred = [](std::string str, std::regex reg) {
        return std::regex_match(str, reg);
    };

    std::chrono::time_point<std::chrono::high_resolution_clock> tpBeforeExecution =
            std::chrono::high_resolution_clock::now();

    boost::filesystem::recursive_directory_iterator dirIter(dirPath);
    boost::filesystem::recursive_directory_iterator dirEnd;
    for (; dirIter != dirEnd; ++dirIter) {
        if (boost::filesystem::is_directory(dirIter->path())) continue;
        std::string fileExtension = dirIter->path().extension().string();
        bool isExtensionCorrect = std::any_of(extensions.cbegin(), extensions.cend(),
                                              std::bind(ExtMatchingPred, fileExtension, std::placeholders::_1));
        if(isExtensionCorrect) concurrentlyCountLines(dirIter->path().string());
    }
    concurrentlyCountLines.PrintToFile(outFileName);

    std::chrono::time_point<std::chrono::high_resolution_clock> tpAfterExecution =
            std::chrono::high_resolution_clock::now();

    std::cout << "\nFiles have been processed in " <<
              std::chrono::duration<long double, std::milli>(tpAfterExecution - tpBeforeExecution).count() <<
              " ms.\nFile statistics:\n" <<
              concurrentlyCountLines << std::endl;
              */
}