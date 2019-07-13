#include <boost/thread/mutex.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <regex>

class ConcurrentLineCounter {
public:
    // Names of the frequently used types
    typedef boost::uint64_t         CounterType;
    typedef boost::atomic_uint64_t  ConcurrentCounterType;
private:
    // Vector variable to store started processes
    std::vector<boost::thread>      fileProcesses;
    // Variables to store the statistics about files
    static CounterType              filesCount;
    static ConcurrentCounterType    blankLinesCount;
    static ConcurrentCounterType    codeLinesCount;
    static ConcurrentCounterType    commentLinesCount;
    // Mutex to lock the counters incrementation as well as output
    static boost::mutex             updateCountersLock;
    // Constant variables with regex templates
    const static std::regex         blankStringRegex;
    const static std::regex         quotedStringRegex;
    const static std::regex         cTypeCommentRegex;
    const static std::regex         linearCommentRegex;
    const static std::regex         cTypeCommentPrefixRegex;
    const static std::regex         linearCommentPrefixRegex;
    const static std::regex         emptyCommentRegex;
    const static std::string        emptyCommentString;

    static void CountLines(const std::string & fileName)
    {
        // Convert file with a given filename into a fileContents string. Then replace quoted strings inside
        // with underscores we don't need them and they can contain key symbol sequences:
        // "string"  ->  _
        std::ifstream file(fileName, std::ios::binary);
        std::stringstream fileStream;
        fileStream << file.rdbuf();
        std::string fileContents(fileStream.str());
        fileContents = std::regex_replace(fileContents, quotedStringRegex, "_");
        // Count comment lines and blank along with code lines in the file content
        auto localCommentLinesCount      = CountAndRemoveComments (fileContents);
        auto localCodeAndBlankLinesCount = CountCodeAndBlankLines (fileContents);
        // Lock the execution of thread until the end of this function' scope };
        // to print name of the processed file and to modify shared counters
        boost::mutex::scoped_lock localLock(updateCountersLock);
        std::cout << "processed file: " << fileName << std::endl;
        commentLinesCount += localCommentLinesCount;
        codeLinesCount    += localCodeAndBlankLinesCount.first;
        blankLinesCount   += localCodeAndBlankLinesCount.second;
    };

    static CounterType CountAndRemoveComments(std::string & fileContents)
    {
        // Shrink sequential comments:    /*comment1*/   /*comment2*/ -> /*comment1/*comment2*/
        fileContents = std::regex_replace(fileContents, std::regex(R"__(\*/[ \t]*/\*)__"), "/*");
        CounterType localCommentLinesCount = 0;
        // Iterate through every c-type comment instance in the file
        std::sregex_iterator commentPtr(fileContents.cbegin(), fileContents.cend(), cTypeCommentRegex);
        std::sregex_iterator endOfComments;
        for (; commentPtr != endOfComments; ++commentPtr) {
            // Get information about captured comment
            std::string commentPrefix = commentPtr->str(1);
            std::string commentBody   = commentPtr->str(2);
            std::string commentSuffix = commentPtr->str(3);
            std::size_t commentPos = fileContents.find(commentBody);
            // Ignore comment if it was shadowed by other comment (one-line or c-type)
            if (std::regex_match(commentPrefix, cTypeCommentPrefixRegex) ||
                std::regex_match(commentPrefix, linearCommentPrefixRegex)) continue;
            // Count lines inside this comment and add their quantity to the local counter
            std::stringstream commentStream(commentBody);
            std::string commentLine;
            CounterType internalLines = 0;
            while (std::getline(commentStream, commentLine))
                ++internalLines;
            localCommentLinesCount += internalLines;
            // Replace the comment body with /**/. But if the comment is multiline
            // and has text after it in the same line, move that text to the next line:
            // /*comment             --\     /**/
            //  */ Some text         --/      Some text
            if (internalLines > 1 && !std::regex_match(commentSuffix, blankStringRegex))
                fileContents.replace(commentPos, commentBody.length(), emptyCommentString + "\n");
            else
                fileContents.replace(commentPos, commentBody.length(), emptyCommentString);
            // Refresh the comments iterator (After this operation ++commentPtr is needed since the
            // first found instance is /**/ as the commentPos index is before our replacement)
            commentPtr = std::sregex_iterator(fileContents.cbegin() + commentPos, fileContents.cend(), cTypeCommentRegex);
        }
        localCommentLinesCount += CountLinearComments(fileContents);
        RemoveAllComments(fileContents);
        return localCommentLinesCount;
    }

