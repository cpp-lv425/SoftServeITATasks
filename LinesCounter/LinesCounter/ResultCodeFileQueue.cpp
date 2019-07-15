#include "ResultCodeFileQueue.h"

ResultCodeFileQueue::~ResultCodeFileQueue() = default;
ResultCodeFileQueue::ResultCodeFileQueue(){}

void ResultCodeFileQueue::Push(CodeFile val)// if we wanna add element
{
	std::lock_guard<std::mutex>lock(mut);// lock mutex
	codeFileQueue.push(val);// add
}

void ResultCodeFileQueue::Print()
{
	while (!codeFileQueue.empty()) {
		cout << " " << codeFileQueue.front();
		codeFileQueue.pop();
	}
}

int ResultCodeFileQueue::GetSize()
{
	return codeFileQueue.size();
}

std::queue<CodeFile> ResultCodeFileQueue::GetCodeFileQueue()
{
	return codeFileQueue;
}
