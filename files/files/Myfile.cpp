#include "pch.h"
#include <algorithm>
#include "Myfile.h"
#include "directory.h"
using  namespace std;
namespace fs = std::filesystem;

int Myfile::blank_lines;
int Myfile::comment_lines;
int Myfile::code_lines;

//constructor
Myfile::Myfile(fs::path path_)
{
	path = path_;
}
//destructor 
Myfile::~Myfile() = default;

//making vector where each item is a line of file
void Myfile::read_file() {
	vector<string> filetext;
	ifstream file;
	file.open(this->path);
	string tmp;
	while (!file.eof()) {
		getline(file, tmp);
		filetext.push_back(tmp);
	}
	this->text = replacetxt(filetext);
	file.close();
}

//cout blank lines
void Myfile::count_blank_lines() {
	for (auto line : this->text) {
		if (is_blank(line)) {
			blank_lines++;
		}
	}

}
//count comment lines
void Myfile::count_comment_lines() {
	int start_comment = 0; //number of  line where multiple comment start
	int finish_comment = 0;//number of  line where multiple comment end
	for (int line = 0; line < text.size(); line++) {
		char twosym[2]; //two symbol for finding // and /* */ in lines of file
		if (text[line].size() == 0) continue;
		for (int symbol = 0; symbol < text[line].size() - 1; symbol++) {
			twosym[0] = text[line][symbol];
			twosym[1] = text[line][symbol + 1];
			if (twosym[0] == '/' and twosym[1] == '*') {
				start_comment = line;
				multiple_comment.push_back(line);
			}
			if (twosym[0] == '*' and twosym[1] == '/') {
				finish_comment = line;
				int i = start_comment;
				while (i <= finish_comment) {
					multiple_comment.push_back(i);
					i++;
				}
				comment_lines += (finish_comment - start_comment + 1);
				break;
			}
			if (start_comment != 0 and finish_comment == 0) continue;
			else if (twosym[0] == '/' and twosym[1] == '/') {
				comment_lines++;
				break;
			}
		}
	}
}

//count code lines
void Myfile::count_codes_lines() {
	for (int line = 0; line < text.size(); line++) {
		//check if line is blank or empty
		if (text[line].size() == 0) continue;
		else if (is_blank(text[line])) continue;
		//check if line is a one line comment
		else if (text[line][0] == '/' and text[line][1] == '/')continue;
		//check if line is in multiple comment
		else if (std::find(multiple_comment.cbegin(), multiple_comment.cend(), line) != multiple_comment.cend()) continue;
		//check if line is like /*...*/
		else if (text[line][0] == '/' and text[line][1] == '*' and text[line][text[line].size() - 1] == '/' and text[line][text[line].size() - 2] == '*') continue;
		else {
			code_lines++;
		}

	}
}
