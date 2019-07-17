#include "stdafx.h"
#include "codeanalyzer.h"
#include "threadpool.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<std::experimental::filesystem::path> paths;

	std::string folder = "C:\\chess-qt-master\\Lab_5";

	for (auto& p : std::experimental::filesystem::recursive_directory_iterator(folder))
	{
		if (p.path().extension() == ".h"|| p.path().extension() == ".cpp"
			|| p.path().extension() == ".hpp" || p.path().extension() == ".c")
		{
			paths.push_back(p.path());
		}
	}


	std::vector<CodeAnalyzer> analyzer;
	for (auto a : paths)
	{
		analyzer.push_back(a);
	}
	ThreadPool pool(std::thread::hardware_concurrency());
	std::vector< std::future<std::string>> results;

	for (int i = 0; i < paths.size(); ++i) {
		results.emplace_back(
			pool.enqueue([&analyzer,i] {
			std::string result = analyzer.at(i).getResult();
			return result;
		})
		);
	}

	std::ofstream resultFile("test.txt");
	for (auto&& result : results)
		resultFile << result.get();

	return 0;
}

