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

// If the condition is false, print "Assertion failed" and prompt to exit
//#define ERROR_TEXT_COLOR 12	// red color before the black background
const int ERROR_TEXT_COLOR = 12; // red color before the black background
bool g_isConsoleInitialized = false;

HANDLE						g_consoleHandler;
CONSOLE_SCREEN_BUFFER_INFO	g_consoleDefaultAttributes;

inline void ConsoleInit()
{
	if (g_isConsoleInitialized) return;
	g_consoleHandler = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(g_consoleHandler, &g_consoleDefaultAttributes);
	g_isConsoleInitialized = true;
}

void ASSERT(bool _condition)
{
	if (_condition) return;
	ConsoleInit();
	SetConsoleTextAttribute(g_consoleHandler, ERROR_TEXT_COLOR);
	std::cout << "Assertion failed, press any key to continue" << std::endl;
	SetConsoleTextAttribute(g_consoleHandler, g_consoleDefaultAttributes.wAttributes);
	system("pause");
	exit(EXIT_FAILURE);
}

int main(int argc, char ** argv)
{
	ConsoleInit();

	using std::cout;
	using std::endl;

	ASSERT (2 != 1);

	ConcurrentCodeLinesCount();

    system("pause");
	return 0;
}