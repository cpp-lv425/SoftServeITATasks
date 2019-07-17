#ifndef STRINGLIST_H
#define STRINGLIST_H

using String = char*;
using List = char **;

//allocate memory for new list
List initialization();

void destroy(List &list);

void push(List &list,const String &string);

void remove(List &list, const String &string);

int size(const List &list);

int index(const List &list, const String &string);

void deleteDuplicates(List &list);

void replaceStrings(List &list, const String &first, const String &second);

void sort(List &list);

#endif // STRINGLIST_H
