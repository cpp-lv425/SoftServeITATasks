#include"StringParse.h"

void RemoveVowels(std::string &str)
{
	std::regex vowels("\"(?:\\\\\"|.)*?\"");
	std::stringstream withoutVowels();
	std::string result;
	std::regex_replace(std::back_inserter(result), str.begin(),
		str.end(), vowels, "$2");
	str = result;
}

std::string RemoveSpaces(std::string &str)
{
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	str.erase(remove(str.begin(), str.end(), '\t'), str.end());
	return str;
}

int CountOfSubStrings(std::string source, std::string lexeme)
{
	int lexeme_count = 0;
	for (std::size_t pos = 0; pos < source.size(); pos += lexeme.size())
	{
		pos = source.find(lexeme, pos);
		if (pos != std::string::npos)
		{
			++lexeme_count;
		}
		else
		{
			break;
		}
	}
	return lexeme_count;
}

bool IsLeftComment(char firstSymbol, char secondSymbol, std::string str)// be sure that string isn't empty
{
	auto it = str.begin();
	if ((it + 1) == str.end())//one symbol int the string
		return false;
	return ((*it == firstSymbol) && (*(it + 1) == secondSymbol));
}

std::string RemoveMultiCommentSymbols(std::string &str)
{
	auto i = str.begin();
	while ((i + 1) != str.end() && i != str.end())
	{
		if ((*i == '*' && *(i + 1) == '/') ||
			*i == '/' && *(i + 1) == '*')
		{
			str.erase(i);
			str.erase(i);
			if (i == str.end())
				break;
		}
		else
			i++;
	}
	return str;
}

bool IsRightComment(std::string str)
{
	auto it = str.end() - 1;
	if (it == str.begin())//only one symbol in the string
		return false;
	else
		return ((*(it - 1) == '*') && (*it == '/'));
}

void RemoveAllBetweenMultiComment(std::string &str)// here we know that /**/ are alway one by each other
{
	if (!(CountOfSubStrings(str, "/*")))
		return;
	auto i = str.begin();
	while (true)
	{
		if (i == str.end())
			break;
		while (*i != '/' || *(i + 1) != '*')
		{
			i++;
			if (i + 1 == str.end())
				return;
		}
		i += 2;
		if (i == str.end())
			return;
		while (i != str.end())
		{
			if (*i == '*' && *(i + 1) == '/')
				break;
			str.erase(i);
		}
	}
}

bool IsAnyCodeLeftWardSimpleComment(std::string str)
{
	RemoveMultiCommentSymbols(str);
	return !IsLeftComment('/', '/', str);
}


bool IsAnyCodeInTheMultiCommStringOpen(std::string str)
{
	RemoveMultiCommentSymbols(str);
	return !str.empty();
}

bool IsAnyCodeInTheMultiCommStringClose(std::string &str)
{
	auto it = str.begin();
	while (*it != '*' && *(it + 1) != '/')
		str.erase(it);
	RemoveMultiCommentSymbols(str);
	return !str.empty();
}