#ifndef ANYTYPE_H
#define ANYTYPE_H

#include <string>
#include <typeinfo>
#include <cassert>

#include"typesinfo.h"

class AnyType final
{
public:
    AnyType() = delete;
    AnyType(const AnyType &other);
    AnyType(AnyType&& other);

    AnyType &operator=(const AnyType& other) = default;

    AnyType &operator=(AnyType&& other);

    template<typename Type>
    AnyType(const Type &value);

    ~AnyType() = default;

    friend void swap(AnyType& left, AnyType& right);

    bool toBool();
    char toChar();
    int toInt();
    double toDouble();

    std::string typeName() const;

    TypesEnum type() const;

private:
    TypesUnion mData;
    TypesEnum mType = TypesEnum::Undefined;
    std::string mTypeName{""};

    template<typename Type>
    void write(const Type &data);
};

template<typename Type>
void AnyType::write(const Type &data)
{
    mData.setNullValues();
    switch (mType)
    {
    case TypesEnum::Bool:
        mData.boolValue = data;
        break;
    case TypesEnum::Char:
        mData.charValue = data;
        break;
    case TypesEnum::Int:
        mData.intValue = data;
        break;
    case TypesEnum::Double:
        mData.doubleValue = data;
        break;
    case TypesEnum::Undefined:
        throw std::bad_cast();
    }
}

template<typename Type>
AnyType::AnyType(const Type &value)
{
    static_assert(!std::is_null_pointer<Type>::value, "Nullptr");
    static_assert(!std::is_void<Type>::value, "Void");
    assert(!(detectType<Type>() == TypesEnum::Undefined));

    mType = detectType<Type>();
    mTypeName = getTypeName<Type>();
    write<Type>(value);
}

#endif // ANYTYPE_H
