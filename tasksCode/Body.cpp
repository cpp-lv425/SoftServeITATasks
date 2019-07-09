#include <iostream>
#include <windows.h>

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

	ASSERT (2 != 1)

	system("pause");
	return 0;
}