#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <type_traits>
#include <iostream>

enum class TypesEnum
{
    Undefined,
    Bool,
    Char,
    Int,
    Double,
};

struct TypesStruct
{
    bool boolValue = 0;
    char charValue = 0;
    int intValue = 0;
    double doubleValue = 0;
    void setNullValues()
    {
        boolValue = 0;
        charValue = 0;
        intValue = 0;
        doubleValue = 0;
    }
    TypesStruct&operator=(const TypesStruct& other) = default;
    TypesStruct(TypesStruct &&other) = default;
    TypesStruct() = default;
};

template <typename Type>
TypesEnum detectType() noexcept
{
    if (std::is_same<Type, bool>::value)
    {
        return TypesEnum::Bool;
    }
    else if (std::is_same<Type, char>::value)
    {
        return TypesEnum::Char;
    }
    else if (std::is_same<Type, int>::value)
    {
        return TypesEnum::Int;
    }
    else if (std::is_same<Type, double>::value)
    {
        return TypesEnum::Double;
    }
    else
    {
        return TypesEnum::Undefined;
    }
}
template<typename Type>
std::string getTypeName()
{
    TypesEnum type = detectType<Type>();
    switch (type)
    {
    case TypesEnum::Int:
        return "int";

    case TypesEnum::Bool:
        return "bool";

    case TypesEnum::Char:
        return "char";

    case TypesEnum::Double:
        return "double";

    case TypesEnum::Undefined:
        return "undefinded";
    }
    return "undefinded";
}

#endif // TYPES_H
