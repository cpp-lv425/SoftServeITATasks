#pragma once
#include<string>
#include<iostream>
#include<iterator>
#include<regex>

void RemoveVowels(std::string &str);// remove any text between " ". In order to not count comment sighs which aren't actually comment

std::string RemoveSpaces(std::string &str);

int CountOfSubStrings(std::string source, std::string lexeme);

bool IsLeftComment(char firstSymbol, char secondSymbol, std::string str);// check if 1-st symb and 2-nd symb are the [0] and [1] in accordance 

std::string RemoveMultiCommentSymbols(std::string &str);//remove any /* or */ symbols. This is helpful when we wanna check if our string 
// has only comments or maybe some code. FE: comment can be like: /**//**//**/. And where we remove any of that we'll is there any code or no

bool IsRightComment(std::string str);// if *\ is in the end of string (comment is closed)

void RemoveAllBetweenMultiComment(std::string &str);// remove anything between /*...*/. In the final we'll have any of multicomment like /**/ and
//is so much easier to check our string when we exactly know that all multi comment are next to each other

bool IsAnyCodeLeftWardSimpleComment(std::string str);// check if there are any symbols leftward comment symbols. this check for comments like //

bool IsAnyCodeInTheMultiCommStringOpen(std::string str);// the similar as the above, but here we check strings where /* is the part of it

bool IsAnyCodeInTheMultiCommStringClose(std::string &str);// the similar, but here we check where multicomment is closed. */