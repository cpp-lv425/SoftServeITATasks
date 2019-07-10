#include <iostream>
#include <anytype.h>

using std::cout;
using std::cerr;

int main()
{
    AnyType any = '*';
    AnyType other(std::move(any));
    AnyType oneMore;
    oneMore = other;
    other = 34.6f;
    oneMore.swap(other);    

    try
    {
        cout << oneMore.toFloat() << '\n'
             << "Contained value type: " << oneMore.typeName()
             << '\n';        
    }
    catch (const AnyType::BadAnyCast& e)
    {
        cerr << e.what() << '\n';
    }

    return 0;
}
