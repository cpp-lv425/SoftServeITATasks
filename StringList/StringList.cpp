#include "stringlist.h"
#pragma warning(disable: 4996)

#include <iostream>

#include <stdlib.h>
#include <string.h>

void StringListInit(char*** list)
{
    // creating dummy node
    try
    {
         *list = NodeMemoryAllocation();
    } catch (const char*)
    {
        throw;
    }

    (*list)[0] = nullptr;
    (*list)[1] = nullptr;
}

char** NextNode(char** current)
{
    ++current;
    return reinterpret_cast<char**>(*current);
}

char** NodeMemoryAllocation()
{
    char** tmp = static_cast<char**>
            (malloc(sizeof(char**) * 2));

    if(!tmp)
    {
        throw "Failed to allocate memory\n";
    }
    return tmp;
}

void DeleteNode(char** del)
{
    // deleting string
    free(del[0]);
    // deleting node
    free(del);
}

void IsEmpty(char* ptr)
{
    if(ptr)
        return;

    throw "The list is empty\n";
}

void UntenableArgument(char* str)
{
    if(str)
        return;

    throw "Untenable argument\n";
}

void StringListDestroy(char*** list)
{
    // there are no nodes
    if (!list)
        return;

    char** current = *list;
    current = NextNode(current);
    char** del = *list;

    // deleting dummy node
    free(del);

    if (!current)
        return;

    // deleting nodes with data
    while (current)
    {
        del = reinterpret_cast<char**>(current);
        current = NextNode(current);
        DeleteNode(del);
    }
    *list = nullptr;
}

void StringListAdd(char** list, String str)
{
    // checks str for null
    try
    {
        UntenableArgument(str);
    } catch (const char*)
    {
        throw;
    }

    if (list[0] == nullptr)
    {
        // adding first node
        try
        {
            list[1] = reinterpret_cast<char*>(NodeMemoryAllocation());
        } catch (const char*)
        {
            // already checked str for null
            free(str);
            throw;
        }

        char** cur = reinterpret_cast<char**>(list[1]);
        cur[0] = str;
        cur[1] = nullptr;

        list[0] = list[1];
        return;
    }

    char** end = reinterpret_cast<char**>(list[0]);

    try
    {
        end[1] = reinterpret_cast<char*>(NodeMemoryAllocation());
    } catch (const char*)
    {
        // already checked str for null
        free(str);
        throw;
    }

    end = NextNode(end);

    // pushing node to the end of list
    end[0] = str;
    end[1] = nullptr;
    list[0] = reinterpret_cast<char*>(end);
}

void StringListRemove(char** list, String str)
{
    try
    {
        IsEmpty(list[1]);
    } catch (const char*)
    {
        if(str)
            free(str);
        throw;
    }

    try
    {
        UntenableArgument(str);
    } catch (const char*)
    {
        throw;
    }

    char** cur = reinterpret_cast<char**>(list[1]);
    char** del;
    char** prev = list;

    while (cur)
    {
        if (!strcmp(str, reinterpret_cast<char*>(cur[0])))
        {
            del = cur;
            cur = NextNode(cur);

            // prev is used reset pointers
            ++prev;
            *prev = reinterpret_cast<char*>(cur);

            DeleteNode(del);
            --prev;

            if(!cur)
                list[0] = reinterpret_cast<char*>(prev);
        }
        else
        {
            prev = cur;
            cur = NextNode(cur);
        }
    }
    // already checked str for null
    free(str);
}

int StringListSize(char** list)
{
    if (!list[0])
        return 0;

    int sizeCounter = 0;

    char** cur = reinterpret_cast<char**>(list[1]);
    while (cur)
    {
        ++sizeCounter;
        cur = NextNode(cur);
    }
    return sizeCounter;
}

int StringListIndexOf(char** list, char* str)
{
    try
    {
        IsEmpty(list[1]);
    } catch (const char*)
    {
        if(str)
            free(str);
        throw;
    }

    try
    {
        UntenableArgument(str);
    } catch (const char*)
    {
        throw;
    }

    int idx = 0;
    char** cur = reinterpret_cast<char**>(list[1]);

    while (cur)
    {
        if (!strcmp(str, reinterpret_cast<char*>(cur[0])))
        {
            // already checked str for null
            free(str);
            return idx;
        }

        ++idx;
        cur = NextNode(cur);
    }
    // already checked str for null
    free(str);

    // if string is not found in the list -1 is returned
    return -1;
}

