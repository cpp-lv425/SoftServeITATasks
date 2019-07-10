#include "pch.h"
#include "Var.h"
#include<iostream>
using namespace std;
//constructors
Var::Var() {
	i = value_unknow;
}
Var::Var(int int_value_) {
	this->int_value = int_value_;
	i = value_int;
}
Var::Var(double double_value_) {
	this->double_value = double_value_;
	i = value_double;
}
Var::Var(bool bool_value_) {
	this->bool_value = bool_value_;
	i = value_bool;
}
Var::Var(char char_value_) {
	this->char_value = char_value_;
	i = value_char;
}

//destructor
Var::~Var() = default;

//leading to type int 
Var::operator int() const {
	try {
		if (i == value_int) {
			throw(this->int_value);
		}
		else throw('eror');
	}
	catch (int i) {
		return i;
	}
	catch (const char i[5]) {
		cout << i;
	}

}
//leading to type double
Var::operator double() const {
	try {
		if (i == value_double)throw(this->double_value);
		else throw("eror");
	}
	catch (double i) {
		return i;
	}
	catch (const char i[5]) {
		cout << i;
	}
}
//leading to type bool
Var::operator bool() const {
	try {
		if (i == value_bool) {
			throw(this->bool_value);
		}

		else throw('eror');
	}
	catch (bool i) {
		return i;
	}
	catch (const char i[5]) {
		cout << i;
	}
}
//leading to type char
Var::operator char() const {
	try {
		if (i == value_char)throw(this->char_value);
		else throw('eror');
	}
	catch (char i) {
		return i;
	}
	catch (const char i[5]) {
		std::cout << i;
	}
}

//copy constructor
Var::Var(const Var &other_Var) {
	this->i = other_Var.i; //define type flag
	switch (this->i) { //define value of Var
	case(value_int):
	{
		this->int_value = other_Var.int_value;
		break;
	}
	case(value_double): {
		this->double_value = other_Var.double_value;
		break;
	}
	case(value_bool): {
		this->bool_value = other_Var.bool_value;
		break;
	}
	case(value_char): {
		this->char_value = other_Var.char_value;
		break;
	}
	default: {
		break;
	}
	}
}

//move constructor
Var::Var(Var && other_Var) {
	if (this != &other_Var) { //check if Vars aren`t same
		this->i = other_Var.i; //change type flag
		switch (this->i) { //move value of Var
		case(value_int):
		{
			this->int_value = other_Var.int_value;
			break;
		}
		case(value_double): {
			this->double_value = other_Var.double_value;
			break;
		}
		case(value_bool): {
			this->bool_value = other_Var.bool_value;
			break;
		}
		case(value_char): {
			this->char_value = other_Var.char_value;
			break;
		}
		default: {
			break;
		}
		}
		other_Var.~Var(); //delete moved Var
	}
}

//assigment oparetor for int 
void Var::operator = (int int_value_) {
	this->int_value = int_value_;
	i = value_int;
}
//assigment oparetor for double
void Var::operator = (double double_value_) {
	this->double_value = double_value_;
	i = value_double;
}
//assigment oparetor for bool
void Var::operator = (bool bool_value_) {
	this->bool_value = bool_value_;
	i = value_bool;
}
//assigment oparetor for char
void Var::operator = (char char_value_) {
	this->char_value = char_value_;
	i = value_char;
}

//copy assigment operator
Var & Var::operator = (const Var & other_Var) {
	this->i = other_Var.i; //change the flag of type
	switch (this->i) { //change value of Var
	case(value_int):
	{
		this->int_value = other_Var.int_value;
		break;
	}
	case(value_double): {
		this->double_value = other_Var.double_value;
		break;
	}
	case(value_bool): {
		this->bool_value = other_Var.bool_value;
		break;
	}
	case(value_char): {
		this->char_value = other_Var.char_value;
		break;
	}
	default: {
		break;
	}
	}
	return *this;
}

//move assigment operator
Var & Var::operator = (Var && other_Var) {
	if (this != &other_Var) { //check if Vars are not the same
		this->i = other_Var.i;  //change the flag of type
		switch (this->i) {	//change value of Var
		case(value_int):
		{
			this->int_value = other_Var.int_value;
			break;
		}
		case(value_double): {
			this->double_value = other_Var.double_value;
			break;
		}
		case(value_bool): {
			this->bool_value = other_Var.bool_value;
			break;
		}
		case(value_char): {
			this->char_value = other_Var.char_value;
			break;
		}
		default: {
			break;
		}
		}
		other_Var.~Var();  //delete other Var
	}
	return *this;
}

//print the type of Var
void  Var::value_type() {
	switch (this->i)
	{
	case(value_int): {
		cout << "int" << endl;
		break;
	}
	case(value_bool): {
		cout << "bool" << endl;
		break;
	}
	case(value_double): {
		cout << "double" << endl;
		break;
	}
	case(value_char): {
		cout << "char" << endl;
		break;
	}
	default:
		break;
	}

}
//swap tvo Vars
void Var::swap(Var &b) {
	Var tmp;
	tmp = *this;
	*this = b;

}