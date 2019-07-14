#include "cppfileanalyzer.h"

#include <iostream>
#include <exception>

using std::cout;
using std::endl;

int main()
{	
	CppFileAnalyzer fileAnalyzer;
	fileAnalyzer.promptDirectory();
	fileAnalyzer.start();	

	system("pause");
	return 0;
}