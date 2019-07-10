#pragma once
void setNext(char** ptr, char** adr);

char** getNext(char** ptr);

void setPrev(char** ptr, char** adr);

char** getPrev(char** ptr);

void StringListInit(char *** ptr);

char** GetNewNode(char** list);

void StringListAdd(char** list, char* str);

void StringListDestroy(char*** list);

char* GetStringByPosition(char** list, int pos);

void StringListPrint(char** list);

int StringListSize(char **list);

int StringListGetIndexOf(char** list, char* str);

void swap(char** str1, char** str2);

void StringListSort(char** list);

char** DeleteNode(char** node, char **list);

void StringListRemove(char** list, char* str);

void StringListRemoveDuplicates(char **list);

void StringListReplaceInStrings(char** list, char* before, char* after);
