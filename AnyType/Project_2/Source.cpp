#include<iostream>
#include<typeinfo>
#include"DataStructures.h"
#include"AnyType.h"

int main()
{
	AnyType anyType = false;
	std::cout << anyType;
	anyType = 12.6;
	std::cout << anyType << std::endl;
	try
	{
		int storedValue = anyType.ToInt();
	}
	catch (std::bad_cast& bc)
	{
		std::cout << "Forbidden"<<std::endl;
		//Bad cast exception
	}

	anyType = 21.3232;
	try
	{
		double *ptr_test = (double*)anyType;
	}
	catch (std::bad_cast& bc)
	{
		std::cout << "Forbidden"<<std::endl;
	}
	std::cout << anyType;
	std::cin.get();
	return 0;
}
