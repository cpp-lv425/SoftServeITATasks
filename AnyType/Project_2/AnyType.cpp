#include"DataStructures.h"
#include"AnyType.h"
#include<iostream>



AnyType::AnyType()// empty constructor
{
	ValueType = Types::UNKNOWN_TYPE;
}

AnyType::~AnyType() = default;

AnyType::AnyType(const AnyType &obj)//copy constructor
{
	data = obj.data;
	ValueType = obj.ValueType;
}

AnyType::AnyType(AnyType &&obj)//move constructor
{
	data = obj.data;
	ValueType = obj.ValueType;
}

char AnyType::ToChar()
{
	if (ValueType != Types::CHAR)// check if current value type != char
		throw AnyType::BadCastException();//if it's not char throw bad cast exception
	return data.charData;//if it's char - return char value
}

int AnyType::ToInt()
{
	if (ValueType != Types::INT)
		throw AnyType::BadCastException();
	return data.intData;
}

double AnyType::ToDouble()
{
	if (ValueType != Types::DOUBLE)
		throw AnyType::BadCastException();
	return data.doubleData;
}

bool AnyType::ToBool()
{
	if (ValueType != Types::BOOL)
		throw AnyType::BadCastException();
	return data.boolData;
}

float AnyType::ToFloat()
{
	if (ValueType != Types::FLOAT)
		throw AnyType::BadCastException();
	return (data.floatData);
}

void AnyType::operator=(AnyType const & obj)//copy assigment
{
	if (this != &obj)
	{
		data = obj.data;
		ValueType = obj.ValueType;
	}
}

void AnyType::operator=(AnyType &&obj)//move assigment
{
	if (this != &obj)
	{
		data = obj.data;
		ValueType = obj.ValueType;
	}
}

void AnyType::operator=(bool var)//overloaded operator =(we get the data type(in this case bool) and fill field of this type in the union)
{
	data.boolData = var;
	ValueType = Types::BOOL;
}
//we'll do the same below for all types

void AnyType::operator=(int var)
{
	data.intData = var;
	ValueType = Types::INT;
}

void AnyType::operator=(float var)
{
	data.floatData = var;
	ValueType = Types::FLOAT;
}

void AnyType::operator=(double var)
{
	data.doubleData = var;
	ValueType = Types::DOUBLE;
}

std::ostream& operator<<(std::ostream &out, const AnyType &anyType)//overloaded << for print // get data and type mhetod
{
	// after figuring out what data type is we print equal value from inion
	std::cout << "Data = ";
	switch (anyType.ValueType)
	{
	case Types::BOOL:   out << anyType.data.boolData << ". Type = bool" << std::endl;       break;
	case Types::INT:   out << anyType.data.intData << ". Type = int" << std::endl;	        break;
	case Types::DOUBLE:   out << anyType.data.doubleData << ". Type = double" << std::endl; break;
	case Types::FLOAT:   out << anyType.data.floatData << ". Type = float" << std::endl;	break;
	case Types::CHAR:   out << anyType.data.charData << ". Type = char" << std::endl;	    break;
	default: out << "Type is undefined" << std::endl;// if unknown type
	}
	return out;// return output stream
}

AnyType::operator int()const// overloaded ()operator
{
	if (ValueType != Types::INT)// if our type != int
		throw AnyType::BadCastException();// this operation can be unsafe. throw bad_cast.
	return data.intData;// if anything is ok return value of our cast
}
//we'll do the same below for all types
AnyType::operator double()const
{
	if (ValueType != Types::DOUBLE)
		throw AnyType::BadCastException();
	return data.doubleData;
}

AnyType::operator char()const
{
	if (ValueType != Types::CHAR)
		throw AnyType::BadCastException();
	return data.charData;
}

AnyType::operator float()const
{
	if (ValueType != Types::FLOAT)
		throw AnyType::BadCastException();
	return data.floatData;
}

AnyType::operator bool()const
{
	if (ValueType != Types::BOOL)
		throw AnyType::BadCastException();
	return data.boolData;
}

void swap(AnyType &obj1, AnyType &obj2)// swap function
{
	AnyType temp = obj1;
	obj1 = obj2;
	obj2 = temp;
}
