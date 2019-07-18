#ifndef STRING_LIST
#define STRING_LIST

using String = char*;

/* Initializes list */
void StringListInit(char*** list);

/* Returns pointer to the next node */
char** NextNode(char** current);

/* Tries to allocate memory for separate node
 * & checks if memory was allocated */
char** NodeMemoryAllocation();

/* Checks the list for emptiness */
void IsEmpty(char* ptr);

/* Checks passed arguments for null */
void UntenableArgument(char* str);

/* Deletes node pointed by del */
void DeleteNode(char** del);

/* Destroy list and set pointer to NULL. */
void StringListDestroy(char*** list);

/* Inserts value at the end of the list. */
void StringListAdd(char** list, String str);

/* Removes all occurrences of str in the list. */
void StringListRemove(char** list, String str);

/* Returns the number of items in the list. */
int StringListSize(char** list);

/* Returns the index position of the first 
occurrence of str in the list. */
int StringListIndexOf(char** list, char* str);

/* Removes duplicate entries from the list. */
void StringListRemoveDuplicates(char** list);

/* Replaces substring before with substring after in str */
char* ReplaceSubstringInString(char* str, char* before, char* after);

/* Replaces every occurrence of the before, 
in each of the string lists's strings, with after. */
void StringListReplaceInStrings(char** list, 
	char* before, char* after);

/* Sorts the list of strings in ascending order */
void StringListSort(char**& list);

#endif // !STRING_LIST
