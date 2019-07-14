#pragma once
#include<string>
#include<fstream>
#include<filesystem>
#include"StringParse.h"
#include<iostream>
#include<regex>
#include<Windows.h>
namespace fs = std::experimental::filesystem;
using namespace std;

//this class contains information about each file which we'll check
class CodeFile
{
private:
	int m_processedLinesCount;
	int m_blankLinesCount;
	int m_commentLinesCount;
	int m_physicalLinesCount;
	ifstream m_file;//stream for this file
	string m_pathName;//path
public:
	CodeFile(fs::path t_path);
	CodeFile(CodeFile &&obj);
	CodeFile(const CodeFile &obj);
	~CodeFile();
	string GetPathName();
	void FileClose();
	int GetProcessedCount();
	int GetPhysicalCount();
	int GetBlankCount();
	int GetCommentCount();
	void FileRead();// this function will count the amount of each type of line
	friend std::ostream& operator<<(std::ostream &out, const CodeFile &codeFile);// overloaded operator <<
};

