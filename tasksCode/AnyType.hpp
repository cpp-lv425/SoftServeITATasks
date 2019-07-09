/*
	Class for storing the data of any fundamental datatype. The data can be
	casted according to the needs, but if the casting target type is not the
	same as the internal datatype the exception bad_cast will be thrown. Also
	if the class have to receive non fundamental type, the exception bad_typeid
	will be thrown.
	Author:						Valentyn Faychuk
	E-mail:						faitchouk.valentyn@gmail.com
	Created on:					08.07.2019
*/
#pragma once

#include <typeinfo>
#include <string>

class AnyType {
private:
	// Structure for storing type of the given value
	enum ValueType
	{
		TypeVoid,				TypeBoolean,
		TypeChar,				TypeUnsignedChar,
		TypeInt,				TypeShortInt,
		TypeLongInt,			TypeLongLongInt,
		TypeUnsignedInt,		TypeUnsignedShortInt,
		TypeUnsignedLongInt,	TypeUnsignedLongLongInt,
		TypeDouble,				TypeLongDouble,
		TypeFloat
	} valueType;

	// Structure for storing the given value
	union ValueData
	{
		bool					valueBool;
		char					valueChar;
		char					valueUChar;
		long long int			valueLLInt;
		long long unsigned int	valueLLUInt;
		long double				valueLDouble;
	} * valueData = nullptr;

	// Set the default internal value' data
	void SetValueData();

	// Set the internal value' data, based on the give value
	template <typename Type>
	void SetValueData(const Type & value);
	
	// Set the default internal value' type
	void SetValueType();
	
	// Set the internal value' type, based on the give value
	template <typename Type>
	void SetValueType(const Type & value);

public:
	// Default constructor
	AnyType() {
		SetValueData();
		SetValueType();
	};
	
	// Overloaded parametrized constructor
	template <typename Type>
	AnyType(Type value) {
		SetValueData(value);
		SetValueType(value);
	};
	
	// Copy constructor
	AnyType(const AnyType & anyType) {
		if (anyType.valueData == nullptr) {
			// If anyType parameter has deactivated state
			// deactivate state of the *this object
			if (valueData != nullptr)
				delete valueData;
			valueData = nullptr;
			this->valueType = anyType.valueType;
			return;
		}
		SetValueData();
		*(this->valueData) = *(anyType.valueData);
		this->valueType = anyType.valueType;
	};
	
	// Move constructor
	AnyType(AnyType && anyType) {
		// Deactivate state of the anyType parameter
		this->valueData = anyType.valueData;
		anyType.valueData = nullptr;
		this->valueType = anyType.valueType;
		anyType.SetValueType();
	};
	
	// Copy assignment operator
	AnyType& operator=(const AnyType & anyType) {
		if (anyType.valueData == nullptr) {
			// If anyType parameter has deactivated state
			// deactivate state of the *this object
			if (valueData != nullptr)
				delete valueData;
			valueData = nullptr;
			this->valueType = anyType.valueType;
			return *this;
		}
		SetValueData();
		*(this->valueData) = *(anyType.valueData);
		this->valueType = anyType.valueType;
		return *this;
	};
	
	// Move assignment operator
	AnyType& operator=(AnyType && anyType) {
		this->valueData = anyType.valueData;
		anyType.valueData = nullptr;
		this->valueType = anyType.valueType;
		anyType.SetValueType();
		return *this;
	};
	
	// Overloaded parametrized copy assignment operator
	template <typename Type>
	AnyType& operator=(const Type & value) {
		SetValueData(value);
		SetValueType(value);
		return *this;
	};
	
	// Overloaded parametrized move assignment operator
	template <typename Type>
	AnyType& operator=(Type && value) {
		SetValueData(std::move(value));
		SetValueType(std::move(value));
		return *this;
	};

	// Overloaded addition assignment operator
	template <typename Type>
	AnyType& operator+=(const Type & value) {
		SetValueData(value + static_cast<Type>(*this));
		return *this;
	};
	
	// Overloaded subtraction assignment operator
	template <typename Type>
	AnyType& operator-=(const Type & value) {
		SetValueData(value - static_cast<Type>(*this));
		return *this;
	};
	
	// Overloaded multiplication assignment operator
	template <typename Type>
	AnyType& operator*=(const Type & value) {
		SetValueData(value * static_cast<Type>(*this));
		return *this;
	};
	
	// Overloaded division assignment operator
	template <typename Type>
	AnyType& operator/=(const Type & value) {
		SetValueData(static_cast<Type>(*this) / value);
		return *this;
	};
	
