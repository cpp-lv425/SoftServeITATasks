#include "stringlist.h"
#include <iostream>
int main()
{
    List list = initialization();
    push(list,"igor");
    push(list,"daniv");
    push(list,"danissimo");
    push(list,"zzz");
    push(list,"aaa");
    sort(list);
    print(list);

    std::cout<<"---------------------------------"<<std::endl;
    push(list,"igor");
    print(list);
    std::cout<<"---------------------------------"<<std::endl;
    std::cout<<"---------------------------------"<<std::endl;

    deleteDuplicates(list);
    print(list);

}
