#ifndef TYPEID_H
#define TYPEID_H

enum class TypeNum
{
    UNKNOWN_TYPE = 0,
    INT,
    BOOL,
    DOUBLE,
    FLOAT,
    CHAR
};

const char typeNames[][20]
{
    "unknown type",
    "int",
    "bool",
    "double",
    "float",
    "char"
};


#endif // TYPEID_H
