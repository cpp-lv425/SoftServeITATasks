#include "anytype.h"
#include "badcast.h"

AnyType::AnyType(const AnyType &other)
{
    mType = other.type();
    mTypeName = other.typeName();
    mData = other.mData;
}

AnyType::AnyType(AnyType &&other)
{
    mData = std::move(other.mData);
    mType = std::move(other.mType);
    mTypeName = std::move(other.mTypeName);
}

AnyType &AnyType::operator=(AnyType &&other)
{
    mData = std::move(other.mData);
    mType = std::move(other.mType);
    mTypeName = std::move(other.mTypeName);
    return *this;
}

void AnyType::Swap(AnyType &left, AnyType &right)
{
    std::swap(left.mData,right.mData);
    std::swap(left.mType,right.mType);
    std::swap(left.mTypeName,right.mTypeName);
}

bool AnyType::toBool()
{
    if(mType == TypesEnum::Bool)
    {
        return mData.boolValue;
    }
    else
    {
        throw BadCast();
    }
}

char AnyType::toChar()
{
    if(mType == TypesEnum::Char)
    {
        return mData.boolValue;
    }
    else
    {
        throw BadCast();
    }
}

int AnyType::toInt()
{
    if(mType == TypesEnum::Int)
    {
        return mData.intValue;
    }
    else
    {
        throw BadCast();
    }
}

double AnyType::toDouble()
{
    if(mType == TypesEnum::Double)
    {
        return mData.boolValue;
    }
    else
    {
        throw BadCast();
    }
}

std::string AnyType::typeName() const
{
    return mTypeName;
}

TypesEnum AnyType::type() const
{
    return mType;
}
