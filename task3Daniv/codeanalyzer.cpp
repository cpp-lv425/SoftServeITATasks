#include "codeanalyzer.h"

#include <regex>
#include <sstream>
#include <iostream>
#include <algorithm>

CodeAnalyzer::CodeAnalyzer(const std::string &code)
{
    mCode = code;

    mCode.erase(std::remove(mCode.begin(), mCode.end(), '\t'), mCode.end());
    mCode.erase(std::remove(mCode.begin(), mCode.end(), '\v'), mCode.end());
    mCode.erase(std::remove(mCode.begin(), mCode.end(), ' '), mCode.end());
}

std::string CodeAnalyzer::code() const
{
    return mCode;
}

std::vector<std::string> CodeAnalyzer::comments() const
{
    return mComments;
}

void CodeAnalyzer::calculateComments()
{
    std::string str = mCode;

    //pattern to detect all "strings"
    const std::regex vowels("\"(?:\\\\\"|.)*?\"");

    std::stringstream result;
    std::regex_replace(std::ostream_iterator<char>(result),str.begin(), str.end(), vowels, "");
    str = result.str();

    //pattern to detect all comments
    const std::string pattern("(/\\*([^*]|[\r\n]|(\\*+([^*/]|[\r\n])))*\\*+/)|(//.*)");
    std::regex r(pattern, std::regex_constants::egrep);

    for (std::sregex_iterator it(str.begin(), str.end(), r), end; it != end; ++it)
    {
        std::cout<<it->str();
        mComments.push_back(it->str());
    }
}
