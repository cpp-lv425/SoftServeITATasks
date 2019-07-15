#pragma once

#include "pch.h"
#include "directory.h"


using  namespace std;
namespace fs = std::filesystem;
class Myfile
{
	vector<string> text;
	fs::path path;
	vector<int> multiple_comment; //if index of numder /2==0 it is the start of multiple comment else its a finish of multiple comment
	
public:	
	static int blank_lines;
	static int comment_lines;
	static int code_lines;
	Myfile(fs::path path_);
	~Myfile();
	void read_file();
	void count_blank_lines();
	void count_comment_lines();
	void count_codes_lines();

};

