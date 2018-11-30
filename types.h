#ifndef  TYPES_H
#define TYPES_H
#include <string>
using namespace std;

enum OPS { ADD, SUB, MUL, DIV, MUX, LSHIFT, RSHIFT, GT, LT, EQ, REG, BAD};

class Variable {
public:
	string name;
	int size;
	bool isSigned;
	int type;
	Variable() {
		name = "";
		size = 0;
		isSigned = false;
	}
	Variable(string name, int size, bool isSigned) {
		this->name = name;
		this->size = size;
		this->isSigned = isSigned;
	}
	string toString() {
		return name + " " + to_string(size) + " " + to_string(isSigned);
	}
	inline bool operator==(Variable b) {
		return (this->name == b.name && this->size == b.size && this->isSigned == b.isSigned);
	}
	inline bool operator==(string b) {
		return (this->name==b);
	}
};

class Input: public Variable{
public: 
	Input(string name, int size, bool isSigned) {
		this->name = name;
		this->size = size;
		this->isSigned = isSigned;
		this->type = 1;
	}
};
class Output: public Variable {
public:
	Output(string name, int size, bool isSigned) {
		this->name = name;
		this->size = size;
		this->isSigned = isSigned;
		this->type = 0;
	}
};
class Wire : public Variable {
public:
	Wire(string name, int size, bool isSigned) {
		this->name = name;
		this->size = size;
		this->isSigned = isSigned;
		this->type = 2;
	}
};

class Operation {
public:
	OPS op;
	Variable in1, in2, in3, out;
	int alap,asap,final;
	vector<Operation*> iDependOn,dependOnMe;
	Operation(Variable in1, Variable in2, Variable out) {
		this->in1 = in1;
		this->in2 = in2;
		this->out = out;
	}
	Operation(Variable in1, Variable in2, Variable in3, Variable out) {
		this->in1 = in1;
		this->in2 = in2;
		this->in3 = in3;
		this->out = out;
	}

};


#endif // ! TYPES_H

