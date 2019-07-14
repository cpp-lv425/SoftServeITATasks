#pragma once
#include<thread>
#include<filesystem>
#include"PathesQueue.h"
#include"ResultCodeFileQueue.h"
#include<vector>

namespace fs = std::experimental::filesystem;

void ThreadWorker(PathesQueue &pq, ResultCodeFileQueue & rcfq);// Work Process

void ThreadFunc(PathesQueue &pq, ResultCodeFileQueue & rcfq, int coreNuber);// management of the division of the work