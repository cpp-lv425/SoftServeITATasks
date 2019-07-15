#define _CRT_SECURE_NO_WARNINGS
#include"StringList.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include"node.h"

void setNext(char** ptr, char** adr)//set pointer to the next element
{
	ptr[2] = (char*)(adr);
}

char** getNext(char** ptr)// get pointer from the next element
{
	return (char**)(ptr[2]);
}

void setPrev(char** ptr, char** adr)//set pointer to the previous element
{
	ptr[0] = (char*)(adr);
}

char** getPrev(char** ptr)//get pointer from the previous element
{
	return (char**)(ptr[0]);
}

void StringListInit(char *** ptr)//List initialization
{
	*ptr = (char**)malloc(sizeof(NODE));// allocate memory for list
	if (ptr == NULL)
		return;
	/*set all pointers for first node NULL. This element will be the first element in the list*/
	setNext((*ptr), NULL);
	setPrev((*ptr), NULL);
	getData(*ptr) = NULL;
}

/*Emample of the list
   [0]⇄[1]⇄[2]⇄[3]->NULL
   ↳------------↑
   .first node->prev == last node.
   */
void StringListAdd(char** list, char* str)//queue realiation. 
{
	int len = strlen(str) + 1;//size for the string
	char **writeAdr;//pointer for new elem in the memory
	if (getData(list) == NULL) //if we wanna add first element
	{
		writeAdr = list;// just set pointer to the current root. because root already exists(we created it in the Init Function)
	}
	else
	{
		writeAdr = GetNewNode(list);// create new node
	}
	getData(writeAdr) = (char*)malloc(sizeof(char)*len);// allocate momory for the string
	if (getData(writeAdr) == NULL)
		return;
	strcpy(getData(writeAdr), str);//copy string to our pointer
}

char** GetNewNode(char** list)// return a pointer to the new node in the memory
{
	char **newNode = (char**)malloc(sizeof(NODE));// create node in the memory
	if (newNode == NULL)
		return NULL;
	setNext(newNode, NULL);// this node will be the last one, so we should set its next node NULL
	if (getPrev(list) == NULL) // we're trying to add second element. Here is some specific rules for creating the connections
	{
		//NULL<-[0]->NULL  [newnode]->NULL
		setPrev(list, newNode);
		/*[0]->NULL [newnode]->NULL
			↳--------↑*/
		setPrev(newNode, list);
		setNext(list, newNode);
		/*[0]⇄[newnode]->NULL
		   ↳---↑*/
		return newNode;
	}
	char** last = getPrev(list);// get last element
	/*[0]⇄[1]⇄...[last]->NULL [newnode]->NULL
	   ↳------------↑
	*/
	setPrev(newNode, last);
	setNext(last, newNode);
	/*[0]⇄[1]⇄...[last]⇄[newnode]->NULL
	↳------------↑
	*/
	setPrev(list, newNode);
	/*[0]⇄[1]⇄...[last]⇄[newnode]->NULL
	↳-----------------------↑
	*/
	return newNode;
}

//
void StringListDestroy(char*** list)
{
	if (getPrev(*list) == NULL)
		return;
	char **victim = getPrev(*list);// set pointer to the end of the list
	char **victimTmp;// temporary var which we will set in the position which we wanna delete
	while (*victim != **list && *victim != NULL)// while(pointer != NULL and pointer != first node). We're moving from the end
	{
		free(getData(victim));//delete string from the current node (delete node[1])
		victimTmp = victim;//set another pointer to the node which we will delete
		victim = getPrev(victim);// move to the previous node. because we'll delete current(run away)
		free(victimTmp);//free node
	}
	/*we havn't deleted all element. now we're staying in the first node(root)*/
	free(getData(victim));//delete string from the current node (delete node[1])
	free(*list);//free root
	*list = NULL;//set root = NULL
}

char* GetStringByPosition(char** list, int pos)// return the string by its position in the list
{
	char **cur = list;//to the root
	if (cur == NULL)
		return NULL;
	while (pos)//while(pos > 0)
	{
		cur = getNext(cur);//move
		if (cur == NULL) {//check if we passed the end
			return error_msg;//element by 'pos' position doesn't exist
		}
		--pos;
	}
	return getData(cur);//return string node[1]
}

void StringListPrint(char** list)// printfList
{
	if (list == NULL)// if we have destroyed list
	{
		printf("LIST IS DESTROYED!\n");
		return;
	}
	if (getPrev(list) == NULL && getNext(list) == NULL)// if we haven't added any elements
	{
		printf("LIST IS EMPTY!\n");
		return;
	}
	char **ptr = list;//pointer to the root
	while (ptr)
	{
		printf("%s\n", getData(ptr));//print data
		ptr = getNext(ptr);
	}
	printf("\n");
}

int StringListSize(char **list)// move to the next node and count the number of nodes
{
	int size = 0;
	char **ptr = list;
	while (ptr)
	{
		ptr = getNext(ptr);
		size++;
	}
	return size;
}

