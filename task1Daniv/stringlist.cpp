#include "stringlist.h"

#include <stdlib.h>
#include <string.h>

const int dataIndex = 0;
const int nextNodeIndex = 1;

List initialization()
{
    List temp = static_cast<char**>(malloc(sizeof(List) * 2));

    if(temp != nullptr)
    {
        temp[dataIndex] = nullptr;
        temp[nextNodeIndex] = nullptr;
    }
    return  temp;
}

void push(List &list, const String &string)
{
    if(list == nullptr)
    {
        return;
    }
    String textStr = static_cast<String>(malloc(sizeof(char) * strlen(string) + 1));
    strcpy(textStr, string);

    if(list[dataIndex] == nullptr)
    {
        list[dataIndex] = textStr;
    }
    else
    {
        auto temp = list;
        while(list[nextNodeIndex] != nullptr)
        {
            list = reinterpret_cast<List>(list[nextNodeIndex]);
        }
        List node = static_cast<List>(malloc(sizeof(char*) * 2));
        node[dataIndex] = textStr;
        node[nextNodeIndex] = nullptr;
        list[nextNodeIndex] = reinterpret_cast<String>(node);
        list = temp;
    }
}

int size(const List &list)
{
    if(list == nullptr)
    {
        return -1;
    }

    int totalSize = 0;
    auto temp = list;
    while (temp != nullptr && temp[dataIndex] != nullptr)
    {
        totalSize++;
        temp = reinterpret_cast<List>(temp[nextNodeIndex]);
    }
    return totalSize;
}

int index(const List &list, const String &string)
{
    if(list == nullptr || string == nullptr)
    {
        return -1;
    }

    auto temp = list;

    for(int i = 0; i != size(list); i++)
    {
        if(strcmp(string,temp[dataIndex])==0)
        {
            return i;
        }
        temp = reinterpret_cast<List>(temp[nextNodeIndex]);
    }
    return -1;

}

void destroy(List &list)
{
    if(list == nullptr)
    {
        return;
    }

    for(int i = 0; i != size(list); ++i)
    {
          List temp = reinterpret_cast<List>(list[nextNodeIndex]);
          free(list[dataIndex]);
          free(list);
          list = temp;
    }

    list = nullptr;
}

void remove(List &list, const String &string)
{
    if(list == nullptr || string == nullptr)
    {
        return;
    }

    int stringIndex = index(list,string);

    if(stringIndex < 0 )
    {
        return;
    }
    auto temp = list;
    auto prev = list;
    if(stringIndex == 0 && size(list) != 0)
    {
        free(list[dataIndex]);
        list = reinterpret_cast<List>(list[nextNodeIndex]);
        remove(list,string);
    }
    else if(stringIndex == 0 && size(list) == 1)
    {
        free(list[dataIndex]);
        free(list);
        list = nullptr;
    }
    else
    {
        for(int i = 0; i < stringIndex; ++i)
        {
            prev = temp;
            temp = reinterpret_cast<List>(temp[nextNodeIndex]);
        }

        prev[nextNodeIndex] = temp[nextNodeIndex];
        free(temp[dataIndex]);
        free(temp);
        remove(list,string);
    }
}

void sort(List &list)
{
    if(list == nullptr)
    {
        return;
    }
    int listSize = size(list);

    if(listSize <= 1)
    {
        return;
    }
    else
    {
        auto temp = list;
        for(int i = 0; i < size(list) - 1; ++i)
        {
            auto temp2 = temp;
            String min = temp[dataIndex];
            for(int j = i+1; j < size(list); ++j)
            {
                temp2 = reinterpret_cast<List>(temp2[nextNodeIndex]);
                if(strcmp(temp2[dataIndex],min)<0)
                {
                    min = temp2[dataIndex];
                }
            }
            replaceStrings(list,temp[dataIndex],min);
            temp = reinterpret_cast<List>(temp[nextNodeIndex]);
        }
    }
}

void replaceStrings(List &list, const String &first, const String &second)
{
    if(list == nullptr || first == nullptr || second == nullptr)
    {
        return;
    }
    int indexFirst = index(list,first);

    if(indexFirst == -1)
    {
        return;
    }

    auto firstNode = list;

    for(int i = 0; i < indexFirst; ++i)
    {
        firstNode = reinterpret_cast<List>(firstNode[nextNodeIndex]);
    }

    free(firstNode[dataIndex]);

    String textStr = static_cast<String>(malloc(sizeof(char) * strlen(second) + 1));
    strcpy(textStr, second);

    firstNode[dataIndex] = textStr;
    replaceStrings(list,first,second);
}

void deleteDuplicates(List &list)
{
    if(list == nullptr || size(list) <= 1)
    {
        return;
    }

    else
    {
        auto temp = list;
        for(int i = 0; i < size(list) - 1; ++i)
        {
            auto temp2 = temp;
            String tempString = temp[dataIndex];
            for(int j = i+1; j < size(list); ++j)
            {
                temp2 = reinterpret_cast<List>(temp2[nextNodeIndex]);
                if(strcmp(temp2[dataIndex],tempString) == 0)
                {
                    remove(list,tempString);
                }
            }
            temp = reinterpret_cast<List>(temp[nextNodeIndex]);
        }
    }
}
