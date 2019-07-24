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
    typedef boost::atomic_bool      ConcurrentFlagType;
private:
    // Vector that allows accessing current processes and trigger variable to shutdown processes
    std::vector<boost::thread>      concurrentProcesses;
    static ConcurrentFlagType       joinConcurrentProcesses;
    // Deque to store pending filenames
    static std::deque<std::string>  pendingFilenames;
    // Variables to store the statistics about files
    static CounterType              filesCount;
    static CounterType    blankLinesCount;
    static CounterType    codeLinesCount;
    static CounterType    commentLinesCount;
    // Mutex to lock the counters incrementation as well as output
    static boost::mutex             updateCountersLock;
    static boost::mutex             pendingFilenamesLock;
    // Constant variables with regex templates
    const static std::regex         blankStringRegex;
    const static std::regex         quotedStringRegex;
    const static std::regex         rawStringRegex;
    const static std::regex         cTypeCommentRegex;
    const static std::regex         cTypeCommentWithPrefixAndSuffixRegex;
    const static std::regex         cTypeCommentInPrefixRegex;
    const static std::regex         cTypeSequentialCommentsRegex;
    const static std::regex         linearCommentRegex;
    const static std::regex         linearCommentInPrefixRegex;
    const static std::string        cTypeEmptyComment;

    static bool IsPendingFilenames()
    {
        // Lock access and verify if there is any pending filenames
        boost::mutex::scoped_lock localLock(pendingFilenamesLock);
        bool isEmpty = !pendingFilenames.empty();
        return isEmpty;
    }

    static std::string PopPendingFilename()
    {
        // Lock access and pop pending pending filename
        boost::mutex::scoped_lock localLock(pendingFilenamesLock);
        if(pendingFilenames.empty())
            return std::string();
        const std::string fileName = pendingFilenames.front();
        pendingFilenames.pop_front();
        return fileName;
    }

    static void PushPendingFilename(const std::string &fileName)
    {
        // Lock access and append pending filename
        boost::mutex::scoped_lock localLock(pendingFilenamesLock);
        pendingFilenames.push_back(fileName);
    }

    static void CountLines()
    {
        // process filenames while there is pending filenames or joining is not forced
        while(!joinConcurrentProcesses || IsPendingFilenames()) {
            // Try to capture filename from pending filenames until success
            std::string fileName = PopPendingFilename();
            while (fileName.empty()) {
                if(joinConcurrentProcesses) return;
                boost::this_thread::yield();
                fileName = PopPendingFilename();
            }
            // Convert file with a given filename into a fileContents string. Then replace quoted and raw strings
            // inside with underscores since we don't need them and they can contain key symbol sequences:
            // "string"  ->  _
            std::ifstream file(fileName, std::ios::binary);
            std::stringstream fileStream;
            fileStream << file.rdbuf();
            std::string fileContents(fileStream.str());
            fileContents = std::regex_replace(fileContents, rawStringRegex, "_");
            fileContents = std::regex_replace(fileContents, quotedStringRegex, "_");
            // Count comment lines and blank along with code lines in the file content
            auto localCommentLinesCount = CountAndRemoveComments(fileContents);
            auto localCodeAndBlankLinesCount = CountCodeAndBlankLines(fileContents);
            // Lock the execution of thread until the end of this function' scope };
            // to print name of the processed file and to modify shared counters
            boost::mutex::scoped_lock localLock(updateCountersLock);
            std::cout << "processed file: " << fileName << std::endl;
            commentLinesCount += localCommentLinesCount;
            codeLinesCount += localCodeAndBlankLinesCount.first;
            blankLinesCount += localCodeAndBlankLinesCount.second;
        }
    };

    static CounterType CountAndRemoveComments(std::string & fileContents)
    {
        // Shrink sequential comments:    /*comment1*/   /*comment2*/ -> /*comment1comment2*/
        fileContents = std::regex_replace(fileContents, cTypeSequentialCommentsRegex, "");
        CounterType localCommentLinesCount = 0;
        // Iterate through every c-type comment instance in the file
        std::sregex_iterator commentPtr(fileContents.cbegin(), fileContents.cend(),
                                        cTypeCommentWithPrefixAndSuffixRegex);
        std::sregex_iterator endOfComments;
        for (; commentPtr != endOfComments; ) {
            // Get information about captured comment
            std::string commentPrefix = commentPtr->str(1);
            std::string commentBody   = commentPtr->str(2);
            std::string commentSuffix = commentPtr->str(3);
            std::size_t commentPos = fileContents.find(commentBody);
            // Ignore comment if it was shadowed by other comment (one-line or c-type),
            // then step after /* and update search results
            if (std::regex_match(commentPrefix, cTypeCommentInPrefixRegex) ||
                std::regex_match(commentPrefix, linearCommentInPrefixRegex)) {
                fileContents.erase(commentPos, 2);
                commentPtr = std::sregex_iterator(fileContents.cbegin() + commentPos - commentPrefix.length(), fileContents.cend(),
                                                  cTypeCommentWithPrefixAndSuffixRegex);
                continue;
            }
            // Count lines inside this comment and add their quantity to the local counter
            std::stringstream commentStream(commentBody);
            std::string commentLine;
            CounterType internalLines = 0;
            while (std::getline(commentStream, commentLine))
                ++internalLines;
            localCommentLinesCount += internalLines;
            // Replace the comment body with /**/. But if the comment is multiline and has text after it in the same line,
            // move that text to the next line and place additional /**/ to account that line already contains comment:
            // /*comment             --\     /**/
            //  */ Some text         --/     /**/ Some text
            bool isMultilineCommentWithText = (internalLines > 1 && !std::regex_match(commentSuffix, blankStringRegex));
            if (isMultilineCommentWithText)
                fileContents.replace(commentPos, commentBody.length(), cTypeEmptyComment + "\n" + cTypeEmptyComment);
            else
                fileContents.replace(commentPos, commentBody.length(), cTypeEmptyComment);
            // Refresh the comments iterator (After this operation ++commentPtr is needed since the commentPos index is
            // before our replacement and the first found instance will be /**/)
            commentPtr = std::sregex_iterator(fileContents.cbegin() + commentPos, fileContents.cend(),
                                              cTypeCommentWithPrefixAndSuffixRegex);
            // Also if we have placed additional /**/, we should step over this stuff twice
            isMultilineCommentWithText? ++++commentPtr : ++commentPtr;
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
                std::string commentBody = matchedComment.str(1);
                fileContents.replace(fileContents.find(commentBody), commentBody.length(), cTypeEmptyComment);
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
            if(std::regex_search(contentLine.cbegin(), contentLine.cend(), matchedComment, cTypeCommentRegex)) {
                if (std::regex_match(std::string(matchedComment.prefix()), blankStringRegex) &&
                    std::regex_match(std::string(matchedComment.suffix()), blankStringRegex))
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
        if (concurrentProcesses.empty()) return;
        // Force joining
        joinConcurrentProcesses = true;
        // Join all threads to ensure that all files have been processed
        std::for_each(concurrentProcesses.begin(), concurrentProcesses.end(),
                      [](boost::thread & fileProcess) {
                          if(fileProcess.joinable())
                              fileProcess.join();
                      });
        concurrentProcesses.clear();
    }
public:
    explicit ConcurrentLineCounter(size_t processesNum = boost::thread::hardware_concurrency()) {
        // start "processesNum" of concurrent processes and save their accessors to the vector
        // if no "processesNum" parameter is specified hardware_concurrency is used
        joinConcurrentProcesses = false;
        concurrentProcesses = std::vector<boost::thread>();
        for (CounterType processNo = 0; processNo < processesNum; ++processNo) {
            concurrentProcesses.emplace_back(ConcurrentLineCounter::CountLines);
        }
        filesCount        = 0;
        blankLinesCount   = 0;
        codeLinesCount    = 0;
        commentLinesCount = 0;
    };

    void operator()(const std::string & fileName)
    {
        // Push given fileName to the pending filenames
        filesCount += 1;
        PushPendingFilename(fileName);
    };

    // Print collected statistics to the output stream
    friend std::ostream & operator << (std::ostream & outStream, ConcurrentLineCounter & linesCounter);

    ~ConcurrentLineCounter() {
        // Wait until the last pending filename to be processed
        JoinThreads();
    }
};

// Static variables, used in the class
// Locks for restricting concurrent access
boost::mutex      ConcurrentLineCounter::updateCountersLock;
boost::mutex      ConcurrentLineCounter::pendingFilenamesLock;
// pendingFilenames that can be accessed by concurrent threads
std::deque<std::string>  ConcurrentLineCounter::pendingFilenames;
// Regex constants
const std::regex  ConcurrentLineCounter::blankStringRegex                     (R"__(^\s*$)__");
const std::regex  ConcurrentLineCounter::quotedStringRegex                    (R"__("(?:\\.|[^\\"\n])*")__");
const std::regex  ConcurrentLineCounter::rawStringRegex                       (R"__(R"(.*)\((?:.|\n)*\)\1")__");
const std::regex  ConcurrentLineCounter::cTypeCommentRegex                    (R"__((/\*[^*]*\*+(?:[^/*][^*]*\*+)*/))__");
const std::regex  ConcurrentLineCounter::cTypeCommentWithPrefixAndSuffixRegex (R"__((.*)(/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)(.*))__");
const std::regex  ConcurrentLineCounter::cTypeCommentInPrefixRegex            (R"__(^.*/\*\*/.*$)__");
const std::regex  ConcurrentLineCounter::cTypeSequentialCommentsRegex         (R"__(\*/[ \t]*/\*)__");
const std::regex  ConcurrentLineCounter::linearCommentRegex                   (R"__((//.*))__");
const std::regex  ConcurrentLineCounter::linearCommentInPrefixRegex           (R"__(^.*//.*$)__");
const std::string ConcurrentLineCounter::cTypeEmptyComment                    ("/**/");
// Flag to force joining
ConcurrentLineCounter::ConcurrentFlagType      ConcurrentLineCounter::joinConcurrentProcesses;
// Counters to store statistics
ConcurrentLineCounter::CounterType             ConcurrentLineCounter::filesCount;
ConcurrentLineCounter::CounterType   ConcurrentLineCounter::blankLinesCount;
ConcurrentLineCounter::CounterType   ConcurrentLineCounter::codeLinesCount;
ConcurrentLineCounter::CounterType   ConcurrentLineCounter::commentLinesCount;

std::ostream & operator << (std::ostream & outStream, ConcurrentLineCounter & lineCounter) {
    // Wait until the last thread has finished its operation
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
        std::regex(R"__(^.*\.c$)__"),
        std::regex(R"__(^.*\.cpp$)__"),
        std::regex(R"__(^.*\.h$)__"),
        std::regex(R"__(^.*\.hpp$)__")
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