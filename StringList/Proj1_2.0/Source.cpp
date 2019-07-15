#include<stdio.h>
#include"StringList.h"

int main()
{
	char **list;
	StringListInit(&list);
	StringListPrint(list);

	printf("The List has been created!\n");
	StringListAdd(list, (char*)"Peter");
	StringListAdd(list, (char*)"John");
	StringListAdd(list, (char*)"Mark");
	StringListAdd(list, (char*)"Anna");
	StringListAdd(list, (char*)"Mark");
	StringListAdd(list, (char*)"Joseph");
	StringListAdd(list, (char*)"Peter");

	printf("List after adding a few elemenst:\n\n");
	StringListPrint(list);

	printf("List after removing \"Mark\":\n\n");
	StringListRemove(list, "Mark");
	StringListPrint(list);

	printf("List size = %d\n", StringListSize(list));

	printf("The first index of \"Anna\" = %d\n", StringListGetIndexOf(list, "Anna"));

	StringListRemoveDuplicates(list);
	printf("\nList after removing duplicates \n\n");
	StringListPrint(list);

	StringListReplaceInStrings(list, "Peter", "Also Peter, but cooler");
	printf("\nList after strings replacing\n\n");
	StringListPrint(list);

	printf("The List has been destroyed!\n");
	StringListDestroy(&list);
	StringListPrint(list);

	/*StringListInit(&list);
	StringListAdd(list, (char*)"Joseph");
	StringListAdd(list, (char*)"Peter");
	StringListPrint(list);*/
	getchar();
	return 0;
}