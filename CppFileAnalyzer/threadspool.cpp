#include "threadspool.h"

ThreadPool::ThreadPool(): stop(false)
{
}

ThreadPool::ThreadPool(int numOfThreads): ThreadPool()
{
	createThreads(numOfThreads);
}

ThreadPool::~ThreadPool()
{
	stop = true;
	condition.notify_all();
	// waits for threads to complete tasks
	for (std::thread& t : threads)
	{
		if(t.joinable())
			t.join();
	}
}

void ThreadPool::createThreads(int numOfThreads)
{
	for (size_t i = 0; i < numOfThreads; i++)
	{
		// creating new thread
		threads.emplace_back([this]()->void
			{
				while (true)
				{
					// task wrapper
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(mtx);
						// waits for next task
						condition.wait(lock, [this]()->bool
							{
								return !taskQueue.empty() || stop;
							});
						
						// threads will return when dtor is called & tasks queue is empty
						if (stop && taskQueue.empty())
							return;						

						// receiving next task & deleting it from queue
						task = std::move(taskQueue.front());
						taskQueue.pop();
					}
					// task execution
					task();					
				}
			});
	}
}

void ThreadPool::schedule(const std::function<void()>& task)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		taskQueue.push(task);
	}	
	// every time task is added one of waiting threads is unblocked
	condition.notify_one();
}
