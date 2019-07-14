#include<iostream>
#include"CodeFile.h"
#include"FilesDirectory.h"
#include"PathesQueue.h"
#include"ResultCodeFileQueue.h"
#include"StringParse.h"
#include"ThreadWork.h"

int main()
{
	setlocale(LC_ALL, "Ukrainian");
	std::string path;
	while (true)// untill path is ok
	{
		try 
		{
		path = InputPath();
		if (!(fs::exists(path)))
			throw "Wrong path!";
		}
		catch (const char * cstr)
		{
			std::cerr << cstr << "Try again!"<< std::endl;
			continue;
		}
		std::cout << "Openned successfully!\nRunning..." << std::endl;
		break;
	}
	auto running_start = std::chrono::steady_clock::now();// set start clock

	vector<fs::path> pathesList = ListFilesInDirectory(path);// list with pathes
	std::vector<CodeFile>codeFile;
	std::queue<fs::path> pathesQ_def;// queue
	for (auto &i : pathesList)
	{
		pathesQ_def.push(i);
	}
	PathesQueue pathesQ(pathesQ_def);// we will pop pathes from this queue. This is shell for pathesQ_def
	ResultCodeFileQueue resCodeFileQ;// we will add parsed "CodeFile" objects here
	ThreadFunc(pathesQ, resCodeFileQ, 4); // start to work with files
	auto running_end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>
		(running_end - running_start);	
	PrintResultInTheFile(resCodeFileQ.GetCodeFileQueue(), elapsed_ms);
	return 0;
}