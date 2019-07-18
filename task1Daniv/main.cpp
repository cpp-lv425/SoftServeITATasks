#include <iostream>

#include <stdio.h>
#include "stringlist.h"

void print(const List &list)
{
    if(list == nullptr)
    {
        return;
    }
    auto temp = list;
    for(int i = 0; i < size(list); ++i)
    {
        printf("%s\n",temp[0]);
        temp = reinterpret_cast<List>(temp[1]);

    }
}
int main()
{
    List list = initialization();
    push(list,"igor");
    push(list,"daniv");
    push(list,"danissimo");
    push(list,"zzz");
    push(list,"aaa");
    push(list,"igor");

    print(list);

    replaceStrings(list,"igor","bbb");
    print(list);

}