	// Transform to bool, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator bool() const {
		if (valueType != ValueType::TypeBoolean)
			throw std::bad_cast{ };
		return static_cast<bool>((*valueData).valueBool);
	};
	
	// Transform to char, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator char() const {
		if (valueType != ValueType::TypeChar)
			throw std::bad_cast{ };
		return static_cast<char>((*valueData).valueChar);
	};
	
	// Transform to unsigned char, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator unsigned char() const {
		if (valueType != ValueType::TypeUnsignedChar)
			throw std::bad_cast{ };
		return static_cast<unsigned char>((*valueData).valueUChar);
	};
	
	// Transform to int, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator int() const {
		if (valueType != ValueType::TypeInt)
			throw std::bad_cast{ };
		return static_cast<int>((*valueData).valueLLInt);
	};
	
	// Transform to short, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator short int() const {
		if (valueType != ValueType::TypeShortInt)
			throw std::bad_cast{ };
		return static_cast<short int>((*valueData).valueLLInt);
	};
	
	// Transform to long, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator long int() const {
		if (valueType != ValueType::TypeLongInt)
			throw std::bad_cast{ };
		return static_cast<long int>((*valueData).valueLLInt);
	};
	
	// Transform to long long, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator long long int() const {
		if (valueType != ValueType::TypeLongLongInt)
			throw std::bad_cast{ };
		return static_cast<long long int>((*valueData).valueLLInt);
	};
	
	// Transform to unsigned, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator unsigned int() const {
		if (valueType != ValueType::TypeUnsignedInt)
			throw std::bad_cast{ };
		return static_cast<unsigned int>((*valueData).valueLLUInt);
	};
	
	// Transform to unsigned short, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator unsigned short int() const {
		if (valueType != ValueType::TypeUnsignedShortInt)
			throw std::bad_cast{ };
		return static_cast<unsigned short>((*valueData).valueLLUInt);
	};
	
	// Transform to unsigned long, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator unsigned long int() const {
		if (valueType != ValueType::TypeUnsignedLongInt)
			throw std::bad_cast{ };
		return static_cast<unsigned long int>((*valueData).valueLLUInt);
	};
	
	// Transform to unsigned long long, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator unsigned long long int() const {
		if (valueType != ValueType::TypeUnsignedLongLongInt)
			throw std::bad_cast{ };
		return static_cast<unsigned long long int>((*valueData).valueLLUInt);
	};
	
	// Transform to float, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator float() const {
		if (valueType != ValueType::TypeFloat)
			throw std::bad_cast{ };
		return static_cast<float>((*valueData).valueLDouble);
	};
	
	// Transform to double, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator double() const {
		if (valueType != ValueType::TypeDouble)
			throw std::bad_cast{ };
		return static_cast<double>((*valueData).valueLDouble);
	};
	
	// Transform to long double, if the internal type is bool,
	// otherwise throw bad_cast exeption
	operator long double() const {
		if (valueType != ValueType::TypeLongDouble)
			throw std::bad_cast{ };
		return static_cast<long double>((*valueData).valueLDouble);
	};

	// Transform to other types, not implemented in the class,
	// generally always throw bad_cast exeption
	template <typename Type>
	operator Type() const {
		throw std::bad_cast{ };
	};

	// Return value' type
	std::string GetValueType() const;
	
	// Delete internal data
	void DeleteData();
	
	// Swap two objects
	void Swap(AnyType & anyType);
	
	// Swap two objects
	friend void Swap(AnyType & anyTypeA, AnyType & anyTypeB);

	// Default destructor
	~AnyType() {
		if (valueData != nullptr)
			delete valueData;
	}
};

// Set the default internal value' data
void AnyType::SetValueData()
{
	if (valueData == nullptr)
		valueData = new ValueData;
	// Set all valueData bytes to 0 (the long long int has 64 bytes
	// which is the max fundamental datatype size)
	(*valueData).valueLLInt = 0;
}

// Set the internal value' data, based on the give value
template <typename Type>
void AnyType::SetValueData(const Type & value)
{
	if (valueData == nullptr)
		valueData = new ValueData;
	// Iterate through possible types to define what value interpretation should be choosen
	// The write the value into the value' data according to the correct interpreation
	auto valueTypeId = typeid(value).name();
	if		(valueTypeId == typeid( bool					).name()) (*valueData).valueBool = static_cast<bool>(value);
	else if (valueTypeId == typeid( char					).name()) (*valueData).valueChar = static_cast<char>(value);
	else if (valueTypeId == typeid( unsigned char			).name()) (*valueData).valueUChar = static_cast<unsigned char>(value);
	else if (valueTypeId == typeid( int						).name()) (*valueData).valueLLInt = static_cast<long long>(value);
	else if (valueTypeId == typeid( short int				).name()) (*valueData).valueLLInt = static_cast<long long>(value);
	else if (valueTypeId == typeid( long int				).name()) (*valueData).valueLLInt = static_cast<long long>(value);
	else if (valueTypeId == typeid( long long int			).name()) (*valueData).valueLLInt = static_cast<long long>(value);
	else if (valueTypeId == typeid( unsigned int			).name()) (*valueData).valueLLUInt = static_cast<unsigned long long>(value);
	else if (valueTypeId == typeid( unsigned short int		).name()) (*valueData).valueLLUInt = static_cast<unsigned long long>(value);
	else if (valueTypeId == typeid( unsigned long int		).name()) (*valueData).valueLLUInt = static_cast<unsigned long long>(value);
	else if (valueTypeId == typeid( unsigned long long int	).name()) (*valueData).valueLLUInt = static_cast<unsigned long long>(value);
	else if (valueTypeId == typeid( float					).name()) (*valueData).valueLDouble = static_cast<long double>(value);
	else if (valueTypeId == typeid( double					).name()) (*valueData).valueLDouble = static_cast<long double>(value);
	else if (valueTypeId == typeid( long double				).name()) (*valueData).valueLDouble = static_cast<long double>(value);
	else throw std::bad_typeid{ }; // Cast exception if the type is unknown
}

