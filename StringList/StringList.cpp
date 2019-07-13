#include "stringlist.h"
#pragma warning(disable: 4996)

#include <stdlib.h>
#include <string.h>

void StringListInit(char*** list)
{
    // creating dummy node
    *list = static_cast<char**>(malloc(sizeof(char**) * 2));

    if (*list == nullptr)
        exit(-1);

    char** cur = *list;
    cur[0] = nullptr;
    cur[1] = nullptr;
}

void StringListDestroy(char*** list)
{
    if (!list)
        return;

    char*** current = reinterpret_cast<char***>(*list);
    ++current = reinterpret_cast<char***>(*current);
    char** del = *list;

    // deleting dummy node
    free(del);

    if (!current)
        return;

    del = reinterpret_cast<char**>(current);

    // deleting nodes with data
    while (true)
    {
        // moving to next node
        ++current = reinterpret_cast<char***>(*current);

        // deleting string
        free(del[0]);
        // deleting node
        free(del);

        if (!current)
            break;

        del = reinterpret_cast<char**>(current);
    }
    list = nullptr;
}

void StringListAdd(char** list, String str)
{
    if (list[0] == nullptr)
    {
        // adding first node
        list[1] = static_cast<char*>
                (malloc(sizeof(char**) * 2));

        if (!list[1])
        {
            if(str)
                free(str);
            throw "Failed to allocate memory\n";
        }

        char** cur = reinterpret_cast<char**>(list[1]);
        cur[0] = str;
        cur[1] = nullptr;
        list[0] = list[1];
        return;
    }

    char*** end = reinterpret_cast<char***>(list[0]);
    end[1] = static_cast<char**>
            (malloc(sizeof(char**) * 2));

    if (end[1] == nullptr)
    {
        if (str)
            free(str);
        throw "Failed to allocate memory\n";
    }

    ++end = reinterpret_cast<char***>(*end);
    // pushing node to the end of list
    end[0] = reinterpret_cast<char**>(str);
    end[1] = nullptr;
    list[0] = reinterpret_cast<char*>(end);
}

void StringListRemove(char** list, String str)
{
    if (!list[1])
    {
        if(str)
            free(str);
        throw "The list is empty\n";
    }

    char*** cur = reinterpret_cast<char***>(list[1]);
    char*** del;

    char*** prev = reinterpret_cast<char***>(list);

    while (cur)
    {
        if (!strcmp(str, reinterpret_cast<char*>(cur[0])))
        {
            del = cur;
            ++cur = reinterpret_cast<char***>(*cur);

            // prev is used reset pointers
            ++prev;
            *prev = reinterpret_cast<char**>(cur);

            // deleting string
            free(del[0]);
            // deleting node
            free(del);
            --prev;

            if(!cur)
                list[0] = reinterpret_cast<char*>(prev);
        }
        else
        {
            prev = cur;
            ++cur = reinterpret_cast<char***>(*cur);
        }
    }
    if (str)
        free(str);
}

int StringListSize(char** list)
{
    if (!list[0])
        return 0;

    int sizeCounter = 0;

    char*** cur = reinterpret_cast<char***>(list[1]);
    while (cur)
    {
        ++sizeCounter;
        ++cur = reinterpret_cast<char***>(*cur);
    }
    return sizeCounter;
}

int StringListIndexOf(char** list, char* str)
{    

    if (!list[1])
    {
        if(str)
            free(str);
        throw "The list is empty\n";
    }

    int idx = 0;
    char*** cur = reinterpret_cast<char***>(list[1]);

    while (cur)
    {
        if (!strcmp(str, reinterpret_cast<char*>(cur[0])))
        {
            if(str)
                free(str);
            return idx;
        }

        ++idx;
        ++cur = reinterpret_cast<char***>(*cur);
    }
    if(str)
        free(str);

    // if string is not found in the list -1 is returned
    return -1;
}

