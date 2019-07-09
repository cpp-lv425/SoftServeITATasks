#include "anytype.h"
#include <iostream>


AnyType::AnyType():holder(TypeNum::UNKNOWN_TYPE)
{
}

AnyType::AnyType(AnyType&& other)
{
    holder = other.holder;
    other.clear();
}

AnyType::AnyType(int i_): holder(TypeNum::INT)
{
    holder.data.i = i_;
}

AnyType::AnyType(bool b_): holder(TypeNum::BOOL)
{
    holder.data.b = b_;
}

AnyType::AnyType(double d_): holder(TypeNum::DOUBLE)
{
    holder.data.d = d_;
}

AnyType::AnyType(float f_): holder(TypeNum::FLOAT)
{
    holder.data.f = f_;
}

AnyType::AnyType(char c_): holder(TypeNum::CHAR)
{
    holder.data.c = c_;
}

AnyType& AnyType::operator=(int i_)
{
    holder.typeNum = static_cast<int>(TypeNum::INT);
    holder.data.i = i_;
    return *this;
}

AnyType &AnyType::operator=(AnyType &&other)
{
    if(this != &other)
    {
        holder = other.holder;
        other.clear();
    }
    return *this;
}

AnyType &AnyType::operator=(bool b_)
{
    holder.typeNum = static_cast<int>(TypeNum::BOOL);
    holder.data.b = b_;
    return *this;
}

AnyType &AnyType::operator=(double d_)
{
    holder.typeNum = static_cast<int>(TypeNum::DOUBLE);
    holder.data.d = d_;
    return *this;
}

AnyType &AnyType::operator=(float f_)
{
    holder.typeNum = static_cast<int>(TypeNum::FLOAT);
    holder.data.f = f_;
    return *this;
}

AnyType &AnyType::operator=(char c_)
{
    holder.typeNum = static_cast<int>(TypeNum::CHAR);
    holder.data.c = c_;
    return *this;
}



int AnyType::toInt() const
{
    if(holder.typeNum != getTypeId(holder.data.i))
        throw AnyType::BadAnyCast();

    return holder.data.i;
}

bool AnyType::toBool() const
{
    if(holder.typeNum != getTypeId(holder.data.b))
        throw AnyType::BadAnyCast();

    return holder.data.b;
}

float AnyType::toFloat() const
{
    if(holder.typeNum != getTypeId(holder.data.f))
        throw AnyType::BadAnyCast();

    return holder.data.f;
}

double AnyType::toDouble() const
{
    if(holder.typeNum != getTypeId(holder.data.d))
        throw AnyType::BadAnyCast();

    return holder.data.d;
}

char AnyType::toChar() const
{
    if(holder.typeNum != getTypeId(holder.data.c))
        throw AnyType::BadAnyCast();

    return holder.data.c;
}



void AnyType::swap(AnyType &other)
{
    std::swap(holder, other.holder);
}

void AnyType::clear()
{
    holder.typeNum = static_cast<int>(TypeNum::UNKNOWN_TYPE);
}

TypeNum AnyType::type() const
{
    return static_cast<TypeNum>(holder.typeNum);
}

const char *AnyType::typeName() const
{
    return typeNames.at(holder.typeNum);
}