int StringListGetIndexOf(char** list, char* str)
{
	int index = 0;
	char **ptr = list;//pointer to the root
	while (ptr)
	{
		if (strcmp(getData(ptr), str) == 0)// if we found data if the list which is equal to str
		{
			return index;
		}
		ptr = getNext(ptr);// move to the next element
		index++;
	}
	return -1;// if index hasn't been found
}

void swap(char** str1, char** str2)//swap the strings (we don't move the nodes poiners. just reset pointers adresses)
{
	char * temp = getData(str1);
	getData(str1) = getData(str2);
	getData(str2) = temp;
}

void StringListSort(char** list)//Sort
{
	for (char **ptr1 = list; ptr1 != NULL; ptr1 = getNext(ptr1))
	{
		for (char **ptr2 = getNext(ptr1); ptr2 != NULL; ptr2 = getNext(ptr2))
		{
			if (strcmp(getData(ptr1), getData(ptr2)) > 0)
			{
				swap(ptr1, ptr2);
			}
		}
	}
}

char** DeleteNode(char** node, char **list)// Function delete node and return the previous node(in order to )
{
	if (node == NULL)
		return NULL;
	char **ptr = node;// ptr to root
	char **victim;// ptr to element which will be deleted
	if (ptr == getPrev(ptr))// only 1 element in the list(root). we don't actually delete the root. we just set NULL.
	{
		//[0]->NULL
		//↳-↑
		setPrev(ptr, NULL);
		free(getData(ptr));//free string(node[1])
		getData(ptr);//set data as NULL
		return ptr;//return this node
	}

	else if (ptr == list)//node is the first in the list
	{
		swap(ptr, getNext(ptr));//if element is first we swap its data with second element
		return DeleteNode(getNext(ptr), list);//and delete second element. We are doing this in order to not remove root
	}
	else if (getNext(ptr) == NULL)//node is the last in the list
	{
		setNext(getPrev(ptr), NULL);
		setPrev(list, getPrev(ptr));
	}
	else// node is inside the list
	{
		/*[0]⇄[1]⇄[2]⇄[3]->NULL
			↳------------↑
			For example we wanna delete [1] element. here our ptr point to [2]
		*/
		setNext(getPrev(ptr), getNext(ptr));

		/*
		   ↱--------↴
		  [0]<-[1]⇄[2]⇄[3]->NULL
		   ↳-----------↑
		   */
		setPrev(getNext(ptr), getPrev(ptr));
		/*
		↱----⇄---↴
		[0]<-[1]->[2]⇄[3]->NULL
		↳-----------↑
		*/
	}
	victim = ptr;//pointer to the element which we wanna delete ([1])
	ptr = getPrev(ptr);// go to the prev ([0])
	free(getData(victim));//delete data free([0])
	free(victim);//delete root
	//[0]⇄[2]⇄[3]->NULL
	// ↳-------↑
	return ptr;//return previos element of which has been deleted
}

void StringListRemove(char** list, char* str)// delete ever
{
	if (list == NULL)
		return;
	char **ptr = getPrev(list);// from the end. In ored to save root
	while (*ptr != *list && *ptr != NULL)//while inside list and !=NULL
	{
		if (strcmp(getData(ptr), str) == 0)//if data == str
		{
			ptr = DeleteNode(ptr, list);//delete node
		}
		else
		{
			ptr = getPrev(ptr);//go to the previous node
		}
	}
	//if we are here we passed the all list exept the first node
	if (strcmp(getData(ptr), str) == 0)//for first node
	{
		ptr = DeleteNode(ptr, list);
	}
}

void StringListRemoveDuplicates(char **list)
{
	/*StringListSort(list);//sort in order to have elements one next to each other
	char **ptr = getPrev(list);//last node
	while (*ptr != *list && *ptr != NULL)
	{
		while ((strcmp(getData(ptr), getData(getPrev(ptr))) == 0))//while next element is the same
		{
			ptr = DeleteNode(getPrev(ptr), list);//delete root
		}
		ptr = getPrev(ptr);// move pointer
	}*/
	if (!getNext(list))// if only one node (we don't have duplicates)
		return;
	char **ptr = getPrev(list);// pointer to the list end
	//we go from the end and compare if any node string ==  root sting
	while (*ptr != *list && *ptr != NULL)
	{
		if (!strcmp(getData(ptr), getData(list)))// if the strings are the same
		{
			ptr = DeleteNode(ptr, list);// delete this node
		}
		else
		{
			ptr = getPrev(ptr);// move ptr
		}
	}
	//call recursive funcion. next call will be from second node. Our root is already unique, so we can move from second node
	StringListRemoveDuplicates(getNext(list));
}

void StringListReplaceInStrings(char** list, char* before, char* after)
{
	char *add_str = (char*)malloc(sizeof(char)*strlen(after)+ 1);//create the string in the memory
	if (add_str == NULL)
		return;
	strcpy(add_str, after);// copy there "before" str
	char **ptr = list;// pointer to the start
	while ((ptr != NULL))
	{
		if (strcmp(getData(ptr), before) == 0)//check if node data == "before" str
		{
			getData(ptr) = add_str;// set data pointer to this string
		}
		ptr = getNext(ptr);//move pointer
	}
}