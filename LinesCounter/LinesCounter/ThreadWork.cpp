#include"ThreadWork.h"

void ThreadWorker(PathesQueue &pq, ResultCodeFileQueue & rcfq)
{
	shared_ptr<fs::path>file_path;
	while (file_path = pq.Pop())// while we have tasks(files pathes)
	{
		CodeFile codeF(*file_path);// create new CodeFile obj
		codeF.FileRead();// start function for this obj
		rcfq.Push(codeF);// add it to our CodeFile queue
	}
}

void ThreadFunc(PathesQueue &pq, ResultCodeFileQueue & rcfq, int coresNuber)
{
	std::vector<std::thread>threadVector;
	for (int i = 0; i < coresNuber; i++)
	{
		threadVector.push_back(std::thread(ThreadWorker, std::ref(pq), std::ref(rcfq)));// call func
	}
	for (auto &i : threadVector)
	{
		i.join();
	}
}