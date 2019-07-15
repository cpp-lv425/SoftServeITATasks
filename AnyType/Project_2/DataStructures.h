#pragma once
enum Types {//posible data types
	BOOL = 0,
	INT,
	CHAR,
	DOUBLE,
	FLOAT,
	UNKNOWN_TYPE
};

union Data // union which'll save one type
{
	bool boolData;
	int intData;
	char charData;
	double doubleData;
	float floatData;;
};