void StringListRemoveDuplicates(char** list)
{
    if (!list[0])
        throw "The list is empty\n";

    char*** unique = reinterpret_cast<char***>(list[1]);
    char*** current = reinterpret_cast<char***>(list[1]);
    char*** prev = reinterpret_cast<char***>(list[1]);
    char*** del;

    ++current = reinterpret_cast<char***>(*current);

    while (true)
    {
        while (current)
        {
            if (!strcmp(reinterpret_cast<char*>(unique[0]),
                        reinterpret_cast<char*>(current[0])))
            {
                del = current;
                ++current = reinterpret_cast<char***>(*current);

                ++prev;
                *prev = reinterpret_cast<char**>(current);

                // deleting string
                free(del[0]);
                // deleting node
                free(del);
                --prev;

                if(!current)
                    list[0] = reinterpret_cast<char*>(prev);
            }
            else
            {
                prev = current;
                ++current = reinterpret_cast<char***>(*current);
            }
        }

        if (!unique[1])
            break;

        // unique moves to next item
        ++unique = reinterpret_cast<char***>(*unique);

        current = reinterpret_cast<char***>(unique);
        prev = current;
        ++current = reinterpret_cast<char***>(*current);
    }
}

void StringListReplaceInStrings(char** list,
                                char* before, char* after)
{
    // throws exception if list is empty
    if(!list[0])
    {
        if(before)
            free(before);
        if(after)
            free(after);
        throw "The list is empty\n";
    }

    // throws exception if any of passed strings is null
    if (!before || !after)
    {
        if(before)
            free(before);
        if(after)
            free(after);
        throw "Untenable arguments\n";
    }

    char*** cur = reinterpret_cast<char***>(list[1]);

    while (cur)
    {
        while (strstr(reinterpret_cast<char*>(cur[0]), before))
        {
            char* occur = strstr(reinterpret_cast<char*>
                                 (cur[0]), before);
            size_t size = strlen(reinterpret_cast<char*>
                                 (cur[0])) - strlen(before) +
                    strlen(after) + 1;
            char* newStr = static_cast<char*>
                    (malloc(sizeof(char) * size));

            if(!newStr)
            {
                free(before);
                free(after);
                throw "Failed to construct string\n";
            }

            char* curCharStr = reinterpret_cast<char*>(cur[0]);
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

            size = strlen(newStr) + 1;

            cur[0] = static_cast<char**>
                    (realloc(cur[0], sizeof(char) * size));

            if(!cur[0])
            {
                free(before);
                free(after);
                throw "Failed to construct string\n";
            }

            strcpy(reinterpret_cast<char*>(cur[0]), newStr);
            free(newStr);
        }
        ++cur = reinterpret_cast<char***>(*cur);
    }

    free(before);
    free(after);
}

void StringListSort(char**& list)
{
    if (!list || !list[1])
        return;

    char*** tmpList;

    char*** cur = reinterpret_cast<char***>(list[1]);
    char*** next = reinterpret_cast<char***>(list[1]);

    ++next = reinterpret_cast<char***>(*next);

    // adding first node to tmpList
    tmpList = cur;
    cur[1] = nullptr;
    cur = next;

    char*** tmpCur;
    char*** tmpNext;

    while (cur)
    {
        ++next = reinterpret_cast<char***>(*next);

        tmpCur = tmpList;
        tmpNext = tmpList;

        ++tmpNext = reinterpret_cast<char***>(*tmpNext);

        if (strcmp(reinterpret_cast<char*>(tmpCur[0]), reinterpret_cast<char*>(cur[0])) != -1)
        {
            cur[1] = reinterpret_cast<char**>(tmpCur);
            tmpList = cur;
        }
        else
        {
            while (tmpNext && (strcmp(reinterpret_cast<char*>(tmpNext[0]), reinterpret_cast<char*>(cur[0])) == -1))
            {
                tmpCur = tmpNext;
                ++tmpNext = reinterpret_cast<char***>(*tmpNext);
            }
            tmpCur[1] = reinterpret_cast<char**>(cur);
            cur[1] = reinterpret_cast<char**>(tmpNext);
        }
        cur = next;
    }

    list[1] = reinterpret_cast<char*>(tmpList);
    tmpList = nullptr;
    cur = reinterpret_cast<char***>(list[1]);

    while (cur[1])
    {
        ++cur = reinterpret_cast<char***>(*cur);
    }
    list[0] = reinterpret_cast<char*>(cur);
}

