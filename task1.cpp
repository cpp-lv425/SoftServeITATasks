#include <string.h>		
#include <stdlib.h>
#include <stdio.h>

void StringListPlot(char** list);		/* Plotting list to stdout */

void StringListInit(char*** list);		
void StringListDestroy(char*** list);		/* Destroy list and set pointer to NULL. */
void StringListAdd(char** list, char * str);	/* Inserts value at the end of the list. */
void StringListRemove(char** list, char* str);	/* Removes all occurrences of str in the list. */				
int StringListSize(char** list);		/* Returns the number of items in the list.*/	
int StringListIndexOf(char** list, char* str);	/* Returns the index position of the first occurrence of str in the list. */
void StringListRemoveDuplicates(char** list);	/* Removes duplicate entries from the list.*/		

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
	printf ("-->Init<--");
	if (!list)
		return;
	if ( !(* list = (char**)malloc (2 * sizeof(char**))) )
		return;
	printf ("\tallocated\t%p\n", * list);
	** list = 0;					//	a
	*(++(* list)) = 0;				//	b
}
void StringListDestroy(char*** list) {/* Destroy list and set pointer to NULL. */
	printf ("->destroy<-\t");
	if (!list)
		return;
	--(*list);
	while (**list != 0) {
		char ** nextItem = (char **)**list;
		++nextItem;
		nextItem = (char **)* nextItem;
		printf ("\n\t\titem:\t\t%p", **list);
		free(**list);
		** list = (char *)nextItem;
	}
	printf ("\n\t\thead:\t\t%p\n", *list);
	free(*list);
	*list = 0;
}

void StringListAdd(char** list, char * str) {/* Inserts value at the end of the list. */
	printf ("-->adding<--\t%p\t%s", str, str);
	if (!list)
		return;
	char ** item;
	if ( !(item = (char**)malloc (2 * sizeof(char**))) )
		return;
	* item = str;					//	a
	printf ("\t\tallocated\t%p\n", item);

	if (*list == 0) {				//	first addition
		* list = (char *)item;			//	b
		--list;
		* list = (char *)item;			//	c
		*(++item) = 0; 				//	d
	} else {					//	next additions
		char** tmp = (char**)*list;		//	b1
		++tmp;					//	b1
		* tmp = (char *)item;			//	b1
		* list = (char *)item;			//	c1
		*(++item) = 0;				//	d1
	}
}

void StringListPlot(char** list) { /* Plotting list to stdout */
	printf ("->plot<-\n");
	if (!list)
		return;
	--list;
	while (*list != 0) {
		list = (char**)*list;
		printf ("*item:%s", *list);
		++list;
	}
}
void StringListRemove(char** list, char* str) {/* Removes all occurrences of str in the list. */			
	printf ("->Remove<-\t%s", str);
	if (!list)
		return;
	--list;
	while (*list != 0) {
		char ** nextItem = (char **)*list;				
		if (!strcmp(*nextItem, str)){			
			++nextItem;
			nextItem = (char **)* nextItem;		
			printf ("\t\tfree\t\t%p\n", *list);
			free(*list);			//	e
			*list = (char *)nextItem;	//	g
		} else {					
			list = ++nextItem;
		}
	}
}

void StringListRemoveDuplicates(char** list) {/* Removes duplicate entries from the list. */
	if (!list)
		return;	
	--list;
	while (*list != 0) {
		char ** nextItem = (char **)*list;
		char * str = *nextItem;	
		printf ("current str:\t%s", str);
		StringListRemove(++++nextItem, str);
		--nextItem;
		list = (char **)nextItem;	
	}
}

int StringListSize(char** list) {/* Returns the number of items in the list. */
	if (!list)
		return -1;
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
	if (!list)
		return -1;
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