void StringListRemoveDuplicates(char** list)
{
    try
    {
        IsEmpty(list[0]);
    } catch (const char*)
    {
        throw;
    }

    char** unique = reinterpret_cast<char**>(list[1]);
    char** current = reinterpret_cast<char**>(list[1]);
    char** prev = reinterpret_cast<char**>(list[1]);
    char** del;

    current = NextNode(current);

    while (true)
    {
        while (current)
        {
            if (!strcmp(reinterpret_cast<char*>(unique[0]),
                        reinterpret_cast<char*>(current[0])))
            {
                del = current;
                current = NextNode(current);

                ++prev;
                *prev = reinterpret_cast<char*>(current);

                DeleteNode(del);
                --prev;

                if(!current)
                {
                    list[0] = reinterpret_cast<char*>(prev);
                }
            }
            else
            {
                prev = current;
                current = NextNode(current);
            }
        }

        if (!(unique[1]))
            break;

        // unique moves to next item
        unique = NextNode(unique);
        current = unique;

        prev = current;
        current = NextNode(current);
    }
}

char* ReplaceSubstringInString(char* str, char* before, char* after)
{
    char* occur = strstr(str, before);

    size_t size = strlen(str - strlen(before)
                         + strlen(after)) + 1;

    char* newStr = static_cast<char*>
            (malloc(sizeof(char) * size));

    if(!newStr)
    {
        // before & after were checked for null earlier
        free(before);
        free(after);
        throw "Failed to construct string\n";
    }

    char* curCharStr = str;
    char* curCharNew = newStr;
    char* curCharAfter = after;

    while (curCharStr != occur)
    {
        *curCharNew++ = *curCharStr++;
    }
    while (*curCharAfter)
    {
        *curCharNew++ = *curCharAfter++;
    }
    curCharStr += strlen(before);
    while (*curCharStr)
    {
        *curCharNew++ = *curCharStr++;
    }
    *curCharNew = '\0';

    return newStr;
}

void StringListReplaceInStrings(char** list,
                                char* before, char* after)
{
    // throws exception if list is empty
    try
    {
        IsEmpty(list[0]);
    } catch (const char*)
    {
        if(before)
            free(before);
        if(after)
            free(after);
        throw;
    }

    // throws exception if any of passed strings is null
    try
    {
        UntenableArgument(before);
        UntenableArgument(after);
    } catch (const char*)
    {
        if(before)
            free(before);
        if(after)
            free(after);
        throw;
    }

    char** cur = reinterpret_cast<char**>(list[1]);

    while (cur)
    {
        while (strstr(reinterpret_cast<char*>(cur[0]), before))
        {
            char* newStr;
            try
            {
                char* del = *cur;
                newStr = ReplaceSubstringInString
                        (reinterpret_cast<char*>(cur[0]),
                        before, after);

                *cur = reinterpret_cast<char*>(newStr);
                free(del);

            } catch (const char*)
            {
                // before & after were checked for null earlier
                free(before);
                free(after);
                throw;
            }

        }
        cur = NextNode(cur);
    }
    // before & after were checked for null earlier
    free(before);
    free(after);
}

void StringListSort(char**& list)
{
    // if list is empty or it has 1 element
    if (!list || !list[1])
        return;

    char** tmpList;

    char** cur = reinterpret_cast<char**>(list[1]);
    char** next = reinterpret_cast<char**>(list[1]);

    next = NextNode(next);

    // adding first node to tmpList
    tmpList = cur;
    cur[1] = nullptr;
    cur = next;

    char** tmpCur, **tmpNext;

    while (cur)
    {
        next = NextNode(next);

        tmpCur = tmpList;
        tmpNext = tmpList;

        tmpNext = NextNode(tmpNext);

        if (strcmp(reinterpret_cast<char*>(tmpCur[0]), reinterpret_cast<char*>(cur[0])) != -1)
        {
            cur[1] = reinterpret_cast<char*>(tmpCur);
            tmpList = cur;
        }
        else
        {
            while (tmpNext && (strcmp(reinterpret_cast<char*>(tmpNext[0]),
                                      reinterpret_cast<char*>(cur[0])) == -1))
            {
                tmpCur = tmpNext;
                tmpNext = NextNode(tmpNext);
            }
            tmpCur[1] = reinterpret_cast<char*>(cur);
            cur[1] = reinterpret_cast<char*>(tmpNext);
        }
        cur = next;
    }

    list[1] = reinterpret_cast<char*>(tmpList);
    tmpList = nullptr;

    // traversing to the last node to write its address to list[0]
    cur = reinterpret_cast<char**>(list[1]);
    while (cur[1])
    {
        cur = NextNode(cur);
    }
    list[0] = reinterpret_cast<char*>(cur);
}

