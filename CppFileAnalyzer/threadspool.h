#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>

class ThreadPool
{
public:
	ThreadPool();
	// copy constructor & copy assignment operator are not allowed
	ThreadPool(const ThreadPool& other) = delete;
	ThreadPool& operator=(const ThreadPool& other) = delete;
	ThreadPool(int numOfThreads);
	~ThreadPool();	
	void createThreads(int numOfThreads);	
	void schedule(const std::function<void()>& task);
	
private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> taskQueue;	
	std::mutex mtx;
	std::condition_variable condition;
	std::atomic_bool stop;
};

#endif // !THREADPOOL_H


