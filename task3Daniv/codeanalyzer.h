#ifndef CODEANALYZER_H
#define CODEANALYZER_H

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

class CodeAnalyzer
{
public:

	CodeAnalyzer(const std::experimental::filesystem::path &path);

	std::string getResult();

private:

	void countCommentLines();

	void countCodeLines();

	void countEmptyLines();

	std::ifstream mFile;
	std::string mCode;
	std::vector<std::string> mComments;


	void detectComments();

	int mCommentLines = 0;
	int mBlankLines = 0;
	int mCodeLines = 0;

	std::string mFileName;

	std::string mResult;
};

#endif // CODEANALYZER_H