#include "helperFunctions.h"
#include <iostream>
#include <limits>
#include <cstring>

using std::cout;
using std::cin;
using std::ios_base;


void TraverseList(char** list)
{
    std::cout << "The list consists of the following items:\n\n";
    char*** cur = reinterpret_cast<char***>(list);

    for (int i = 1; cur;
         ++cur = reinterpret_cast<char***>(*cur), ++i)
    {
        std::cout << "Item No. " << i
                  << '\t' << reinterpret_cast<char*>(cur[0])
                << '\n';
    }

    std::cout << "\n\n";
}

int PromptIntInRange(int start, int end)
{
    int tmp = 0;
    cin.exceptions(ios_base::failbit | ios_base::badbit);
    do
    {
        try
        {
            cout << ">> ";
            cin >> tmp;
            if (tmp < start || tmp > end)
                throw std::exception();
            break;
        }
        catch (ios_base::failure& e)
        {
            cout << "\nIncorrect input. Enter an integer.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        catch (const std::exception& e)
        {
            cout << "\nIncorrect input. Enter number from "
                 << start << " through " << end << std::endl;
        }

    } while (true);

    while (cin.get() != '\n')
        continue;

    return tmp;
}


int MainMenu()
{
    std::cout << "\n1. Add string to the list.\n"
              << "2. Removes all occurrences of string in the list.\n"
              << "3. Print the number of items in the list.\n"
              << "4. Returns the index position of the first occurrence of string in the list.\n"
              << "5. Removes duplicate entries from the list.\n"
              << "6. Replaces every occurrence of string_1, in each of the string lists's strings, with string_2.\n"
              << "7. Sorts the list of strings in ascending order.\n"
              << "8. Print the list.\n"
              << "9. Quit.\n\n";

    return PromptIntInRange(1, 9);
}

char* inputString()
{
    char* buf = static_cast<char*>(malloc(sizeof(char) * 100));

    cin.getline(buf, 100);
    size_t size = strlen(buf) + 1;
    char* input = static_cast<char*>(malloc(sizeof(char) * size));
    strcpy(input, buf);
    free(buf);
    return input;
}