// Set the default internal value' type
void AnyType::SetValueType()
{
	valueType = ValueType::TypeVoid;
}

// Set the internal value' type, based on the give value
template <typename Type>
void AnyType::SetValueType(const Type & value)
{
	// Iterate through possible types and define what type does the value have
	// Then write the type to the state variable valueType
	auto valueTypeId = typeid(value).name();
	if		(valueTypeId == typeid( bool					).name()) valueType = ValueType::TypeBoolean;
	else if (valueTypeId == typeid( char					).name()) valueType = ValueType::TypeChar;
	else if (valueTypeId == typeid( unsigned char			).name()) valueType = ValueType::TypeUnsignedChar;
	else if (valueTypeId == typeid( int						).name()) valueType = ValueType::TypeInt;
	else if (valueTypeId == typeid( short int				).name()) valueType = ValueType::TypeShortInt;
	else if (valueTypeId == typeid( long int				).name()) valueType = ValueType::TypeLongInt;
	else if (valueTypeId == typeid( long long int			).name()) valueType = ValueType::TypeLongLongInt;
	else if (valueTypeId == typeid( unsigned int			).name()) valueType = ValueType::TypeUnsignedInt;
	else if (valueTypeId == typeid( unsigned short int		).name()) valueType = ValueType::TypeUnsignedShortInt;
	else if (valueTypeId == typeid( unsigned long int		).name()) valueType = ValueType::TypeUnsignedLongInt;
	else if (valueTypeId == typeid( unsigned long long int	).name()) valueType = ValueType::TypeUnsignedLongLongInt;
	else if (valueTypeId == typeid( float					).name()) valueType = ValueType::TypeFloat;
	else if (valueTypeId == typeid( double					).name()) valueType = ValueType::TypeDouble;
	else if (valueTypeId == typeid( long double				).name()) valueType = ValueType::TypeLongDouble;
	else throw std::bad_typeid{ }; // Cast exception if the type is unknown
}

// Return value' type
std::string AnyType::GetValueType() const
{
	switch (valueType)
	{
		case ValueType::TypeBoolean:				return std::string("bool");
		case ValueType::TypeChar:					return std::string("char");
		case ValueType::TypeUnsignedChar:			return std::string("unsigned char");
		case ValueType::TypeInt:					return std::string("int");
		case ValueType::TypeShortInt:				return std::string("short int");
		case ValueType::TypeLongInt:				return std::string("long int");
		case ValueType::TypeLongLongInt:			return std::string("long long int");
		case ValueType::TypeUnsignedInt:			return std::string("unsigned int");
		case ValueType::TypeUnsignedShortInt:		return std::string("unsigned short int");
		case ValueType::TypeUnsignedLongInt:		return std::string("unsigned long int");
		case ValueType::TypeUnsignedLongLongInt:	return std::string("unsigned long long int");
		case ValueType::TypeFloat:					return std::string("float");
		case ValueType::TypeDouble:					return std::string("double");
		case ValueType::TypeLongDouble:				return std::string("long double");
		case ValueType::TypeVoid:					return std::string("no type");
		default:									return std::string("N/A");
	}
}

// Delete internal data
void AnyType::DeleteData()
{
	if (valueData != nullptr)
		delete valueData;
	valueData = nullptr;
	SetValueType();
}

// Swap two objects
void AnyType::Swap(AnyType & anyType)
{
	AnyType tmpAnyType(std::move(*this));
	*this = std::move(anyType);
	anyType = std::move(tmpAnyType);
}

// Swap two object of the AnyType class 
void Swap(AnyType & anyTypeA, AnyType & anyTypeB)
{
	AnyType tmpAnyType(std::move(anyTypeA));
	anyTypeA = std::move(anyTypeB);
	anyTypeB = std::move(tmpAnyType);
}