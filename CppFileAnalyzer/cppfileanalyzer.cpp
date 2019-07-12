#include "cppfileanalyzer.h"

//#define DEBUG
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <functional>

using std::cout;
using std::cin;

CppFileAnalyzer::CppFileAnalyzer()
{
}

void CppFileAnalyzer::promptPath()
{
	std::string input;
	while (true)
	{
		input = promptString();

		initDirectory = (input.size()) ? input :
			fs::current_path();

		// converts directory separators in path to satify
		// requirement of current OS
		initDirectory.make_preferred();

		if (fs::exists(initDirectory))
		{
			cout << "\nProcessed directory: " << initDirectory.string() << '\n';
			break;
		}
		cout << "Such directory doesn't exist.\
 Please try again\n>> ";
		initDirectory.clear();
	}
}

std::string CppFileAnalyzer::promptString()const
{
	cout << "\nEnter project root folder in format \"Drive:/dir/subdir\"\n"
		<< "or enter empty line to process current work"
		<< " directory\n>> ";

	std::string input;
	std::getline(cin, input);
	return input;
}

void CppFileAnalyzer::browseForFiles()
{
	// launch timer
	startTime = ch::high_resolution_clock::now();

	fs::recursive_directory_iterator begin(initDirectory);
	fs::recursive_directory_iterator end;

	// enlists *.c *.h *.hpp *.cpp files
	std::copy_if(begin, end, std::back_inserter(files),
		[](const fs::path& path)
		{
			return fs::is_regular_file(path) &&
				(
				(path.extension() == ".c") ||
					(path.extension() == ".cpp") ||
					(path.extension() == ".h") ||
					(path.extension() == ".hpp")
					);
		});

	std::cout << "\nList of found files: " << std::endl;
	std::copy(files.begin(), files.end(),
		std::ostream_iterator<fs::path>(std::cout, "\n"));
	std::cout << '\n';
}

void CppFileAnalyzer::start()
{
	allocateWork();
	outputResults();
	writeResultsToFile();
}

void CppFileAnalyzer::allocateWork()
{
	// there are no files to process
	if (!files.size())
		return;

	int minPerThread = 2;
	int maxThreads = (files.size() + minPerThread - 1) / minPerThread;

	int supportedThreads = std::thread::hardware_concurrency();

	// choses desired number of threads or supported number of threads 
	// whichever is less
	// if std::thread::hardware_concurrency() returns 0
	// number of threads is 4
	int numOfThreads = std::min(!supportedThreads ? 4 :
		supportedThreads, maxThreads);

	int step = files.size() / numOfThreads;
	step = !step ? 1 : step;

	// creating pool of threads
	ThreadPool pool(numOfThreads);

	// adding tasks
	for (int i = 0; i < files.size(); i++)
	{
		auto f = std::mem_fn(&CppFileAnalyzer::processFile);
		auto fb = std::bind(f, this);
		pool.schedule(fb);
	}
}

void CppFileAnalyzer::processFile()
{
	fs::path file;
	{
		std::unique_lock<std::mutex> ul(mtx);

#ifdef DEBUG
		cout << "Current thread ID: "
			<< std::this_thread::get_id()
			<< '\n';
#endif // DEBUG
		if (!files.empty())
		{
			file = files.front();
			files.pop_front();
		}
		else
			return;
	}

	std::ifstream fin;

	fin.open(file.c_str(), std::ios_base::in);
	if (!fin.is_open())
	{
		std::cout << "Unable to open file.\n";
		system("pause");
		return;
	}

	std::string line, buf;
	std::vector<std::pair<TextType, std::string>> text;

	while (std::getline(fin, line))
	{
		// whitespaces are not copied to container
		copy_if(line.begin(), line.end(),
			back_inserter(buf),
			[](const char c)
			{return !::isblank(c); });

		text.emplace_back(make_pair
		(TextType::Unknown, buf));
		buf.clear();
	}
	fin.close();
	++filesNum;

	textAnalyzer(text);
}

