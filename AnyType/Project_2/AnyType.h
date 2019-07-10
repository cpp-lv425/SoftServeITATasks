#pragma once
#include<iostream>

class AnyType
{
private:
	Types ValueType;//current type of the data
	Data data;//current data 
public:
	AnyType();

	~AnyType();

	template <typename T>
	AnyType(T value)//function can accept any types
	{
		if (std::is_same<T, bool>::value)// if type == bool
		{
			ValueType = Types::BOOL, data.boolData = value;// save data into the bool field and set ValueType  
		}
		//same code below
		else if (std::is_same<T, int>::value)
		{
			ValueType = Types::INT, data.intData = value;
		}

		else if (std::is_same<T, char>::value)
		{
			ValueType = Types::CHAR, data.charData = value;
		}

		else if (std::is_same<T, double>::value)
		{
			ValueType = Types::DOUBLE, data.doubleData = value;
		}

		else if (std::is_same<T, float>::value)
		{
			ValueType = Types::FLOAT, data.floatData = value;
		}

		else { throw std::invalid_argument("Invalid parametr."); }// if parament isn't equal to all of the data types from our union
	}

	AnyType(const AnyType &obj);

	AnyType(const AnyType &&obj);

	int ToInt();

	bool ToBool();

	double ToDouble();

	float ToFloat();

	char ToChar();

	void operator=(AnyType const & obj);

	void operator=(AnyType const &&obj);

	void operator=(bool var);

	void operator=(int var);

	void operator=(float var);

	void operator=(double var);

	friend std::ostream& operator<<(std::ostream &out, const AnyType &anyType);

	template<typename T>
	operator T()const// if we wanna use explicit cast operator () for unknown type
	{
		throw std::bad_cast{};
	}

	operator int()const;

	operator double()const;

	operator char()const;

	operator float()const;

	operator bool()const;
};

void swap(AnyType &obj1, AnyType &obj2);