#include <iostream>
#include <typeinfo>
#include <exception>
#include <string>

using std::cout;
using std::endl;
using std::string;

struct ICloneable {
	virtual ICloneable* clone() const = 0;
	virtual ICloneable* move() = 0;
	virtual ~ICloneable() { }
};

template <typename T>
struct ValueHolder : ICloneable {
	T data_;		
	ValueHolder(const T & arg) : data_ {arg} {}
	ValueHolder * clone() const { 
		return new ValueHolder(this->data_); 
	}
	ValueHolder * move() { 
		return this; 
	}
};

class Any {
	ICloneable * ptr;			// pointer ICloneable * of base class can contain any type of it template child:
						// ValueHolder<bool>, ValueHolder<int> or ValueHolder<double>
public:
	Any() : ptr {nullptr} { }

	template <typename T>
	Any(const T & arg) {
		if (typeid(T) != typeid(int) || typeid(T) != typeid(double) || typeid(T) != typeid(bool))
			ptr = nullptr;
		ptr = new  ValueHolder<T>(arg);
	}

	~Any() { delete ptr; };

	Any(const Any & arg){
		if (arg.ptr == nullptr)
			ptr = nullptr;
		else
			ptr = arg.ptr->clone();
	}

	Any& operator=(Any const & arg) {						
		if (this != & arg){
			delete ptr;
			ptr = arg.ptr->clone();
		}
		return * this;
	}
	
	Any(Any && arg){
		if (arg.ptr == nullptr)
			ptr = nullptr;
		else {		
			ptr = arg.ptr->move();	
			arg.ptr = nullptr;
		}
	}

	Any& operator=(Any && arg) {						
		if (this != & arg){
			if (ptr)	
				delete ptr;
			ptr = arg.ptr->move();
			arg.ptr = nullptr;
		}
		return * this;
	}

	template <typename T>
	T * cast() const {
		ValueHolder<T> * tmp = dynamic_cast<ValueHolder<T> *>(ptr);
		if (tmp == nullptr)
			return nullptr;
		return &(tmp->data_);
	}

	template <typename T>
	T castTo() const {		
		ValueHolder<T> * tmp = dynamic_cast<ValueHolder<T> *>(ptr);
		if (tmp == nullptr)
			throw BadCastTo();
		return tmp->data_;
	}
	
	string type() const {
		int compilerType =
		    #ifdef __clang__
		        0;
		    #elif defined(__GNUC__) || defined(__GNUG__)
		        0;
		    #elif _MSC_VER
		        1;
		    #endif
		string type = typeid(*ptr).name();
		switch (compilerType) {
			case 0:
				if (type == "11ValueHolderIiE") return "int";
				if (type == "11ValueHolderIbE") return "bool";
				if (type == "11ValueHolderIdE") return "double";
				return "empty";
			case 1:
				if (type == "struct ValueHolder<int>") return "int";
				if (type == "struct ValueHolder<bool>") return "bool";
				if (type == "struct ValueHolder<double>") return "double";
				return "empty";
			default:
				return "empty";
		}
	}
	static void swap(Any & lhs, Any & rhs) {
		Any tmp(std::move(lhs));
		lhs = std::move(rhs);
		rhs = std::move(tmp);
	}
	
	class BadCastTo : public std::exception {
		const char* _msg;
	public:        
		BadCastTo(const char* msg = "Bad castTo() exception: required type mismatch.\n"): _msg(msg) {    }
		const char* what() const noexcept override { return _msg; }
	};
};

int main(){
	cout << "start" << endl;
	
	Any empty;
	cout << "created empty object of Any type.\n" << endl;

	Any any1 = 1;
	Any any2 = 2.2;
	cout << "created 2 objects of Any type: any1 = 1, any2 = 2.2\n" << endl;
	int i1 = any1.castTo<int>();
	cout << "any1 casted to int:" << i1 << endl;
	string type = any1.type();
	cout << "any1 has type:" << type << endl;

	double d2 = any2.castTo<double>();
	cout << "any2 casted to double:" << d2 << endl;
	type = any2.type();
	cout << "any2 has type:" << type << endl;

	cout << "\nswaping ..." << endl;
	Any::swap(any1, any2);
	cout << "\t\tswaped." << endl;

	i1 = any2.castTo<int>();
	cout << "i1:" << i1 << endl;
	type = any2.type();
	cout << "any2 has type:" << type << endl;

	d2 = any1.castTo<double>();
	cout << "d2:" << d2 << endl;
	type = any1.type();
	cout << "any1 has type:" << type << endl;

	cout << "\nsizeof any1:" << sizeof(any1) << endl;
	cout << "sizeof any2:" << sizeof(any2) << endl;
	cout << "sizeof empty Any:" << sizeof(empty) << endl;

	cout << "\nany1 assigning to bool" << endl;
	any1 = true;
	bool flag = any1.castTo<bool>();
	cout << "flag:" << flag << endl;
	type = any1.type();
	cout << "any1 has type:" << type << endl;

	cout << "\nany1 assigning to double:" << endl;
	any1 = 1.7;
	d2 = any1.castTo<double>();
	cout << "d2:" << d2 << endl;
	type = any1.type();
	cout << "any1 has type:" << type << endl;
	
	cout << "\nbad cast situation modelling: casting any1 to int." << endl;
	try {
		i1 = any1.castTo<int>();
	} catch (const Any::BadCastTo& e) {
		std::cerr << e.what();
	}

	cout << "\nwrong type assignment situation modelling: Any any3 = short which not supported." << endl;
	short sh = 2;
	Any any3 = sh;
	type = any3.type();
	cout << "any3 has type:" << type << endl;

	return 0;	
}

//https://rextester.com/l/cpp_online_compiler_visual
