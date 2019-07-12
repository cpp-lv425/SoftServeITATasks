#include "cppfileanalyzer.h"

#include <iostream>
#include <exception>

using std::cout;
using std::endl;

int main()
{	
	CppFileAnalyzer fileAnalyzer;

	while (true)
	{
		try
		{
			fileAnalyzer.promptPath();
			fileAnalyzer.browseForFiles();
			break;
		}
		catch (const std::exception& e)
		{
			cout << "\nUnable to process entered directory due to access denial.\
 Please specify another directory.\n";
		}
	}	
	
	fileAnalyzer.start();	

	system("pause");
	return 0;
}