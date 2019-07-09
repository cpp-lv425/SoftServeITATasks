/*
	Set of the interface functions for using the dynamic list of c strings. Done
	on the SoftServe' demand. According to the convention the functions starting
	with underline, e.g. <_FuncName>, are hidden and as they don't relate to the
	interface should not be used. Exceptiona type - const char *.
	Author:						Valentyn Faychuk
	E-mail:						faitchouk.valentyn@gmail.com
	Created on:					06.07.2019
*/
#include <stdlib.h>
#include <string.h>

// Type for the string intself
typedef char * String;
// Types for the list size as well as string sizes
typedef int StringListSizeType;
typedef int StringSizeType;

const char * g_badAllocMessage = "Bad memory allocation";
const char * g_NULLPtrParameterMessage = "NULL pointer as a parameter";

// ******************************************************************* CONTROLLERS

// Initialize list
void StringListInit(char *** list);

// Destroy list and set pointer to the nullptr
void StringListDestroy(char *** list);

// ******************************************************************* /CONTROLLERS

// ************************************************************************ GETTERS

// Return the number of items in the list
int StringListSize(char ** list);

// Give the allocation size of the list
StringListSizeType _StringListAllocatedSize(char ** list);

// Return the index position of the first occurance of the str in the list
int StringListIndexOf(char ** list, const char * str);

// Return the index position of the first occurance of the str in the list
// Start its search from given index if the start parameter is present
StringListSizeType _StringListIndexOf(char ** list, const char *, StringListSizeType start);

// ************************************************************************ /GETTERS

// ************************************************************************ MUTATORS

// Insert the value at the end
void StringListAdd(char *** list, const char * str);

// Remove all occurrences of the str from the list
void StringListRemove(char ** list, const char * str);

// Remove duplicate entries from the list
void StringListRemoveDuplicates(char ** list);

// Replace every occurrence of the before with the after
void StringListReplaceInStrings(char ** list, const char * before, const char * after);

// Sort the strings in ascending order
void StringListSort(char ** list);

// Sort the strings in ascending order via the quicksort algorithm using partitions
void _StringListSort(char ** list, StringListSizeType begin, StringListSizeType pivot);

// Shrink allocated memory to fit all the elements in the list
void StringListShrinkToFit(char *** list);

// ************************************************************************ /MUTATORS

void StringListInit(char *** list)
{
	// Hold the damage
	if (list == NULL) return;
	// Allocate place for the listSize, allocSize metadata fields + stringList itself
	// and set the listSize = 0 as well as allocSize = 1
	// General structure:		[listSize] [allocSize] [firstElement] ... [lastElement]
	// Created instance:		[ 0 ] [ 1 ] [NULL]
	StringListSizeType * stringListWithSizes =
		static_cast<StringListSizeType *>(malloc(sizeof(StringListSizeType) * 2 + sizeof(char **)));
	if (stringListWithSizes == NULL) throw g_badAllocMessage;
	*stringListWithSizes = 0;
	*(++stringListWithSizes) = 1;
	// Create correct pointer to the stringList and push it's address into the variable list
	// [list] -> [char **] -> [firstElement] [secondElement] ... [lastElement]
	*list = reinterpret_cast<char **>(++stringListWithSizes);
	(*list)[0] = NULL;
}

void StringListDestroy(char *** list)
{
	// Hold the damage
	if (list == NULL || *list == NULL) return;
	// Free all the strings inside the stringList
	StringListSizeType stringListSize = StringListSize(*list);
	for (StringListSizeType stringIndex{ 0 }; stringIndex < stringListSize; ++stringIndex) {
		free((*list)[stringIndex]);
	}
	// Free the stringList including the metadata fields
	StringListSizeType * stringListWithSizes = reinterpret_cast<StringListSizeType *>(*list) - 2;
	free(stringListWithSizes);
	// Return the NULL pointer
	*list = NULL;
}

inline int StringListSize(char ** list)
{
	// Distribute the damage to prevent hazard - throw an exception if the list is NULL
	if (list == NULL) throw g_NULLPtrParameterMessage;
	/*
	   Get the stringList size from the metadata
	   [ listSize ] [ allocSize ] [ firstElement ] ... [ lastElement ]
			^
	*/
	StringListSizeType size;
	size = *(reinterpret_cast<StringListSizeType *>(list) - 2);
	return size;
}

