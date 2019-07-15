#pragma once
#include<filesystem>
#include<mutex>
#include<queue>

namespace fs = std::experimental::filesystem;
/*this class is shell for our pathesList queue. This has been released in order to
have safe access to element of the queue. Mhetod Pop could let get value
only for the one thread. So, we are sure that's not any race dondition
and threads won't work with the same element at the same time*/
class PathesQueue
{
	std::queue<fs::path> pathesQueue;
	std::mutex mut;
public:
	PathesQueue();
	PathesQueue(std::queue<fs::path>t_pathesQueue);
	PathesQueue(PathesQueue &&obj);
	PathesQueue(PathesQueue &obj);
	~PathesQueue();
	PathesQueue& operator=(const PathesQueue& other);
	PathesQueue& operator=(PathesQueue&& other);
	std::shared_ptr<fs::path>Pop();

};

