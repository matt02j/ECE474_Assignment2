// 474asignment2.cpp : Defines the entry point for the console application.
//

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "types.h"

using namespace std;

void parseInputs(stringstream *stream, vector<Input>* inputs);
void parseOutputs(stringstream *stream, vector<Output>* Outputs);
void parseWires(stringstream *stream, vector<Wire>* wires);
OPS parseOp(string myop);

int main(int argc, char* argv[]){
	 string netlistFileName, verilogFileName;
	 ifstream netlistFile;
	 ofstream verilogFile;
	 string buffer,seg1,seg2,seg3;
	 stringstream stream;
	 vector<Input> inputs;
	 vector<Output> outputs;
	 vector<Wire> wires;

	 int numAdders=0;
	 int numComps=0;
	 int numSubs=0;
	 int numMuls = 0;
	 int numDivs=0;
	 int numRShifts=0;
	 int numLShifts=0;

	if (argc != 3) {
		cout << "Usage: dpgen netlistFile verilogFile"<<endl;
		return 1;
	}
	netlistFileName = argv[1];
	verilogFileName = argv[2];


	netlistFile.open(netlistFileName);
	verilogFile.open(verilogFileName);

	// make input/output/wire classes?
	//convert to cdfg

	while (!netlistFile.eof()) {
		seg1.clear();
		buffer.clear();
		stream.clear();
		getline(netlistFile, buffer);
		stream << buffer;
		stream >> seg1;
		if (seg1._Equal("input")) {
			parseInputs(&stream, &inputs);
		}
		else if (seg1._Equal("output")) {
			parseOutputs(&stream, &outputs);
		}
		else if (seg1._Equal("wire")) {
			parseWires(&stream, &wires);
		}
		else {
			string op, assign, first, second;
			stream >> assign >> first;
			if (stream.eof()) {
				//assignment
			}
			else {
				stream >> op >> second;
				OPS myop = parseOp(op);
				switch (myop) {
				case ADD:
					cout << "ADD adder_ "<< numAdders<<"(" << first << ", " << second << ", " << seg1 << ");" << endl;  //TODO change to verilog file // add width parameter
					numAdders++;
					break;
				case SUB:

					break;
				case MUL:

					break;
				case DIV:

					break;
				case GT:
				case LT:
				case EQ:

					break;
				case RSHIFT:

					break;
				case LSHIFT:

					break;
				case MUX:

					break;
				}
			}
		}
		

	}
	for (int i = 0; i < inputs.size(); i++) {
		cout << inputs.at(i).toString() << endl;
	}
	for (int i = 0; i < outputs.size(); i++) {
		cout << outputs.at(i).toString() << endl;
	}
	for (int i = 0; i < wires.size(); i++) {
		cout << wires.at(i).toString() << endl;
	}

	netlistFile.close();
	verilogFile.close();
    return 0;
}

void parseInputs(stringstream *stream, vector<Input> *inputs){
	string type;
	string name;
	Input* tmp;
	int size;
	bool isSigned;
	*stream >> type;
	if (type.at(0) == 'U') {
		isSigned = false;
		size = stoi(type.substr(4, type.size()-4),nullptr);
	}
	else {
		isSigned = true;
		size = stoi(type.substr(3, type.size()-3),nullptr);
	}
	while (!stream->eof()) {
		name = "";
		*stream >> name;
		if (name.back()==',') {  //check to see if ece3 has c11
			name.erase(name.end()-1);
		}
		tmp = new Input(name, size, isSigned);
		inputs->push_back(*tmp);
	}
}
void parseOutputs(stringstream *stream, vector<Output> *outputs) {
	string type;
	string name;
	Output* tmp;
	int size;
	bool isSigned;
	*stream >> type;
	if (type.at(0) == 'U') {
		isSigned = false;
		size = stoi(type.substr(4, type.size() - 4), nullptr);
	}
	else {
		isSigned = true;
		size = stoi(type.substr(3, type.size() - 3), nullptr);
	}
	while (!stream->eof()) {
		name = "";
		*stream >> name;
		if (name.back() == ',') {  //check to see if ece3 has c11
			name.erase(name.end() - 1);
		}
		tmp = new Output(name, size, isSigned);
		outputs->push_back(*tmp);
	}
}
void parseWires(stringstream *stream, vector<Wire> *wires) {
	string type;
	string name;
	Wire* tmp;
	int size;
	bool isSigned;
	*stream >> type;
	if (type.at(0) == 'U') {
		isSigned = false;
		size = stoi(type.substr(4, type.size() - 4), nullptr);
	}
	else {
		isSigned = true;
		size = stoi(type.substr(3, type.size() - 3), nullptr);
	}
	while (!stream->eof()) {
		name = "";
		*stream >> name;
		if (name.back() == ',') {  //check to see if ece3 has c11
			name.erase(name.end() - 1);
		}
		tmp = new Wire(name, size, isSigned);
		wires->push_back(*tmp);
	}
}
OPS parseOp(string myop) {
	if (myop == "+") {
		return ADD;
	}
	else if (myop == "-") {
		return SUB;
	}
	else if (myop == "*") {
		return MUL;
	}
	else if (myop == "/") {
		return DIV;
	}
	else if (myop == "?") {
		return MUX;
	}
	else if (myop == ">") {
		return GT;
	}
	else if (myop == ">>") {
		return RSHIFT;
	}
	else if (myop == "<") {
		return LT;
	}
	else if (myop == "<<") {
		return LSHIFT;
	}
	else if (myop == "==") {
		return EQ;
	}
	else {
		return BAD;
	}
}