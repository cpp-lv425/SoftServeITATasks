#include "StringList.h"
#pragma warning(disable: 4996)

void StringListInit(char***& list)
{
    // creating dummy node
    list = static_cast<char***>(malloc(sizeof(char**)));

    if (!list)
        exit(-1);

    *list = nullptr;
}

void StringListDestroy(char***& list)
{
    if (!list)
        return;

    char*** current = list;
    char*** del = list;
    current = reinterpret_cast<char***>(*current);

    // deleting dummy node
    free(del);

    if (!current)
        return;

    del = current;

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

        del = current;
    }
    list = nullptr;
}

void StringListAdd(char**& list, String str)
{
    if (!list)
    {
        // adding first node
        list = static_cast<char**>
                (malloc(sizeof(char**) * 2));

        if (!list)
        {
            if(str)
                free(str);
            return;
        }

        list[0] = str;
        list[1] = nullptr;
        return;
    }

    char*** cur = reinterpret_cast<char***>(list);
    while (true)
    {
        if (!cur[1])
        {
            ++cur;
            break;
        }
        ++cur = reinterpret_cast<char***>(*cur);
    }

    // creating new node
    *cur = static_cast<char**>
            (malloc(sizeof(char**) * 2));

    if (*cur == nullptr)
    {
        if (str)
            free(str);
        return;
    }

    cur = reinterpret_cast<char***>(*cur);
    // pushing node to the end of list
    cur[0] = reinterpret_cast<char**>(str);
    cur[1] = nullptr;
}

void StringListRemove(char**& list, String str)
{
    if (!list)
    {
        if(str)
            free(str);
        return;
    }

    char*** cur = reinterpret_cast<char***>(list);
    char*** del;

    // comparing first node
    while (!strcmp(str, reinterpret_cast<char*>(list[0])))
    {
        del = reinterpret_cast<char***>(list);
        ++cur = reinterpret_cast<char***>(*cur);

        // deleting string
        free(del[0]);
        // deleting node
        free(del);

        if (cur)
        {
            // binding to the rest of list
            list = reinterpret_cast<char**>(cur);
        }
        else
        {
            list = nullptr;
            if (str)
                free(str);
            return;
        }
    }

    char*** prev = reinterpret_cast<char***>(list);
    ++cur = reinterpret_cast<char***>(*cur);

    // comparing the rest of nodes
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
    if (!list)
        return 0;

    int counter = 0;

    char*** cur = reinterpret_cast<char***>(list);
    while (cur)
    {
        ++counter;
        ++cur = reinterpret_cast<char***>(*cur);
    }
    return counter;
}

int StringListIndexOf(char** list, char* str)
{
    // !!! if list is empty or str doesn't match any of
    // items function returns -1;
    if (!list)
    {
        if(str)
            free(str);
        return -1;
    }

    int counter = 0;
    char*** cur = reinterpret_cast<char***>(list);

    while (cur)
    {
        if (!strcmp(str, reinterpret_cast<char*>(cur[0])))
        {
            if(str)
                free(str);
            return counter;
        }

        ++counter;
        ++cur = reinterpret_cast<char***>(*cur);
    }
    if(str)
        free(str);
    return -1;
}

void StringListRemoveDuplicates(char**& list)
{
    if (!list)
        return;

    char*** unique = reinterpret_cast<char***>(list);
    char*** current = reinterpret_cast<char***>(list);
    char*** prev = reinterpret_cast<char***>(list);
    char*** del;

    ++current = reinterpret_cast<char***>(*current);

    while (true)
    {
        while (current)
        {
            if (!strcmp(reinterpret_cast<char*>(unique[0]), reinterpret_cast<char*>(current[0])))
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
    // returns if list is empty or passed strings (before, after) are null
    if (!list || !before || !after)
    {
        if(before)
            free(before);
        if(after)
            free(after);
        return;
    }

    char*** cur = reinterpret_cast<char***>(list);

    while (cur)
    {
        while (strstr(reinterpret_cast<char*>(cur[0]), before))
        {
            char* occur = strstr(reinterpret_cast<char*>(cur[0]), before);
            size_t size = strlen(reinterpret_cast<char*>(cur[0])) - strlen(before) + strlen(after) + 1;
            char* newStr = static_cast<char*>(malloc(sizeof(char) * size));
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

            strcpy(reinterpret_cast<char*>(cur[0]), newStr);
            free(newStr);
        }
        ++cur = reinterpret_cast<char***>(*cur);
    }
    if(before)
        free(before);
    if(after)
        free(after);
}

void StringListSort(char**& list)
{
    if (!list || !list[1])
        return;

    char*** tmpList;

    char*** cur = reinterpret_cast<char***>(list);
    char*** next = reinterpret_cast<char***>(list);

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

    list = reinterpret_cast<char**>(tmpList);
    tmpList = nullptr;
    free(tmpList);
}

