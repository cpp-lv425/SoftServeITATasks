#pragma once
/* Initializes list */
extern void StringListInit(char*** list);
/* Destroy list and set pointer to NULL. */
extern void StringListDestroy(char*** list);
/*new element*/
extern char ** new_element(char * str);

/* Inserts value at the end of the list. */
extern void StringListAdd(char** list, char * str);
/* Removes all occurrences of str in the list. */
extern void StringListRemove(char*** list, char * str);

/* Returns the number of items in the list. */
extern int StringListSize(char** list);
/* Returns the index position of the first occurrence of str in the list. */
extern int StringListIndexOf(char** list, char* str);

/* Removes duplicate entries from the list. */
extern void StringListRemoveDuplicates(char** list);
/* Replaces every occurrence of the before, in each of the string lists's strings, with after. */
extern void StringListReplaceInStrings(char** list, char* before, char* after);
/* Sorts the list of strings in ascending order */
extern void StringListSort(char** list);

extern void PrintList(char ** list);