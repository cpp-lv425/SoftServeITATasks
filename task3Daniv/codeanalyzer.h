#ifndef CODEANALYZER_H
#define CODEANALYZER_H

#include <string>
#include <vector>

class CodeAnalyzer
{
public:
    CodeAnalyzer(const std::string &code);

    std::string code() const;

    std::vector<std::string> comments() const;

private:
    std::string mCode;
    std::vector<std::string> mComments;

    void calculateComments();
};

#endif // CODEANALYZER_H
