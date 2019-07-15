#include "pch.h"
#include "string_list.h"
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	char t[] = "nastia";
	char s[] = "marko";
	char p[] = "petro";
	char r[] = "rostik";
	char v[] = "valik";
	char z[] = "valik";
	char ** list;
	StringListInit(&list);
	StringListAdd(list, t);
	StringListAdd(list, s);
	StringListAdd(list, p);
	StringListAdd(list, r);
	StringListAdd(list, r);
	StringListAdd(list, v);
	StringListAdd(list, s);
	cout << StringListSize(list) << endl;
	PrintList(list);
	cout << StringListIndexOf(list, p) << endl;
	StringListReplaceInStrings(list, r, z);
	PrintList(list);
	StringListRemove(&list, z);
	PrintList(list);
	StringListRemoveDuplicates(list);
	PrintList(list);
	StringListSort(list);
	PrintList(list);
	StringListDestroy(&list);

}