/*######################################################################################################*/
// First task
/* Initializes list */
extern void StringListInit(char*** list);
/* Destroy list and set pointer to NULL. */
extern void StringListDestroy(char*** list);

/* Inserts value at the end of the list. */
extern void StringListAdd(char*** list, const char * str);
/* Removes all occurrences of str in the list. */
extern void StringListRemove(char** list, const char * str);

/* Returns the number of items in the list. */
extern int StringListSize(char** list);
/* Returns the index position of the first occurrence of str in the list. */
extern int StringListIndexOf(char** list, const char * str);

/* Removes duplicate entries from the list. */
extern void StringListRemoveDuplicates(char** list);
/* Replaces every occurrence of the before, in each of the string lists's strings, with after. */
extern void StringListReplaceInStrings(char** list, const char * before, const char * after);
/* Sorts the list of strings in ascending order */
extern void StringListSort(char** list);
/* Shrinks allocated memory to fit all elements in the list */
extern void StringListShrinkToFit(char *** list);
/*######################################################################################################*/
// Second task
#include <AnyType.hpp>
/*######################################################################################################*/
// Third task
extern void ConcurrentCodeLinesCount();
/*######################################################################################################*/
#include <iostream>
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <regex>

// If the condition is false, print "Assertion failed" and prompt to exit
#define ERROR_TEXT_COLOR 12	// red color before the black background
#define ASSERT(condition) if(!(condition)) {											\
	if (!g_isConsoleInitialized) InitializeConsole();									\
	SetConsoleTextAttribute(g_consoleHandler, ERROR_TEXT_COLOR);						\
	std::cout << "Assertion failed, press any key to continue" << std::endl;			\
	SetConsoleTextAttribute(g_consoleHandler, g_consoleDefaultAttributes.wAttributes);	\
	system("pause"); exit(1); }

HANDLE						g_consoleHandler;
CONSOLE_SCREEN_BUFFER_INFO	g_consoleDefaultAttributes;
bool g_isConsoleInitialized = false;

inline void InitializeConsole() {
	g_consoleHandler = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(g_consoleHandler, &g_consoleDefaultAttributes);
	g_isConsoleInitialized = true;
}


int main(int argc, char ** argv)
{
	InitializeConsole();

	using std::cout;
	using std::endl;

	cout << "START CODE LINES COUNTER:" << endl;
	//ConcurrentCodeLinesCount();

    std::string fileContents(R"row(// include the needed files
#include <iostream> //include the iostream lib
/* use namespace */
using namespace std;

/*
    main function
*/
int main(int argc, char** argv)
{
    // the multiline comment is next /* your comment */

    /*
        but for example if you want singleline comment, you
        should type:
            // your comment
    */
    return 0;
}
/* end of the program*/ something /*end of the program*/)row");
    if (fileContents[fileContents.length()-1] != '\n')
        fileContents.push_back('\n');

    //std::regex blankLineRegex(R"row([[:blank:]]*)row");
    //std::regex commentLineRegex(R"row(//.*)row");
    //std::regex multilineCommentToken(R"row((/\*)1(.*)\*/.*)row");
    //std::regex newLineToken(R"row([\^]?(.*)[\n$])row");


    //std::regex lineRegexToken(R"row([\^]?(.*[\n$]))row");
    //std::regex blankLineRegex(R"row(^[[:blank:]]*\n$)row");
    //std::regex quotedStringRegex(R"row("(?:\\.|[^\\"])*")row");
    std::regex multilineCommentRegex(R"row(/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)row");
    std::regex singlelineCommentRegex(R"row((.|\n)*//[^\n]*)row");
    std::regex twoMultilineCommentsSequentially(R"row(/\*[^*]*\*+(?:[^/*][^*]*\*+)*/([^\n]*)/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)row");

    //fileContents = std::regex_replace(fileContents, quotedStringRegex, std::string("\"\""));
    std::cout << "=========================================================================" << std::endl;
    std::cout << fileContents;
    std::cout << "=========================================================================" << std::endl;

    int localBlankLinesCount = 0;

    std::stringstream contentStream(fileContents);
    std::string contentLine;
    while (std::getline(contentStream, contentLine)) {
        if (std::regex_match(contentLine, std::regex(R"row(^[[:blank:]]*$)row"))) {
            fileContents.erase(fileContents.find(contentLine), contentLine.length());
            ++localBlankLinesCount;
        }
    }
    std::cout << "blank lines: " << localBlankLinesCount << std::endl;

    std::cout << "=========================================================================" << std::endl;
    std::cout << fileContents;
    std::cout << "=========================================================================" << std::endl;


    fileContents = std::regex_replace(fileContents, twoMultilineCommentsSequentially, "$1/* */");

    std::cout << "=========================================================================" << std::endl;
    std::cout << fileContents;
    std::cout << "=========================================================================" << std::endl;

    std::smatch m;
    std::sregex_iterator beg(fileContents.cbegin(), fileContents.cend(), multilineCommentRegex);
    std::sregex_iterator end;
    while (beg != end) {
        std::string comm = beg->str();
        std::string pref = beg->prefix();
        std::cout << "comment body: "<< comm << std::endl;
        if (!std::regex_match(pref, singlelineCommentRegex)) {
            std::cout << "comment correct: " << std::endl;
            std::stringstream commentLinesStream(comm);
            int localLinesCount = 0;
            std::string tmpLine;
            while (std::getline(commentLinesStream, tmpLine))
                ++localLinesCount;
            std::cout << "comment lines: " << localLinesCount << std::endl;

            /* replace the file */
            size_t start_pos = fileContents.find(comm);
            fileContents.replace(start_pos, comm.length(), std::string(""));
            beg = std::sregex_iterator(fileContents.cbegin() + start_pos, fileContents.cend(), multilineCommentRegex);
            continue;
        }
        std::cout << "comment incorrect!!!!" << std::endl;
        beg++;
    }

    std::cout << "=========================================================================" << std::endl;
    std::cout << fileContents;
    std::cout << "=========================================================================" << std::endl;


    std::stringstream commentLinesStream(fileContents);
    int commentLinesCount = 0;
    std::string tmpLine;
    while (std::getline(commentLinesStream, tmpLine))
    {
        std::cout << "line: \"" << tmpLine << "\"" << std::endl;
        if (std::regex_match(tmpLine, singlelineCommentRegex)) {
            std::cout << "comment body: " << tmpLine << std::endl;
            fileContents.erase(fileContents.find(tmpLine), tmpLine.length());
            ++commentLinesCount;
        }
    }
    std::cout << "singleline comment lines: " << commentLinesCount << std::endl;

    std::cout << "=========================================================================" << std::endl;
    std::cout << fileContents;
    std::cout << "=========================================================================" << std::endl;

    ConcurrentCodeLinesCount();

	ASSERT (2 != 1)
    system("pause");
	return 0;
}