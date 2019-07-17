#include "stdafx.h"
#include "codeanalyzer.h"

#include <regex>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

CodeAnalyzer::CodeAnalyzer(const std::experimental::filesystem::path& path)
	: mFile{path},
	mFileName{ path.filename().string() }
{
}

std::string CodeAnalyzer::getResult()
{
	std::string str((std::istreambuf_iterator<char>(mFile)),
		std::istreambuf_iterator<char>());
	mCode = str;

	mCode.erase(std::remove(mCode.begin(), mCode.end(), '\t'), mCode.end());
	mCode.erase(std::remove(mCode.begin(), mCode.end(), '\v'), mCode.end());
	mCode.erase(std::remove(mCode.begin(), mCode.end(), ' '), mCode.end());

	detectComments();

	countCommentLines();

	countEmptyLines();

	countCodeLines();

	mResult = mFileName;

	mResult.append("\ncomment lines - " + std::to_string(mCommentLines));

	mResult.append("\nempty lines  -" +std::to_string(mBlankLines));

	mResult.append("\ncode lines - " + std::to_string(mCodeLines));

	mResult.append("\n\n");
	return mResult;
}

void CodeAnalyzer::detectComments()
{
	std::string str = mCode;

	//pattern to detect all "strings"
	const std::regex vowels("\"(?:\\\\\"|.)*?\"");

	std::stringstream result;
	std::regex_replace(std::ostream_iterator<char>(result), str.begin(), str.end(), vowels, "");
	str = result.str();

	//pattern to detect all comments
	const std::string pattern("(/\\*([^*]|[\r\n]|(\\*+([^*/]|[\r\n])))*\\*+/)|(//.*)");
	std::regex r(pattern, std::regex_constants::egrep);


	for (std::sregex_iterator it(str.begin(), str.end(), r), end; it != end; ++it)
	{
		mComments.push_back(it->str());
	}
}

void CodeAnalyzer::countCommentLines()
{
	int totalCommentLines = 0;
	for (size_t i = 0; i < mComments.size(); ++i)
	{
		std::string s = mComments[i];
		int pos1 = mCode.find(mComments[i]);
		std::string::size_type n = std::count(mComments[i].begin(), mComments[i].end(), '\n');

		int pos2;
		std::string betweenCom;
		std::string::size_type nbetween;

		if (pos1 + mComments[i].size() > mCode.size())
		{
			continue;
		}
		if (i != mComments.size() - 1)
		{
			pos2 = mCode.find(mComments[i + 1]);
			betweenCom = mCode.substr(pos1 + mComments[i].size(), pos2 - pos1 - mComments[i].size());
			nbetween = std::count(betweenCom.begin(), betweenCom.end(), '\n');
		}

		if ((pos1 - 1) > 0)
		{
			if (mCode.at(pos1 - 1) == '\n' && (pos1 + mComments[i].size()) < mCode.size())
			{
				if (mCode.at(pos1 + mComments[i].size()) == '\n')
				{
					totalCommentLines += n + 1;
					mCode.erase(pos1, mComments[i].size() + 1);
				}
			}

			if ((pos1 + mComments[i].size()) < mCode.size())
			{
				if (mCode.at(pos1 - 1) == '\n' && mCode.at(pos1 + mComments[i].size()) != '\n' && nbetween == 0)
				{
					mCode.erase(pos1, mComments[i].size());
				}
			}

			if (mCode.at(pos1 - 1) != '\n' && mCode.at(pos1 + mComments[i].size()) != '\n')
			{
				totalCommentLines += n + 1;
				mCode.erase(pos1, mComments[i].size());
			}

			if ((mCode.at(pos1 - 1) != '\n' && mCode.at(pos1 + mComments[i].size()) == '\n'))
			{
				totalCommentLines += n + 1;
				mCode.erase(pos1, mComments[i].size());
			}
		}

	}
	mCommentLines = totalCommentLines;
}

void CodeAnalyzer::countCodeLines()
{
	std::istringstream f(mCode);
	std::string line;
	int total = 0;
	while (std::getline(f, line)) 
	{
		total++;
	}

	mCodeLines = total - mBlankLines;
}

void CodeAnalyzer::countEmptyLines()
{
	for (int i = 0; i < mCode.size() - 1; ++i)
	{
		if (mCode[i] == '\n' && mCode[i + 1] == '\n')
		{
			mBlankLines++;
		}
	}
}