// MyVar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Var.h"

using namespace std;
int main()
{
	Var a;
	a = 2;
	int c = a;
	cout << c << endl;
	a = 2.2;
	double d = a;
	cout << d << endl;
	a = true;
	a.value_type();
	bool e = a;
	cout << e << endl;
	a = 'm';
	char b = a;
	cout << b << endl;
	Var f(c);
	int p = f;
	cout << p << endl;
	double l = (double)f;
	cout << l;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
