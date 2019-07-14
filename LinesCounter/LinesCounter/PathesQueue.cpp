#include "PathesQueue.h"

PathesQueue::PathesQueue() = default;

PathesQueue::~PathesQueue() = default;

PathesQueue::PathesQueue(std::queue<fs::path> t_pathesQueue)
{
	pathesQueue = std::move(t_pathesQueue);
}

PathesQueue::PathesQueue(PathesQueue && obj)
{
	pathesQueue = std::move(obj.pathesQueue);
}

PathesQueue::PathesQueue(PathesQueue & obj)
{
	pathesQueue = obj.pathesQueue;
}

PathesQueue & PathesQueue::operator=(const PathesQueue & other)
{
	pathesQueue = other.pathesQueue;
	return *this;
}

PathesQueue & PathesQueue::operator=(PathesQueue && other)
{
	pathesQueue = std::move(other.pathesQueue);
	return *this;
}

std::shared_ptr<fs::path> PathesQueue::Pop()
{
	std::lock_guard<std::mutex>lock(mut);// lock mutex(other threads can't access here)
	if (pathesQueue.empty())return nullptr;// if queue is empty (all tasks have been booked by threads)
	std::shared_ptr<fs::path> const result
	(
		std::make_shared<fs::path>(std::move(pathesQueue.front()))// get first element
	);
	pathesQueue.pop();// delete first elem
	return result;// return this elem
}
