#pragma once
class Var
{
	//emun of possible type of Var
	enum type {
		value_int = 'int',
		value_double = 'doub',
		value_bool = 'bool',
		value_char = 'char',
		value_unknow = 'NULL'
	};
	//union to save Var
	union {

		int int_value;
		double double_value;
		bool bool_value;
		char char_value;
	};
	type i; //"flag" with type of Var

public:
	//default constructors
	Var();
	Var(int int_value_);
	Var(double double_value_);
	Var(bool bool_value_);
	Var(char char_value_);
	
	//copy constructor
	Var(const Var &other_Var);

	//move constructor
	Var(Var && other_Var); 

	//assigment operator
	void operator = (int int_value_);
	void operator = (double double_value_);
	void operator = (bool bool_value_);
	void operator = (char char_value_);

	//move assigment operator
	Var & operator = (Var && other_Var); 

	//copy assigment operator
	Var & operator = (const Var & other_Var);

	//cast to type operator
	operator int() const;
	operator double() const;
	operator bool() const;
	operator char() const;

	//print the type of Var
	void value_type();
	//swap two Vars 
	void swap(Var &b);
	
	//destructor
	~Var();
};

