#include "pch.h"
#include "string_list.h"
#include <string.h>
#include <iostream>

using namespace std;

enum { pv = 0, nt = 1, name = 2 };

//make new element
char ** new_element(char* txt) {
	char ** element = (char**)malloc(12);
	element[pv]= NULL;
	element[nt] = NULL;
	char * str = (char *)malloc(strlen(txt) * sizeof(char) + sizeof(char));
	str = strcpy(str, txt);
	element[name] = str;
	return element;
}

/* Initializes list */
void StringListInit(char*** list) {
	*list = (char**)malloc(12);
	(*list)[pv] = NULL;
	(*list)[nt] = NULL;
	(*list)[name] = NULL;
}
//adding new element to list
void StringListAdd(char** list, char * str) {
	
	char ** current = list;
	if (list[name] == NULL) {	//adding first element
		char * tmpStr = (char *)malloc(strlen(str) * sizeof(char) + sizeof(char));
		tmpStr = strcpy(tmpStr, str);
		current[name] = tmpStr;
	}
	else {	//adding element to the end of the list 
		char ** element=new_element(str);
		while (current[nt] != NULL) {
			current = (char **)current[nt];
		}
		element[pv] = (char *)current;
		current[nt] = (char *)element;
		current = (char**)current[nt];
	}
}
//count element in list
int StringListSize(char** list) {
	char **current=(char**)malloc(12);
	current = list;
	int size = 0;
	while (current!=NULL) {
		size++;
		current = (char**)current[nt];
	}
	return size;
}

//find index of element with needed element[name]
int StringListIndexOf(char** list, char* str) {
	char ** current = list;
	int index = 1;
	while (true) {
		if (strcmp(current[name], str) == 0) {
			return index;
		}
		current = (char**)current[nt];
		index++;
		if (current == NULL) return -1; //return -1 if it`s not fit element
	}
}
//replace element[name] 
void StringListReplaceInStrings(char** list, char* before, char* after) {
	char ** current = list;
	while (strcmp(current[name],before)!=0) {
		current = (char **)current[nt];
		if (current == NULL) return; //do nothing if its not element which fit to replacing 
	}
	current[name] = after;
}
//remove all element with fited element[name] 
void StringListRemove(char*** list, char * str) {
	char ** current = *list;
	char ** prev=NULL;
	char ** next = NULL;
	for (current; current!=NULL; current = (char **)current[nt]) {
		if (strcmp(current[name], str) == 0) {  //compare element[name] and taken string 
			if (prev == NULL) {		//check if first element must be deleted
				prev = current;
				current = (char **)current[nt];
				free(prev);
				*list = current;
				*list[pv] = NULL;
			}
			else {
				next = (char**)current[nt];
				if (next != NULL) {	//check if it`s not the last element must be deleted
				prev[nt] = current[nt];
				next[pv] = current[pv];
				free(current);
				current = next;
				}
				else {	//delete the last element in the list
					prev[nt] = NULL;
					free(current);
					current = NULL;
					break;
				}
				
			}
		}
		prev = current;
	}
}
//deleting fisrt founded element with suitable [name] 
void StringListRemoveFirst(char *** list,char * str) {
	char ** current = *list;
	char ** prev = NULL;
	char ** next = NULL;
	while (strcmp(current[name], str) != 0) { //finding element with suitable [name]
		prev = current;
		current = (char **)current[nt];
	}
	//deleting founded element
			if (prev == NULL) { //check if it is first
				prev = current;
				current = (char **)current[nt];
				free(prev[name]);
				free(prev);
				*list = current;
				*list[pv] = NULL;
			}
			else {
				prev[nt] = current[nt];
				next = (char**)current[nt];
				next[pv] = current[pv];
				free(current[name]);
				free(current);
				current = next;
			}
		
}
//remove elements which have duplicates in list
void StringListRemoveDuplicates(char** list) {
	char ** current;	
	char ** currentN;
	char ** prev;
	char ** next;
	for (current = list; (char**)current[nt] != NULL;current=(char **)current[nt]) {	//iterate through all string instances
		char * str = current[name];
		prev = current;
		// iterate while comparing each current string instance with other instances between current and end
		for (currentN = (char**)current[nt];currentN!= NULL; currentN = (char **)currentN[nt]) {
			if (strcmp(currentN[name], str) == 0) { //check if two element are duplicates and delete of it
				current = (char **)current[nt];
				StringListRemoveFirst(&list, str); //remove one of duplicates
				
			}
			prev = currentN;
			
		}
	}
}

//bubble sort of the list 
void StringListSort(char** list) {
	int size = StringListSize(list);
	for (int i = 0; i < size; i++) {	
		for (char **current = list; (char**)current[nt]!= NULL; current = (char**)current[nt]) {
			char ** next = (char **)current[nt];
			if (strcmp(current[name], next[name]) > 0) {
				char * tmp = next[name];
				next[name] = current[name];
				current[name] = tmp;
			}
			}
		}
	
}
//printing list to the console
void PrintList(char **list) {
	char ** current;
	current = list;
	cout << "start of list" << endl;
	while (current!= NULL) {
		cout << current[name] << endl;
		current = (char **)current[nt];
	}
	cout << "end of list" << endl;
}
//destroy list 
void StringListDestroy(char*** list) {
	char ** current = *list;
	if (*list == NULL) { cout << "empty"; }
	else {
		while ((char**)current[nt] != NULL) {
			current = (char **)current[nt];
		}
		while (current != NULL) {
			char ** tmp = current;
			// Hold the damage
			if (current[name] != NULL)
				free(current[name]);
 			current = (char **)current[pv];
			
			if (tmp != NULL)
				free(tmp);
		}
		
		*list = NULL;
		cout << "empty" << endl;
	}
}