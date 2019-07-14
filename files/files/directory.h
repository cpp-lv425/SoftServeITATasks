#pragma once
#include "pch.h"
#include <iostream>
#include <filesystem>
#include <thread>
#include <fstream>
#include <vector>
#include <mutex>      
#include "Myfile.h"
using  namespace std;
namespace fs = std::filesystem;
class Myfile;
int count_cpp_files(string path);
vector<fs::path> list_cpp_files_in_directory(string path);
vector<Myfile> objects(vector<fs::path> path_files);
void my_thread(Myfile file);
bool is_blank(string line);
vector <string> replacetxt(vector<string> txt);
