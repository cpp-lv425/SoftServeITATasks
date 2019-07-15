//
// Created by oleks on 15.07.2019.
//

#ifndef UNTITLED5_ANYTYPE_H
#define UNTITLED5_ANYTYPE_H

#include <iostream>


enum Type {none, type_int, type_char, type_double, type_bool};

union Data {
    double d;
    int i;
    char c;
    bool b;
};


class AnyType {
private:
    Type type;
    Data data;
public:
    AnyType();
    explicit AnyType(double);
    explicit AnyType(int);
    explicit AnyType(char);
    explicit AnyType(bool);
    AnyType(const AnyType&);
    AnyType(AnyType&&);
    AnyType& operator=(AnyType&&);
    AnyType& operator=(double);
    AnyType& operator=(int);
    AnyType& operator=(char);
    AnyType& operator=(bool);
    void clear();
    double toDouble() const;
    int toInt() const;
    char toChar() const;
    bool toBool() const;
    Type getType() const;
    void destroy();
    void swap(AnyType&);
};


#endif //UNTITLED5_ANYTYPE_H
