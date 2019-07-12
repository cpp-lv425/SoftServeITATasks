//
// Created by oleks on 10.07.2019.
//

#ifndef UNTITLED_ANYTYPE_H
#define UNTITLED_ANYTYPE_H

#include <iostream>
#include <exception>
#include <cstring>

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
    void swap(AnyType*);
};


#endif //UNTITLED_ANYTYPE_H
