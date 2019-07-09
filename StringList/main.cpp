#include <iostream>

// includes implementation of the StringList
#include "stringlist.h"

// includes functions which make it easier to use
// the functionality of StringList (e.g. to output the list)
#include "helperfunctions.h"

using std::cout;

int main()
{
    char** StringList = nullptr;
    StringListInit(&StringList);

    while (true)
    {
        switch (MainMenu())
        {
        //        1. Add string to the list.
        //        2. Removes all occurrences of string in the list.
        //        3. Print the number of items in the list.
        //        4. Returns the index position of the first occurrence of string in the list.
        //        5. Removes duplicate entries from the list.
        //        6. Replaces every occurrence of string_1, in each of the string lists's strings, with string_2.
        //        7. Sorts the list of strings in ascending order.
        //        8. Print the list.
        //        9. Quit.
        case 1:
        {
            cout << "Enter new string: ";
            char* newStr = InputString();
            StringListAdd(StringList, newStr);
            TraverseList(StringList);
            break;
        }
        case 2:
        {
            cout << "Enter a string occurrences of which must be removed from the list: ";
            char* str = InputString();
            StringListRemove(StringList, str);
            TraverseList(StringList);
            break;
        }
        case 3:
        {
            cout << "Number of strings in the list: "
                 << StringListSize(StringList)
                 << '\n';
            break;
        }
        case 4:
        {
            cout << "Enter a string to find out its index in the list: ";
            char* str = InputString();
            int idx = StringListIndexOf(StringList, str);
            if(idx == -1)
                cout << "There is no such string in the list.\n";
            else
                cout << "Index of the string: " << idx << '\n';

            break;
        }
        case 5:
        {
            StringListRemoveDuplicates(StringList);
            cout << "Duplicates removed.\n";
            TraverseList(StringList);
            break;
        }
        case 6:
        {
            cout << "Enter a sub-string which must be replaced in list: ";
            char* before = InputString();
            cout << "Enter a sub-string which must be put instead of previous sub-string: ";
            char* after = InputString();
            StringListReplaceInStrings(StringList, before, after);
            TraverseList(StringList);
            break;
        }
        case 7:
        {
            cout << "List has been sorted.\n";
            StringListSort(StringList);
            TraverseList(StringList);
            break;
        }
        case 8:
        {
            TraverseList(StringList);
            break;
        }
        case 9:
        {
            StringListDestroy(&StringList);
            return 0;
        }
        }

    }
}
