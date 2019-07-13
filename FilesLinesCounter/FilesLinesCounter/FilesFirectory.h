#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<filesystem>
#include"StringParse.h"

namespace fs = std::experimental::filesystem;
using namespace std;

int files_in_current_directory(string path) {
	int files = 0;
	for (auto& p : fs::recursive_directory_iterator(path))
		files++;
	return files;
}


vector<fs::path> list_files_in_directory(const fs::path& dir)
{
	vector<fs::path> files;
	if (fs::exists(dir) && fs::is_directory(dir))
	{
		for (const auto& entry : fs::recursive_directory_iterator(dir))
		{
			files.emplace_back(entry.path());
		}
	}
	return files;
}

bool PropriateExtension(fs::path path)
{
	vector<fs::path>extension = { ".cpp",".c",".hpp",".h" };
	for (auto &i : extension)
	{
		if (path.extension() == i)
			return true;
	}
	return false;
}

vector<fs::path>  cpp_files_in_directory(vector<fs::path> list)
{
	vector<fs::path>  cppfiles;

	for (auto & p : list) {
		if (PropriateExtension(p))
		{
			cppfiles.emplace_back(p);
		}
	}
	return cppfiles;
}
