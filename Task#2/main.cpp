#include <iostream>
#include "AnyType.h"

int main() {
    AnyType var1(10);
    AnyType var2(-1.9);
    var2 = false;
    var1.swap(var2);
    std::cout << var1.toBool() << " " << var2.toDouble();
    Type type = var1.getType();
    std::cout << type;
}