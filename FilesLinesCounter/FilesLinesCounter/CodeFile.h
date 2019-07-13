#include<string>
#include<fstream>
#include<filesystem>
#include"StringParse.h"
#include<iostream>
#include<regex>
#include<Windows.h>
namespace fs = std::experimental::filesystem;
using namespace std;

class CodeFile
{
private:
	int m_processedCount;
	int m_blankCount;
	int m_commentCount;
	int m_physicalCount;
	ifstream m_file;
	fs::path m_path;
public:
	CodeFile(fs::path t_path)
	{
		m_file.open(t_path);
		m_processedCount = 0;
		m_blankCount = 0;
		m_commentCount = 0;
		m_physicalCount = 0;
		m_path = t_path;
	}

	CodeFile(CodeFile &&obj)
	{
		this->m_file = std::move(obj.m_file);
		m_processedCount = obj.m_processedCount;
		m_blankCount = obj.m_blankCount;
		m_commentCount = obj.m_commentCount;
		m_physicalCount = obj.m_physicalCount;
		m_path = obj.m_path;
	}

	CodeFile(const CodeFile &obj)
	{
		m_processedCount = obj.m_processedCount;
		m_blankCount = obj.m_blankCount;
		m_commentCount = obj.m_commentCount;
		m_physicalCount = obj.m_physicalCount;
		m_path = obj.m_path;
	}
	~CodeFile()
	{
		m_file.close();
	}

	string GetPathName()
	{
		return m_path.u8string();
	}

	void FileClose()
	{
		m_file.close();
	}

	int GetProcessedCount()
	{
		return m_processedCount;
	}

	int GetPhysicalCount()
	{
		return m_physicalCount;
	}

	int GetBlankCount()
	{
		return m_blankCount;
	}

	int GetCommentCount()
	{
		return m_commentCount;
	}

	void RemoveVowels(std::string &str)
	{
		std::regex vowels("\"(?:\\\\\"|.)*?\"");
		std::stringstream withoutVowels();
		std::string result;
		std::regex_replace(std::back_inserter(result), str.begin(),
			str.end(), vowels, "$2");
		str = result;
	}

	string RemoveSpaces(string &str)
	{
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
		str.erase(remove(str.begin(), str.end(), '\t'), str.end());
		return str;
	}

	void FileRead()
	{
		int k = 0;/*fdj//fdfd*/ 
		string currentLine;
		bool notClosedComplicatedComment = false;
		while (true)
		{
			//cout << "Line: " << m_processedCount << endl;
			//Sleep(100);
			/*int j;/*55566*/
			getline(m_file, currentLine);
			if (m_file.eof())
				break;
			m_processedCount++;
			if (!notClosedComplicatedComment)
			{
				RemoveSpaces(currentLine);
				RemoveVowels(currentLine);
				RemoveAllBetweenMultiComment(currentLine);

				if (currentLine.empty())// empty line (only \n)
				{
					m_blankCount++;
					continue;
				}
				if (CountOfSubStrings(currentLine,"//"))
				{
					m_commentCount++;
					if (IsAnyCodeLeftWardSimpleComment(currentLine))// if // is somewhere, but not at the start
						m_physicalCount++;
					continue;
				}
				
				if (CountOfSubStrings(currentLine,"/*"))
				{
					m_commentCount++;// statment above is true therefore this line has comment
					if ((CountOfSubStrings(currentLine, "*/")) && 
						(CountOfSubStrings(currentLine, "*/") == CountOfSubStrings(currentLine, "/*")))// check if we closed /* comment in this line
					{
						if (!(IsAnyCodeInTheMultiCommStringOpen(currentLine)))// this is only one situation where our line is fully comment line FE: /*comment*/
							continue;
						m_physicalCount++;// if we found /*   and */  but we also have some code outside comment scope. FE: /*comment*/code;
					}
					else
					{
						notClosedComplicatedComment = true;// we've opened /* scope but haven't closed. So next line(s) will be comment(s)
						if (IsAnyCodeInTheMultiCommStringOpen(currentLine))// we should check here again because this is situation where we don't have closed */
							m_physicalCount++;//FE: code;/*
					}
					continue;
				}
				else
				{
					m_physicalCount++;
				}
			}
			else// we were sent here because multilines comment is still opened
			{
				m_commentCount++;
				if (CountOfSubStrings(currentLine, "*/"))
				{
					if((CountOfSubStrings(currentLine, "*/")) == (CountOfSubStrings(currentLine, "/*") +1))
					{
						notClosedComplicatedComment = false;
					}
					if((IsAnyCodeInTheMultiCommStringClose(currentLine)))
					{
						m_physicalCount++;
					}
				}
			}
		}
	}
	friend std::ostream& operator<<(std::ostream &out, const CodeFile &codeFile);
};

std::ostream& operator<<(std::ostream &out, CodeFile &codeFile)
{
	out << "File:"<<codeFile.GetPathName()<<"\n"
		"Total process lines count = " << codeFile.GetProcessedCount() << "\n"
		<< "Total physical lines count = " << codeFile.GetPhysicalCount() <<"\n"
		<< "Total comment lines count = " << codeFile.GetCommentCount() << "\n"
		<< "Total blank lines count = " << codeFile.GetBlankCount() << "\n";
	return out;
}