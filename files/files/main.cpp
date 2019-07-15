// files.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "directory.h"
#include "Myfile.h"
using  namespace std;
namespace fs = std::filesystem;

int main()
{

	string path = "C:\\Users\\Anastasia Antonyk\\Desktop\\SoftserveITATasks";
	cout << "Path:\t" << path << endl;
	cout << "Number of detected files:\t" << count_cpp_files(path) << endl;
	vector<fs::path> cppfiles = list_cpp_files_in_directory(path); //find cpp files in chosen directory
	vector<Myfile> files = objects(cppfiles); //making vector of object of cppfiles
	vector<thread> t;
	for (auto p : files) {
		t.emplace_back(my_thread, p);
	}
	for (auto & p : t) {
		p.join();
	}
	cout << "Number of blank lines in files:\t" << Myfile::blank_lines << endl;
	cout << "Number of comment lines in file:\t" << Myfile::comment_lines << endl;
	cout << "Number of code line in file:\t" << Myfile::code_lines << endl;

}