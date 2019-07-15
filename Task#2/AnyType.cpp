//
// Created by oleks on 15.07.2019.
//

#include "AnyType.h"

void AnyType::clear() {
    // Set union data to zero
    memset(&data, 0, sizeof(data));
    type = none;
}

void AnyType::swap(AnyType &anyType) {
    // Swap data
    Data tmp = data;
    data = anyType.data;
    anyType.data = tmp;
    // Swap type
    Type tmp_type = type;
    type = anyType.type;
    anyType.type = tmp_type;
}

AnyType::AnyType() {
    clear();
}

AnyType::AnyType(double _d) {
    data.d = _d;
    type = type_double;
}

AnyType::AnyType(int _i) {
    data.i = _i;
    type = type_int;
}

AnyType::AnyType(char _c) {
    data.c = _c;
    type = type_char;
}

AnyType::AnyType(bool _b) {
    data.b = _b;
    type = type_bool;
}

AnyType::AnyType(const AnyType &anyType) {
    data = anyType.data;
    type = anyType.type;
}

AnyType::AnyType(AnyType &&anyType) {
    data = anyType.data;
    type = anyType.type;
    anyType.clear();
}

AnyType& AnyType::operator=(double _d) {
    data.d = _d;
    type = type_double;
    return *this;
}

AnyType& AnyType::operator=(AnyType &&anyType) {
    if(this != &anyType) {
        data = anyType.data;
        type = anyType.type;
        anyType.clear();
    }
    return *this;
}

AnyType& AnyType::operator=(int _i) {
    data.i = _i;
    type = type_int;
    return *this;
}

AnyType& AnyType::operator=(char _c) {
    data.c = _c;
    type = type_char;
    return *this;
}

AnyType& AnyType::operator=(bool _b) {
    data.b = _b;
    type = type_bool;
    return *this;
}

double AnyType::toDouble() const {
    // Raise exception if bad cast
    if (type != type_double)
        throw std::bad_cast();
    return data.d;
}

int AnyType::toInt() const {
    if (type != type_int)
        throw std::bad_cast();
    return data.i;
}

char AnyType::toChar() const {
    if (type != type_char)
        throw std::bad_cast();
    return data.c;
}

bool AnyType::toBool() const {
    if (type != type_bool)
        throw std::bad_cast();
    return data.b;
}

Type AnyType::getType() const {
    return type;
}