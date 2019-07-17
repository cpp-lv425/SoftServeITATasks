#include <iostream>

#include "anytype.h"
#include "badcast.h"

int main()
{
    AnyType a = 10;
    a = false;
    AnyType anyType = 1;
    anyType = true;
    anyType = 1.7;
    std::cout<<anyType.typeName();
    AnyType b(a);
    AnyType temp(18.2345);
    AnyType c(std::move(temp));
    AnyType d = a;
    AnyType e = AnyType(true);

    std::cout<<a.typeName()<<std::endl;
    std::cout<<a.toInt()<<std::endl;

    std::cout<<b.typeName()<<std::endl;
    std::cout<<b.toInt()<<std::endl;

    std::cout<<c.typeName()<<std::endl;
    std::cout<<c.toDouble()<<std::endl;

    std::cout<<d.typeName()<<std::endl;
    std::cout<<d.toInt()<<std::endl;

    std::cout<<e.typeName()<<std::endl;
    std::cout<<e.toBool()<<std::endl;

}
