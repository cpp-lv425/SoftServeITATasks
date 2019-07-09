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

template<class T>
inline int getTypeId(T value)
{
    return 0;
}

template<>
inline int getTypeId(int value)
{
    return static_cast<int>(TypeNum::INT);
}

template<>
inline int getTypeId(bool value)
{
    return static_cast<int>(TypeNum::BOOL);
}

template<>
inline int getTypeId(double value)
{
    return static_cast<int>(TypeNum::DOUBLE);
}

template<>
inline int getTypeId(float value)
{
    return static_cast<int>(TypeNum::FLOAT);
}

template<>
inline int getTypeId(char value)
{
    return static_cast<int>(TypeNum::CHAR);
}



#endif // TYPEID_H