inline StringListSizeType _StringListAllocatedSize(char ** list)
{
	/*
	   Get the stringList allocation size from the metadata
	   [ listSize ] [ allocSize ] [ firstElement ] ... [ lastElement ]
						  ^
	*/
	StringListSizeType allocSize;
	allocSize = *(reinterpret_cast<StringListSizeType *>(list) - 1);
	return allocSize;
}

inline int StringListIndexOf(char ** list, const char * str)
{
	// Distribute the damage to prevent hazard - throw an exception if the list is NULL
	if (list == NULL) throw g_NULLPtrParameterMessage;
	// Compare strings one by one until the similarity found and return the index of similarity
	// In the contrast case return the size of the list which is the index "right after the end"
	StringListSizeType stringListSize = StringListSize(list);
	// Hold the damage
	if (str == NULL) return stringListSize;
	for (StringListSizeType stringIndex{ 0 }; stringIndex < stringListSize; ++stringIndex)
		if (!memcmp(str, list[stringIndex], strlen(str) + 1)) return stringIndex;
	return stringListSize;
}

inline StringListSizeType _StringListIndexOf(char ** list, const char * str, StringListSizeType start)
{
	// Compare strings one by one until the similarity found and return the index of similarity
	// In the contrast case return the size of the list which is the index "right after the end"
	StringListSizeType stringListSize = StringListSize(list);
	for (StringListSizeType stringIndex{ start }; stringIndex < stringListSize; ++stringIndex)
		if (!memcmp(str, list[stringIndex], strlen(str) + 1)) return stringIndex;
	return stringListSize;
}

void StringListAdd(char *** list, const char * str)
{
	//Hold the damage
	if (list == NULL || *list == NULL || str == NULL) return;
	// Get the total quantity of placed strings and the total quantity of allocated "ready to use" strings
	StringListSizeType listSize = StringListSize(*list);
	StringListSizeType allocSize = _StringListAllocatedSize(*list);
	// Allocate enough memory and place new string instance to the list if needed
	if (++listSize > allocSize) {
		allocSize *= 2;
		StringListSizeType * stringListWithSizes = reinterpret_cast<StringListSizeType *>(*list) - 2;
		stringListWithSizes = static_cast<StringListSizeType *>(
			realloc(stringListWithSizes, sizeof(StringListSizeType) * 2 + sizeof(char **)*allocSize));
		if(stringListWithSizes == NULL) throw g_badAllocMessage;
		*list = reinterpret_cast<char **>(stringListWithSizes + 2);
	}
	// Add the string instance to the list
	StringSizeType strSize = strlen(str) + 1;
	(*list)[listSize - 1] = static_cast<String>(malloc(sizeof(char) * strSize));
	if ((*list)[listSize - 1] == NULL) throw g_badAllocMessage;
	memcpy((*list)[listSize - 1], str, strSize);
	// Change the metadata fields
	StringListSizeType * stringListWithSizes = reinterpret_cast<StringListSizeType *>(*list) - 2;
	*stringListWithSizes = listSize;
	*(++stringListWithSizes) = allocSize;
}

void StringListRemoveDuplicates(char ** list)
{
	// Hold the damage
	if (list == NULL) return;
	/*
	   Check the duplicates presence for each string in the list
	   If the duplicate for a certain string is present remove the first occurance of this string
	   Then try to find another duplicate and repeat actions until the duplicates will be removed
	*/
	StringListSizeType stringListSize = StringListSize(list);
	bool hasPrevDuplicatesBeenFound = false;
	for (StringListSizeType stringIndex{ 0 }; stringIndex < stringListSize; ++stringIndex) {
		if (hasPrevDuplicatesBeenFound) --stringIndex;
		hasPrevDuplicatesBeenFound = false;
		// Find the duplicate between the first instance of str and the end of list
		String str = list[stringIndex];
		StringListSizeType duplicateIndex = stringIndex + 1;
		duplicateIndex = _StringListIndexOf(list, str, duplicateIndex);
		// If the duplicate has been found remove it and acknowledge further iterations
		while (duplicateIndex < stringListSize)
		{
			hasPrevDuplicatesBeenFound = true;
			StringListRemove(list, str);
			stringListSize = StringListSize(list);
			duplicateIndex = _StringListIndexOf(list, str, duplicateIndex);
		}
	}
}

