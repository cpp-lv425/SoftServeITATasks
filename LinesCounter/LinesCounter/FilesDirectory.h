#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<filesystem>
#include"ResultCodeFileQueue.h"
#include<chrono>
#include<iostream>
namespace fs = std::experimental::filesystem;
using namespace std;
/*these functions provide work and search with files and folders*/

int FilesInCurrentDirectory(string path);//count of file in the directory

vector<fs::path> ListFilesInDirectory(const fs::path& dir);//move through all the files in the directory and 

bool PropriateExtension(fs::path path);// contition if our file == .cpp or .h etc


std::string InputPath();

void PrintResultInTheFile(std::queue<CodeFile> codeFileQ, std::chrono::microseconds runningDuration);