#include <regex>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main()
{
    //Read file to string
    ifstream in("SourceCode.cpp", ios::in);
    istreambuf_iterator<char> beg(in), end;
    string str(beg, end);
    in.close();

    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\v'), str.end());
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    std::cout << str;

    std::string codeString = str;
    //remove " "
    const std::regex vowels("\"(?:\\\\\"|.)*?\"");

    std::stringstream result;
    std::regex_replace(std::ostream_iterator<char>(result),str.begin(), str.end(), vowels, "");
    str = result.str();

    //remove comments and push in vector
    string pattern("(/\\*([^*]|[\r\n]|(\\*+([^*/]|[\r\n])))*\\*+/)|(//.*)");
    regex r(pattern, regex_constants::egrep);
    std::vector<std::string> comments;
    for (sregex_iterator it(str.begin(), str.end(), r), end; it != end; ++it)
    {
        comments.push_back(it->str());
    }
    std::cout<<comments.size();
}
