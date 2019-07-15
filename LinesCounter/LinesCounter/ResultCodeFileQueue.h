#pragma once
#include<filesystem>
#include<mutex>
#include<queue>
#include"CodeFile.h"

namespace fs = std::experimental::filesystem;
/*this class is similar to class "PathesQueue", but instead of safe getting elements
it has safe addind. Mhetod push does it. Class has a queue of CodeFile class as private parametr
and add to this queue new "CodeFile" object*/
class ResultCodeFileQueue
{
private:
	std::queue<CodeFile>codeFileQueue;
	std::mutex mut;
public:
	ResultCodeFileQueue();
	~ResultCodeFileQueue();
	void Push(CodeFile val);
	void Print();
	int GetSize();
	std::queue<CodeFile> GetCodeFileQueue();
};

