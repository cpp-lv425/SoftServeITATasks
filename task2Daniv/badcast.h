#ifndef BADCAST_H
#define BADCAST_H

#include <exception>

class BadCast : public std::exception
{
public:
    virtual const char* what() const noexcept override
    {
        return "Can't cast this type";
    }
};
#endif // BADCAST_H
