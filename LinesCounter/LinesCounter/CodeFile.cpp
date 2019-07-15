#include "CodeFile.h"

CodeFile::CodeFile(fs::path t_path)
{
	m_file.open(t_path);
	if(!m_file)
		throw std::runtime_error("Could not open file");
	m_processedLinesCount = 0;
	m_blankLinesCount = 0;
	m_commentLinesCount = 0;
	m_physicalLinesCount = 0;
	m_pathName = t_path.u8string();
}

CodeFile::CodeFile(CodeFile && obj)
{
	this->m_file = std::move(obj.m_file);
	m_processedLinesCount = obj.m_processedLinesCount;
	m_blankLinesCount = obj.m_blankLinesCount;
	m_commentLinesCount = obj.m_commentLinesCount;
	m_physicalLinesCount = obj.m_physicalLinesCount;
	m_pathName = obj.m_pathName;
}

CodeFile::CodeFile(const CodeFile & obj)
{
	m_processedLinesCount = obj.m_processedLinesCount;
	m_blankLinesCount = obj.m_blankLinesCount;
	m_commentLinesCount = obj.m_commentLinesCount;
	m_physicalLinesCount = obj.m_physicalLinesCount;
	m_pathName = obj.m_pathName;
}

CodeFile::~CodeFile()
{
	m_file.close();
}

string CodeFile::GetPathName()
{
	return m_pathName;
}

void CodeFile::FileClose()
{
	m_file.close();
}

int CodeFile::GetProcessedCount()
{
	return m_processedLinesCount;
}

int CodeFile::GetPhysicalCount()
{
	return m_physicalLinesCount;
}

int CodeFile::GetBlankCount()
{
	return m_blankLinesCount;
}

int CodeFile::GetCommentCount()
{
	return m_commentLinesCount;
}

void CodeFile::FileRead()
{
	string currentLine;
	bool notClosedComplicatedComment = false;// condition var. if == true that means we haven't closed multiline comment
	while (true)
	{
		getline(m_file, currentLine);// read string
		if (m_file.eof())// if we have read all the file
			break;
		m_processedLinesCount++;
		if (!notClosedComplicatedComment)// if we are not inside multiline comment section
		{
			RemoveSpaces(currentLine);
			RemoveVowels(currentLine);
			RemoveAllBetweenMultiComment(currentLine);

			if (currentLine.empty())// empty line (only \n)
			{
				m_blankLinesCount++;
				continue;
			}
			if (CountOfSubStrings(currentLine, "//"))// firstly, we should try to find this kind of comment
			{
				m_commentLinesCount++;
				if (IsAnyCodeLeftWardSimpleComment(currentLine))// if // is somewhere, but not at the start
					m_physicalLinesCount++;
				continue;
			}

			if (CountOfSubStrings(currentLine, "/*"))
			{
				m_commentLinesCount++;// statment above is true therefore this line has comment
				if ((CountOfSubStrings(currentLine, "*/")) &&
					(CountOfSubStrings(currentLine, "*/") == CountOfSubStrings(currentLine, "/*")))// check if we closed /* comment in this line
				{
					if (!(IsAnyCodeInTheMultiCommStringOpen(currentLine)))// this is only one situation where our line is fully comment line FE: /*comment*/
						continue;
					m_physicalLinesCount++;// if we found /*   and */  but we also have some code outside comment scope. FE: /*comment*/code;
				}
				else
				{
					notClosedComplicatedComment = true;// we've opened /* scope but haven't closed. So next line(s) will be comment(s)
					if (IsAnyCodeInTheMultiCommStringOpen(currentLine))// we should check here again because this is situation where we don't have closed */
						m_physicalLinesCount++;//FE: code;/*
				}
				continue;
			}
			else
			{
				m_physicalLinesCount++;// if anything above was false the line is just simple code line
			}
		}
		else// we were sent here because multilines comment is still opened
		{
			m_commentLinesCount++;
			if (CountOfSubStrings(currentLine, "*/"))// check if we have */ in the line. If not it's still comment
			{
				if ((CountOfSubStrings(currentLine, "*/")) == (CountOfSubStrings(currentLine, "/*") + 1))// check for this situation: */ ..... /*
				{
					notClosedComplicatedComment = false;// if anything is ok and this we closed the comment scope
				}
				if ((IsAnyCodeInTheMultiCommStringClose(currentLine)))// check if our line consist any code
				{
					m_physicalLinesCount++;
				}
			}
		}
	}
}

std::ostream & operator<<(std::ostream & out, const CodeFile & codeFile)
{
	out << "File:"<<codeFile.m_pathName <<"\n"
		"Total process lines count = " << codeFile.m_processedLinesCount <<   "\n"
		<< "Total physical lines count = " << codeFile.m_physicalLinesCount <<"\n"
		<< "Total comment lines count = " << codeFile.m_commentLinesCount <<  "\n"
		<< "Total blank lines count = " << codeFile.m_blankLinesCount << "     \n";
	return out;
}