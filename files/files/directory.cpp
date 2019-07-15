#include "pch.h"
#include "directory.h"
#include "Myfile.h"



using  namespace std;
namespace fs = std::filesystem;

//check if file have one of extension: cpp, c, hpp, h
bool suitable_file(fs::directory_entry pathname) {
	bool suitable = false;
	vector<fs::path> extentions = { ".cpp",".h",".c",".hpp" };
	for (auto p : extentions) {
		if ((pathname.path()).extension() == p) suitable = true;
	}
	return suitable;
}

//count suitable files in directory and subdirectories
int count_cpp_files(string path) {
	int files = 0;
	for (auto& p : fs::recursive_directory_iterator(path)) {
		if (fs::is_directory(p.path())) continue;	//don`t check directory path for suitable extension
		if (suitable_file(p)) {
			files++;
		}
	}
	return files;
}

//make a vector with pathes of suitable files
vector<fs::path> list_cpp_files_in_directory(string path) {
	vector<fs::path> files;
	for (auto& p : fs::recursive_directory_iterator(path)) {
		if (fs::is_directory(p.path())) continue;
		if (suitable_file(p)) {
			files.emplace_back(p.path());
		}

	}
	return files;
}

//make a vector of object of class Myfile with pathes of cpp files
vector<Myfile> objects(vector<fs::path> path_files) {
	vector<Myfile> files;
	for (auto p : path_files) {
		files.emplace_back(p);
	}

	return files;
}

//make thread for every file to work out with it
mutex mut;
void my_thread(Myfile file) {
	mut.lock();
	file.read_file();
	file.count_blank_lines();
	file.count_comment_lines();
	file.count_codes_lines();
	//cout << "in the function my_thread with thread index: " << this_thread::get_id() << endl;
	mut.unlock();
}

//check if the string line is blank
bool is_blank(string line) {
	bool is_blank = true;
	for (auto symbol : line) {
		if (symbol != '\t' and symbol != ' ') is_blank = false;
	}
	return is_blank;
}

//replace text in "" to 0..00 to avoid detecting comments in strings
vector <string> replacetxt(vector<string> txt) {
	vector<string> newtxt;
	for (auto line : txt) {
		int symbol = 0;
		while (symbol < line.size()) {
			if (line[symbol] == '\"') {
				symbol++;
				while (line[symbol] != '\"') {
					line[symbol] = '0';
					symbol++;
				}
			}
			symbol++;
		}
		newtxt.push_back(line);
	}
	return newtxt;
}