    static CounterType CountLinearComments(std::string & fileContents)
    {
        CounterType localCommentLinesCount = 0;
        // Iterate through lines in the file content
        std::stringstream contentStream(fileContents);
        std::string contentLine;
        while (std::getline(contentStream, contentLine)) {
            // Replace each found //... instance with a /**/ and increase the counter
            std::smatch matchedComment;
            if(std::regex_search(contentLine.cbegin(), contentLine.cend(), matchedComment, linearCommentRegex)) {
                std::string commentBody = matchedComment.str(2);
                fileContents.replace(fileContents.find(commentBody), commentBody.length(), emptyCommentString);
                ++localCommentLinesCount;
            }
        }
        return localCommentLinesCount;
    }

    static void RemoveAllComments(std::string & fileContents) {
        fileContents.push_back('\n');
        // Iterate through lines in the file content
        std::stringstream contentStream(fileContents);
        std::string contentLine;
        while (std::getline(contentStream, contentLine)) {
            // Remove each instance of /**/ along with leading newline sign if
            // it is the only text in the line, otherwise remove only /**/
            std::smatch matchedComment;
            if(std::regex_search(contentLine.cbegin(), contentLine.cend(), matchedComment, emptyCommentRegex)) {
                if (std::regex_match(std::string(matchedComment.prefix()), blankStringRegex))
                    fileContents.erase(fileContents.find(contentLine + "\n"), contentLine.length() + 1);
                else
                    fileContents.erase(fileContents.find(matchedComment.str()), matchedComment.length());
            }
        }
        fileContents.erase(fileContents.length()-1, 1);
    }

    static std::pair<CounterType, CounterType> CountCodeAndBlankLines(std::string & fileContents)
    {
        CounterType localCodeLinesCount = 0;
        CounterType localBlankLinesCount = 0;
        // Iterate through each line in the file content and increase the corresponding counters
        std::stringstream fileContentsStream(fileContents);
        std::string contentLine;
        while (std::getline(fileContentsStream, contentLine)) {
            if (std::regex_match(contentLine, blankStringRegex))
                ++localBlankLinesCount;
            else
                ++localCodeLinesCount;
        }
        return std::make_pair(localCodeLinesCount, localBlankLinesCount);
    }

    void JoinThreads()
    {
        // Join all threads to ensure that all files have been processed
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
        fileProcesses.emplace_back(ConcurrentLineCounter::CountLines, fileName);
    };

    // Output statistic both to the stream
    friend std::ostream & operator << (std::ostream & outStream, ConcurrentLineCounter & linesCounter);

    ~ConcurrentLineCounter() {
        JoinThreads();
        fileProcesses.clear();
    }
};

