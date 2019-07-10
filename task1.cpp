#include <string.h>		
#include <stdlib.h>
#include <stdio.h>

void StringListPlot(char** list);		

void StringListInit(char*** list);		
void StringListDestroy(char*** list);		
void StringListAdd(char** list, char * str);	
void StringListRemove(char** list, char* str);/* Removes all occurrences of str in the list. */				
int StringListSize(char** list);/* Returns the number of items in the list. */						
int StringListIndexOf(char** list, char* str);/* Returns the index position of the first occurrence of str in the list. */
void StringListRemoveDuplicates(char** list);/* Removes duplicate entries from the list.*/		

int main (int argc, char ** argv){
	printf ("-->START<--\n");

	char *str1 = (char*)malloc(sizeof(char) *5); 
	str1[0] = 't';str1[1] = 'x';str1[2] = 't';str1[3] = '1';str1[4] = '\n';
	char *str2 = (char*)malloc(sizeof(char) *5); 
	str2[0] = 't';str2[1] = 'x';str2[2] = 't';str2[3] = '2';str2[4] = '\n';
	char *str3 = (char*)malloc(sizeof(char) *5); 
	str3[0] = 't';str3[1] = 'x';str3[2] = 't';str3[3] = '3';str3[4] = '\n';
	char *str11 = (char*)malloc(sizeof(char) *5); 
	str11[0] = 't';str11[1] = 'x';str11[2] = 't';str11[3] = '1';str11[4] = '\n';
	char *str12 = (char*)malloc(sizeof(char) *5); 
	str12[0] = 't';str12[1] = 'x';str12[2] = 't';str12[3] = '2';str12[4] = '\n';
	char str[] = "Any text";

	char** list1;	
	list1 = 0;
	StringListInit(&list1);

	StringListAdd(list1, str1);

	StringListAdd(list1, str2);
	StringListAdd(list1, str2);
	StringListAdd(list1, str3);
	StringListAdd(list1, str3);
	StringListAdd(list1, str1);

	StringListPlot(list1);

	printf ("SIZE:%i\n", StringListSize(list1));

	printf ("FIRST INDEX IS %i OF %s", StringListIndexOf(list1, str11), str11);
	printf ("FIRST INDEX IS %i OF %s", StringListIndexOf(list1, str12), str12);
	printf ("FIRST INDEX IS %i OF %s", StringListIndexOf(list1, str3), str3);

	/*StringListRemove2(list1, str2);
	StringListPlot2(list1);

	StringListRemove2(list1, str11);
	StringListPlot2(list1);

	StringListRemove2(list1, str3);
	StringListPlot2(list1);*/
	
	//StringListRemoveDuplicates(list1);
	//StringListPlot2(list1);
	
	StringListRemoveDuplicates(list1);
	StringListPlot(list1);
	
	StringListRemove(list1, str2);
	StringListPlot(list1);

	StringListRemove(list1, str11);
	StringListPlot(list1);

	StringListRemove(list1, str3);
	StringListPlot(list1);
	
	StringListRemove(list1, str2);
	StringListPlot(list1);

	StringListRemove(list1, str11);
	StringListPlot(list1);

	StringListRemove(list1, str3);
	StringListPlot(list1);

	StringListRemoveDuplicates(list1);
	StringListPlot(list1);

	printf ("FIRST INDEX IS %i OF %s", StringListIndexOf(list1, str11), str11);
	printf ("FIRST INDEX IS %i OF %s", StringListIndexOf(list1, str12), str12);
	printf ("FIRST INDEX IS %i OF %s", StringListIndexOf(list1, str3), str3);

	printf ("SIZE:%i\n", StringListSize(list1));
	StringListDestroy(&list1);
	printf ("list1:%p\n", list1);

	free(str1);	free(str2);	free(str3);	free(str11);	free(str12);
	return 0;
}

void StringListInit(char*** list){
	* list = (char**)malloc (2 * sizeof(char**));
	** list = 0;					
	*(++(* list)) = 0;				
}
void StringListDestroy(char*** list) {/* Destroy list and set pointer to NULL. */
	--(*list);
	while (**list != 0) {
		char ** nextItem = (char **)**list;
		++nextItem;
		nextItem = (char **)* nextItem;
		free(**list);
		** list = (char *)nextItem;
	}
	free(*list);
	*list = 0;
}

void StringListAdd(char** list, char * str) {/* Inserts value at the end of the list. */
	char ** item;
	item = (char**)malloc (2 * sizeof(char**));
	* item = str;					

	if (*list == 0) {				//	first adding
		* list = (char *)item;			
		--list;
		* list = (char *)item;			
		*(++item) = 0; 				
	} else {					//	next addings
		char** last = (char**)*list;		
		++last;					
		* last = (char *)item;			
		* list = (char *)item;			
		*(++item) = 0;				
	}
}

void StringListPlot(char** list) {
	--list;
	while (*list != 0) {
		list = (char**)*list;
		++list;
	}
}
void StringListRemove(char** list, char* str) {/* Removes all occurrences of str in the list. */			
	--list;
	while (*list != 0) {
		char ** nextItem = (char **)*list;				
		if (!strcmp(*nextItem, str)){			
			++nextItem;
			nextItem = (char **)* nextItem;		
			free(*list);			
			*list = (char *)nextItem;	
		} else {					
			list = ++nextItem;
		}
	}
}

void StringListRemoveDuplicates(char** list) {/* Removes duplicate entries from the list. */	
	--list;
	while (*list != 0) {
		char ** nextItem = (char **)*list;
		char * str = *nextItem;	
		StringListRemove(++++nextItem, str);
		--nextItem;
		list = (char **)nextItem;	
	}
}

int StringListSize(char** list) {/* Returns the number of items in the list. */
	--list;
	int size = 0;
	while (*list != 0) {
		list = (char**)*list;
		++size;
		++list;
	}
	return size;
}

int StringListIndexOf(char** list, char* str) {/* Returns the index position of the first occurrence of str in the list. */
	--list;
	int index = -1;
	while (*list != 0) {
		list = (char**)*list;
		++index;
		if(!strcmp(*list, str)) {
			return index;
		}
		++list;
	}
	return -1;
}


