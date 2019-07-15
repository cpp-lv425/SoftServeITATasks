#include<iostream>
#include<typeinfo>
#include"DataStructures.h"
#include"AnyType.h"

int main()
{
	AnyType anyType = false;
	std::cout << anyType;
	anyType = 12.6323223;
	std::cout << anyType << std::endl;
	anyType = 21.3232;
	try
	{
		double *ptr_test = (double*)anyType;
	}
	catch (const AnyType::BadCastException &ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	double val = anyType.ToDouble();
	std::cout << val << std::endl;
	std::cin.get();
	return 0;
}