// Static variables, used in the class
boost::mutex      ConcurrentLineCounter::updateCountersLock;
const std::regex  ConcurrentLineCounter::blankStringRegex           (R"__(^\s*$)__");
const std::regex  ConcurrentLineCounter::quotedStringRegex          (R"__("(?:\\.|[^\\"\n])*")__");
const std::regex  ConcurrentLineCounter::cTypeCommentRegex          (R"__((.*)(/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)(.*))__");
const std::regex  ConcurrentLineCounter::linearCommentRegex         (R"__((.*)(//[^\n]*)(.*))__");
const std::regex  ConcurrentLineCounter::cTypeCommentPrefixRegex    (R"__(^.*/\*\*/.*$)__");
const std::regex  ConcurrentLineCounter::linearCommentPrefixRegex   (R"__(^.*//.*$)__");
const std::regex  ConcurrentLineCounter::emptyCommentRegex          (R"__(/\*\*/)__");
const std::string ConcurrentLineCounter::emptyCommentString         ("/**/");
ConcurrentLineCounter::CounterType             ConcurrentLineCounter::filesCount;
ConcurrentLineCounter::ConcurrentCounterType   ConcurrentLineCounter::blankLinesCount;
ConcurrentLineCounter::ConcurrentCounterType   ConcurrentLineCounter::codeLinesCount;
ConcurrentLineCounter::ConcurrentCounterType   ConcurrentLineCounter::commentLinesCount;

std::ostream & operator << (std::ostream & outStream, ConcurrentLineCounter & lineCounter) {
    lineCounter.JoinThreads();
    outStream << "Number of scanned files:\t\t"          << ConcurrentLineCounter::filesCount
              << "\nwhere:\n\tnumber of code lines:\t\t" << ConcurrentLineCounter::codeLinesCount
              << "\n\tnumber of comment lines:\t"        << ConcurrentLineCounter::commentLinesCount
              << "\n\tnumber of blank lines:\t\t"        << ConcurrentLineCounter::blankLinesCount
              << std::endl;
    return outStream;
}

std::pair<std::string, std::string> GetUserInputUI();

// The function for counting the lines
void ConcurrentCodeLinesCount()
{
    // Create the concurrent line counter
    ConcurrentLineCounter concurrentCounter;
    // Request user to input directory path and file name
    std::string dirPath;
    std::string outFileName;
    std::tie(dirPath, outFileName) = GetUserInputUI();
    // Allowed extensions
    std::vector<std::regex> extensions({
        std::regex(R"raw(^.*\.c$)raw"),
        std::regex(R"raw(^.*\.cpp$)raw"),
        std::regex(R"raw(^.*\.h$)raw"),
        std::regex(R"raw(^.*\.hpp$)raw")
    });
    // The predicate to figure out if the given string corresponds given regex
    auto StrMatchRegex = [](std::string str, std::regex reg) {
        return std::regex_match(str, reg);
    };
    // Set time point before execution
    auto tpBeforeExecution = std::chrono::high_resolution_clock::now();
    // Iterate through all directory and its subdirectory items and push each file
    // with appropriate extension to the concurrent counter
    boost::filesystem::recursive_directory_iterator dirIter(dirPath);
    boost::filesystem::recursive_directory_iterator dirEnd;
    for (; dirIter != dirEnd; ++dirIter) {
        if (boost::filesystem::is_directory(dirIter->path())) continue;
        std::string fileExt = dirIter->path().extension().string();
        bool isExtensionCorrect = std::any_of(extensions.cbegin(), extensions.cend(),
                                              std::bind(StrMatchRegex, fileExt, std::placeholders::_1));
        if(isExtensionCorrect) concurrentCounter(dirIter->path().string());
    }
    // Output results to the file
    std::ofstream outFile(outFileName.c_str(), std::ios::trunc | std::ios::binary);
    outFile << "In the \"" << dirPath << "\":\n" << concurrentCounter << std::endl;
    outFile.close();
    // Set time point after execution
    auto tpAfterExecution = std::chrono::high_resolution_clock::now();
    // Compute resulting duration
    auto totalDuration =
            std::chrono::duration<long double, std::milli>(tpAfterExecution - tpBeforeExecution).count();
    // Output execution time and calculated statistics
    std::cout << "\nFiles have been processed in " << totalDuration << " ms."
              << "\nFile statistics:\n"            << concurrentCounter << std::endl;
}

std::pair<std::string, std::string> GetUserInputUI()
{
    std::string dirPath;
    std::string outFileName;
    std::cout << "===================  Concurrent line counter  ===================" << std::endl;
    do {
        // Ask the user to input directory until he input the correct one
        std::cout << "\nInput full path to the directory with code (without quotes),"
                  << "\ne.g. C:\\Users\\Public\\VS projects\\current: ";
        std::getline(std::cin, dirPath);
    } while (!boost::filesystem::exists(dirPath) || !boost::filesystem::is_directory(dirPath));
    // Ask the user to input name of the output file
    std::cout << "\nOnly files with extensions \".c\" \".cpp\" \".h\" \".hpp\" will be analyzed!"
              << "\nInput name of the output file (without .txt), e.g. my_out_file: ";
    std::getline(std::cin, outFileName);
    std::cout << std::endl;
    return std::make_pair(dirPath, outFileName + ".txt");
}