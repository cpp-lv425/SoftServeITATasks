#include"CodeFile.h"
#include"StringParse.h"
#include"FilesFirectory.h"
#include<thread>
#include<fstream>
#include<filesystem>
#include<string>
#include<mutex>
#include<chrono>
#include<queue>
#include<atomic>
using namespace std;
namespace fs = std::experimental::filesystem;

std::atomic_int m_processedCount;
std::atomic_int m_physicalCount;
std::atomic_int m_commentCount;
std::atomic_int m_blankCount;

class Queue
{
private:
	std::queue<fs::path> q;
	std::mutex mut;
public:
	Queue() = default;
	Queue(std::queue<fs::path> t_q)
	{
		q = std::move(t_q);
	}
	Queue(Queue &&obj)
	{
		q = std::move(obj.q);
	}
	Queue(const Queue &obj)
	{
		q = obj.q;
	}
	Queue& operator=(const Queue& other)
	{
		q = other.q;
		return *this;
	}
	Queue& operator=(Queue&& other)
	{
		q = std::move(q);
		return *this;
	}
	std::shared_ptr<fs::path> Pop()
	{
		std::lock_guard<std::mutex>lock(mut);
		if (q.empty())return nullptr;
		std::shared_ptr<fs::path> const res
		(
			std::make_shared<fs::path>(std::move(q.front()))
		);
		q.pop();
		return res;
	}
};

class ResultQ
{
private:
	queue<CodeFile>cf;
	mutex mut;
public:
	ResultQ()
	{}
	void Push(CodeFile val)
	{
		std::lock_guard<std::mutex>lock(mut);
		cf.push(val);
	}
	void Print()
	{
		while (!cf.empty()) {
			cout << " " << cf.front();
			cf.pop();
		}
	}
	void PrintTotalResult()
	{

	}
};

ResultQ resQ;
void worker(Queue &q, ResultQ &rq)
{
	shared_ptr<fs::path> file_path;
	while (file_path = q.Pop())
	{
		CodeFile codeF(*file_path);
		codeF.FileRead();
		rq.Push(codeF);
	}
}

void Func(Queue &q, ResultQ &rq, int coresNum)
{
	std::vector<std::thread>thread_vector;
	for (int i = 0; i < coresNum; i++)
	{
		thread_vector.push_back(std::thread(worker,std::ref(q), std::ref(rq)));
	}
	for (auto &i : thread_vector)
	{
		i.join();
	}
}

int main()
{
	setlocale(LC_ALL, "Ukrainian");
	string path = "C:\\Users\\Petro\\Desktop\\";

	vector<fs::path> pathesList = list_files_in_directory(path);
	pathesList = cpp_files_in_directory(pathesList);
	std::vector<CodeFile>codeFile;
	ResultQ rq;
	std::queue < fs::path > q_def;
	for (auto &i : pathesList)
	{
		q_def.push(i);
	}
	Queue q(q_def);
	Func(q, rq, 2);
	rq.Print();

	//Func(q, codeFile, 4);
	int i = 0;
	/*vector<thread>t;
	for (int i = 0; i < FilesVector.size(); i++)
	{
		t.emplace_back(&CodeFile::FileRead, &FilesVector[i]);
	}
	for (auto &i : t)
	{
		i.join();
	}
	for (auto &i : FilesVector)
	{
		cout << i << endl;
	}
	/*vector<thread> t;
	for (int i = 0; i < codeFileVector.size(); i++)
	{
		t.emplace_back(&CodeFile::FileRead, &codeFileVector[i]);
	}
	for (auto &i : t)
	{
		i.join();
	}
	
	t.emplace_back(&CodeFile::FileRead, &codeFileVector[0]);
	t[0].join();*/
	
	
	cin.get();
	return 0;
}