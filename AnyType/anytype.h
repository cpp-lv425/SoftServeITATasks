#ifndef ANYTYPE_H
#define ANYTYPE_H
#include <exception>
#include "typeid.h"
#include <vector>

class AnyType
{
public:    
    // creates empty object
    // sets contained value to UNKNOWN_TYPE
    AnyType();
    // as class does not allocate memory dynamically
    // there is no need to supply custom copy ctor
    AnyType(const AnyType& other) = default;

    // contained value of moved object is set
    // to UNKNOWN_TYPE
    AnyType(AnyType&& other);

    AnyType(int i_);
    AnyType(bool b_);
    AnyType(double d_);
    AnyType(float f_);
    AnyType(char c_);

    // as class does not allocate memory dynamically
    // there is no need to overload copy assignment operators
    AnyType& operator=(const AnyType& other) = default;

    // contained value of moved object is set
    // to UNKNOWN_TYPE
    AnyType& operator=(AnyType&& other);

    AnyType& operator=(int i_);
    AnyType& operator=(bool b_);
    AnyType& operator=(double d_);
    AnyType& operator=(float f_);
    AnyType& operator=(char c_);

    int toInt()const;
    bool toBool()const;
    float toFloat()const;
    double toDouble()const;
    char toChar()const;

    void swap(AnyType& other);

    // sets contained value to UNKNOWN_TYPE
    // so that it cannot be retrieved from the object
    void clear();

    TypeNum type() const;
    const char* typeName()const;

    // as class does not allocate memory dynamically
    // there is no need to supply custom dtor
    ~AnyType() = default;

    // indicates that requested type does not
    // match stored type
    class BadAnyCast: public std::exception
    {
        const char* mmsg;
    public:        
        BadAnyCast(const char* msg = "BadAnyCast exception: requested type does not match stored type."):
            std::exception(), mmsg(msg)
        {            
        }
        virtual const char* what() const noexcept override
        {
            return mmsg;
        }
    };

private:
    struct Holder
    {
        Holder(TypeNum typeNum_ = TypeNum::UNKNOWN_TYPE):
            typeNum(typeNum_)
        {
        }

        TypeNum typeNum;
        union Data
        {
            int i;
            bool b;
            double d;
            float f;
            char c;
        };
        Data data;
    };

    Holder holder;
};

#endif // ANYTYPE_H
