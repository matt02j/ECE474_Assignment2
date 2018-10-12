#ifndef  TYPES_H
#define TYPES_H
using namespace std;

enum OPS { ADD, SUB, MUL, DIV, MUX, LSHIFT, RSHIFT, GT, LT, EQ, BAD};


class Input {
private:
	string name;
	int size;
	bool isSigned;
public:
	Input() {
		name = "";
		size = 0;
		isSigned = false;
	}
	Input(string name, int size, bool isSigned) {
		this->name = name;
		this->size = size;
		this->isSigned = isSigned;
	}
	string toString() {
		return name +" "+ to_string(size) + " " + to_string(isSigned);
	}

};
class Output {
private:
	string name;
	int size;
	bool isSigned;
public:
	Output() {
		name = "";
		size = 0;
		isSigned = false;
	}
	Output(string name, int size, bool isSigned) {
		this->name = name;
		this->size = size;
		this->isSigned = isSigned;
	}
	string toString() {
		return name + " " + to_string(size) + " " + to_string(isSigned);
	}

};
class Wire {
private:
	string name;
	int size;
	bool isSigned;

public:
	Wire() {
		name = "";
		size = 0;
		isSigned = false;
	}
	Wire(string name, int size, bool isSigned) {
		this->name = name;
		this->size = size;
		this->isSigned = isSigned;
	}
	string toString() {
		return name + " " + to_string(size) + " " + to_string(isSigned);
	}

};



#endif // ! TYPES_H