void StringListRemove(char ** list, const char * str)
{
	// Hold the damage
	if (list == NULL || str == NULL) return;
	// Get the size of the list and the index of the string to be removed
	StringListSizeType stringIndex = StringListIndexOf(list, str);
	StringListSizeType stringListSize = StringListSize(list);
	// Return if the str has not been found
	if (stringIndex == stringListSize) return;
	/*
	   Iterate through least strings and rewrite each string with the next
	   For example if i is to be removed:
	   [ 1 ] [ 2 ]  ...  [i-1] [ i ] [i+1] [i+2]  ...  [n-1] [ n ]
		   \     \           \        /     /     /     /     /
		  [ 1 ] [ 2 ]  ...  [i-1] [i+1] [i+2] [i+3]  ...  [ n ] [NULL]
	*/
	free(list[stringIndex]);
	while (++stringIndex < stringListSize)
		list[stringIndex - 1] = list[stringIndex];
	list[stringIndex] = NULL;
	// Change the size field in metadata
	StringListSizeType * stringListWithSizes = reinterpret_cast<StringListSizeType *>(list) - 2;
	*stringListWithSizes = stringListSize - 1;
}

void StringListReplaceInStrings(char ** list, const char * before, const char * after)
{
	// Hold the damage
	if (list == NULL || before == NULL || after == NULL) return;
	// Iterate through similar strings to before and remplace them with after
	StringListSizeType stringIndex = StringListIndexOf(list, before);
	StringListSizeType stringListSize = StringListSize(list);
	while (stringIndex < stringListSize) {
		StringSizeType afterSize = strlen(after) + 1;
		String temporaryString =
			static_cast<String>(realloc(list[stringIndex], sizeof(char) * afterSize));
		if (temporaryString == NULL) throw g_badAllocMessage;
		list[stringIndex] = temporaryString;
		memcpy(list[stringIndex], after, afterSize);
		stringIndex = _StringListIndexOf(list, before, stringIndex);
	}
}

void StringListSort(char ** list)
{
	// Hold the damage
	if (list == NULL) return;
	/*
	   Standard quick sort principle, though the pivot is always taken at
	   the end of the list
	*/
	_StringListSort(list, 0, StringListSize(list) - 1);
}

void _StringListSort(char ** list, StringListSizeType begin, StringListSizeType pivot)
{
	// Remeber the index of end and bufferize the pivot string
	StringListSizeType end = pivot;
	String pivotString = list[pivot];
	// Get size of the pivot c string for memcmp function
	StringSizeType pivotStringSize = strlen(pivotString) + 1;
	/*
	   Iterate over strings before the pivot string and
	   if the taken string is greater than the pivot string
	   perform further permutation:
	   [ 0 ] ... [ i ] ... [ n ] [piv]    =>    [ 0 ] ... [ n ] ... [piv] [ i ]
	*/
	bool hasPrevPermutationBeenDone = false;
	for (StringListSizeType stringIndex{ begin }; stringIndex <= pivot; ++stringIndex) {
		if (hasPrevPermutationBeenDone) --stringIndex;
		hasPrevPermutationBeenDone = false;
		if (memcmp(list[stringIndex], list[pivot], pivotStringSize) > 0) {
			hasPrevPermutationBeenDone = true;
			String temporaryString = list[pivot];
			list[pivot] = list[stringIndex];
			list[stringIndex] = list[--pivot];
			list[pivot] = temporaryString;
		}
	}
	/*
	   Perform actions recursively:
	   [ 0 ] ... [ i ] [piv] [ j ] ... [ n ]
		\___________/         \___________/
	   [ 0 ] ... [piv]       [ 0 ] ... [piv]
	*/
	if (pivot - 1 > begin)
		_StringListSort(list, begin, pivot - 1);
	if (pivot + 1 < end)
		_StringListSort(list, pivot + 1, end);
}

void StringListShrinkToFit(char *** list)
{
	//Hold the damage
	if (list == NULL || *list == NULL) return;
	StringListSizeType allocSize = StringListSize(*list);
	// Allocate enough memory to hold all list elements and change the metadata allocSize field
	StringListSizeType * stringListWithSizes = reinterpret_cast<StringListSizeType *>(*list) - 2;
	realloc(stringListWithSizes, sizeof(StringListSizeType)*2 + sizeof(char **)*allocSize);
	*(++stringListWithSizes) = allocSize;
}