void CppFileAnalyzer::textAnalyzer
(std::vector<std::pair<TextType, std::string>>& text)
{
	for (int i = 0; i < text.size(); ++i)
	{
		// blank line
		if (!text[i].second.size())
		{
			text[i].first = TextType::BlankLine;
			continue;
		}
		// CppStyleComment
		if (text[i].second.at(0) == '/' && text[i].second.at(1) == '/')
		{
			text[i].first = TextType::CppStyleComment;
			continue;
		}
		// CStyleComment
		if (text[i].second.at(0) == '/' && text[i].second.at(1) == '*')
		{
			processCStyleComment(text, i);
			continue;
		}

		// "
		if (text[i].second.at(0) == '\"')
		{
			processQuotes(text, i);
			continue;
		}

		// code
		text[i].first = TextType::Code;

		// comment after code
		int pos = text[i].second.find('/');

		if (pos == std::string::npos)
			continue;

		if (text[i].second[pos + 1] == '/')
		{
			text[i].first = TextType::CodeAndComment;
			continue;
		}

		if (text[i].second[pos + 1] == '*')
		{
			text[i].first = TextType::CodeAndComment;
			processCStyleComment(text, i, pos);
		}
	}

	// summing up results
	for (auto& item : text)
	{
		switch (item.first)
		{
		case TextType::BlankLine:
		{
			++blankLinesNum;
			break;
		}
		case TextType::Code:
		{
			++codeLinesNum;
			break;
		}
		case TextType::CStyleComment:
		{
			// skipping
		}
		case TextType::CppStyleComment:
		{
			++commLinesNum;
			break;
		}
		case TextType::CodeAndComment:
		{
			++commLinesNum;
			++codeLinesNum;
			break;
		}
		}
	}
}

void CppFileAnalyzer::processCStyleComment
(std::vector<std::pair<TextType, std::string>>& text,
	int& i, int pos)
{
	if (text[i].first == TextType::Unknown)
		text[i].first = TextType::CStyleComment;

	while (true)
	{
		pos = text[i].second.find("*", pos);

		if (pos == std::string::npos)
		{
			if (i < text.size() - 1)
				++i;
			else
				return;

			text[i].first = TextType::CStyleComment;
			pos = 0;
			continue;
		}

		if (text[i].second[pos + 1] == '/')
		{
			// check if there is code after comments
			pos = text[i].second.find_first_not_of(" \t", pos + 2);

			if (pos == std::string::npos)
				return;

			text[i].first = TextType::CodeAndComment;
			return;
		}
		++pos;
	}
}

void CppFileAnalyzer::processQuotes
(std::vector<std::pair<TextType, std::string>>&text, int& i)
{
	if (text[i].first == TextType::Unknown)
		text[i].first = TextType::Code;

	int pos = 0;

	while (true)
	{
		pos = text[i].second.find("\"", pos);

		if (pos == std::string::npos)
			return;

		// check if there are comments after closing
		// quotation marks
		pos = text[i].second.find('/', pos + 1);

		if (pos == std::string::npos)
			return;

		if (text[i].second[pos + 1] == '/')
		{
			text[i].first = TextType::CodeAndComment;
			return;
		}

		if (text[i].second[pos + 1] == '*')
		{
			text[i].first = TextType::CodeAndComment;
			processCStyleComment(text, i, pos);
		}
	}
}

void CppFileAnalyzer::outputResults()
{
	duration = ch::high_resolution_clock::now() -
		startTime;

	std::cout << "\nNumber of processed files: "
		<< filesNum << '\n'
		<< "Number of blank lines: " << blankLinesNum
		<< '\n'
		<< "Number of lines with code: " << codeLinesNum
		<< '\n'
		<< "Number of commented lines: " << commLinesNum
		<< '\n'
		<< "Execution time: " << duration.count()
		<< " seconds\n";
}

void CppFileAnalyzer::writeResultsToFile() const
{
	std::ofstream fout("results.txt");
	if (!fout.is_open())
	{
		std::cout << "Unable to open file.\n";
		system("pause");
		return;
	}
	fout << "\nNumber of processed files: "
		<< filesNum << '\n'
		<< "Number of blank lines: " << blankLinesNum
		<< '\n'
		<< "Number of lines with code: " << codeLinesNum
		<< '\n'
		<< "Number of commented lines: " << commLinesNum
		<< '\n'
		<< "Execution time: " << duration.count()
		<< " seconds\n";

	fout.close();

	cout << "Results have been written to the file\
 \"results.txt\"\n";
}


