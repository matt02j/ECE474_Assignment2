// 474asignment2.cpp : Defines the entry point for the console application.
//


//TODO add implicit register for each output, 
//change out from cout to file,
//add module header
// maybe declare extra wires as needed
//check error conditions and verify error message is printed
//test output with vivado




#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "types.h"
#include "Scheduler.h"

using namespace std;

void parseInputs(stringstream *stream, vector<Input>* inputs);
void parseOutputs(stringstream *stream, vector<Output>* Outputs);
void parseWires(stringstream *stream, vector<Wire>* wires);
OPS parseOp(string myop);
void writeInputInits(vector<Input> inputs, shared_ptr<ofstream> verilogFile);
void writeOutputInits(vector<Output> outputs, shared_ptr<ofstream> verilogFile);
void writeWireInits(vector<Wire> wires, shared_ptr<ofstream> verilogFile);
void writeModuleHeading(vector<Input> inputs, vector<Output> outputs, shared_ptr<ofstream> verilogFile, string moduleName);
void writeModuleClosing(shared_ptr<ofstream> verilogFile);
string checkValid(string varName, vector<Input> inputs, vector<Output> outputs, vector<Wire> wires);
Variable findByName(string name, vector<Input> *in, vector<Output> *out, vector<Wire>* wire);

int main(int argc, char* argv[]){
	 string netlistFileName, verilogFileName;
	 ifstream netlistFile;
	 auto verilogFile = make_shared<ofstream>();
	 string buffer,seg1,seg2,seg3;
	 stringstream stream;
	 vector<Input> inputs;
	 vector<Output> outputs;
	 vector<Wire> wires;
	 vector<Operation> operations;
	 string moduleName = "outputModule";

	 int numAdders=0;
	 int numComps=0;
	 int numSubs=0;
	 int numMuls = 0;
	 int numMuxs=0;
	 int numRShifts=0;
	 int numLShifts=0;
	 int numRegs = 0;

	if (argc != 3) {
		cout << "Usage: dpgen netlistFile verilogFile"<<endl;
		return 1;
	}
	netlistFileName = argv[1];
	verilogFileName = argv[2];


	netlistFile.open(netlistFileName);
	verilogFile->open(verilogFileName);

	// make input/output/wire classes?
	//convert to cdfg

	bool finishedInits = false;
	while (!(netlistFile.eof())) {
		seg1.clear();
		buffer.clear();
		stream.clear();
		getline(netlistFile, buffer);
		stream << buffer;
		stream >> seg1;
		if (seg1 == "//" || seg1 == "") { //comment
			continue;
		}
		if (seg1 == "input") { //input
			parseInputs(&stream, &inputs);
		}
		else if (seg1 == "output") { //output declaration
			parseOutputs(&stream, &outputs);
		}
		else if (seg1 == "wire" || seg1 == "register") { //wire declaration
			parseWires(&stream, &wires);
		}
		else { //assignment
			if (!finishedInits) {
				writeModuleHeading(inputs, outputs, verilogFile, moduleName);
				writeInputInits(inputs, verilogFile);
				writeOutputInits(outputs, verilogFile);
				writeWireInits(wires, verilogFile);
				finishedInits = true;
			}

			string op, assign, first, second;
			stream >> assign >> first;

			//OPS myop;
			/*if (stream.eof()) {
				myop = REG;
			}*/
			stream >> op >> second;
			OPS myop = parseOp(op);
			int size=0;
			bool isSigned=false;
			for (unsigned int i = 0; i < outputs.size(); i++) {
				if (outputs.at(i).name == seg1) {
					size = outputs.at(i).size;
					isSigned = outputs.at(i).isSigned;
					break;
				}
			}
			if (size == 0) {
				for (unsigned int i = 0; i < wires.size(); i++) {
					if (wires.at(i).name == seg1) {
						size = wires.at(i).size;
						isSigned = wires.at(i).isSigned;
						break;
					}
				}
			}

			/*
			if (size == 0) {
				cerr << "Unable to find output " << seg1 << endl;
				return 1;
			}
			*/

			if (myop != BAD) {
				string validStr = "good";
				validStr = checkValid(first, inputs, outputs, wires);
				if (validStr != "good") {
					cout << validStr;
					netlistFile.close();
					verilogFile->close();
					return 0;
				}
				validStr = checkValid(seg1, inputs, outputs, wires);
				if (validStr != "good") {
					cout << validStr;
					netlistFile.close();
					verilogFile->close();
					return 0;
				}
				if (myop != REG) {
					validStr = checkValid(second, inputs, outputs, wires);
					if (validStr != "good") {
						cout << validStr;
						netlistFile.close();
						verilogFile->close();
						return 0;
					}
				}
			}
			//cover to Operation objects for the graph
			Variable in1, in2, in3, out;
			in1 = findByName(first,&inputs,&outputs,&wires);
			in2 = findByName(second, &inputs, &outputs, &wires);
			out = findByName(seg1, &inputs, &outputs, &wires);
			if (myop == MUX) {
				string colon, third;
				stream >> colon >> third;
				in3 = findByName(third, &inputs, &outputs, &wires);
				operations.push_back(*(new Operation(in1,in2,in3,out)));
			}
			else {

				operations.push_back(*(new Operation(in1, in2, out)));
			}
/*
			*verilogFile << "\t";
			if (isSigned) {
				*verilogFile << "S";
			}
			switch (myop) {
				case ADD: {
					*verilogFile << "ADD #(" << size <<") adder_"<< numAdders<<"(" << first << ", " << second << ", " << seg1 << ");" << endl;  //TODO change to verilog file
					numAdders++;
					break;
				}
				case SUB: {
					*verilogFile << "SUB #(" << size << ") sub_" << numSubs << "(" << first << ", " << second << ", " << seg1 << ");" << endl;  //TODO change to verilog file
					numSubs++;
					break;
				}
				case MUL: {
					*verilogFile << "MUL #(" << size << ") mul_" << numAdders << "(" << first << ", " << second << ", " << seg1 << ");" << endl;  //TODO change to verilog file
					numMuls++;
					break;
				}
				case DIV: {
					//turns out we dont need this for 474
					break;
				}
				case GT: {
					*verilogFile << "COMP #(" << size << ") comp_" << numComps << "(" << first << ", " << second << ", , ," << seg1 << ");" << endl;  //is this legal? probably not
					numComps++;
					break;
				}
				case LT: {
					*verilogFile << "COMP #(" << size << ") comp_" << numComps << "(" << first << ", " << second << "," << seg1 << ", , );" << endl;  //is this legal? probably not
					numComps++;
					break;
				}
				case EQ: {
					*verilogFile << "COMP #(" << size << ") comp_" << numComps << "(" << first << ", " << second << ", ," << seg1 << ", );" << endl;  //is this legal? probably not
					numComps++;
					break;
				}
				case RSHIFT: {
					*verilogFile << "SHR #(" << size << ") shr_" << numRShifts << "(" << first << ", " << second << ", " << seg1 << ");" << endl;  //TODO change to verilog file
					numRShifts++;
					break;
				}
				case LSHIFT: {
					*verilogFile << "SHL #(" << size << ") mul_" << numLShifts << "(" << first << ", " << second << ", " << seg1 << ");" << endl;  //TODO change to verilog file
					numLShifts++;
					break;
				}
				case MUX: {
					string colon, third;
					stream >> colon >> third;
					string validStr = checkValid(third, inputs, outputs, wires);
					if (validStr != "good") {
						cout << validStr;
						netlistFile.close();
						verilogFile->close();
						return 0;
					}
					*verilogFile << "MUX #(" << size << ") mux_" << numMuxs << "(" << first << ", " << second << ", " << third << ", "<< seg1 << ");" << endl;  //TODO change to verilog file
					numMuxs++;
					break;
				}
				case REG: {
					*verilogFile << "REG #(" << size << ") reg_" << numRegs << "(" << "Clk, Rst, " << first << ", " << seg1 << ");" << endl;
					numRegs++;
					break;
				}
				case BAD: {
					cout << "ERROR: invalid operator" << endl;
					netlistFile.close();
					verilogFile->close();
					return 0;
				}
			}*/
		}
	}
	
	writeModuleClosing(verilogFile);

	/*
	for (unsigned int i = 0; i < inputs.size(); i++) {
		cout << inputs.at(i).toString() << endl;
	}
	for (unsigned int i = 0; i < outputs.size(); i++) {
		cout << outputs.at(i).toString() << endl;
	}
	for (unsigned int i = 0; i < wires.size(); i++) {
		cout << wires.at(i).toString() << endl;
	}
	*/

	netlistFile.close();
	verilogFile->close();
    return 0;
}
Variable findByName(string name, vector<Input> *in, vector<Output> *out, vector<Wire>* wire) {
	//TODO return the apropriate wire/input/output
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
	else if (myop == "") {
		return REG;
	}
	else {
		return BAD;
	}
}

void writeInputInits(vector<Input> inputs, shared_ptr<ofstream> verilogFile) {
	*verilogFile << "\t" << "input Clk, Rst;" << endl;
	for (unsigned int i = 0; i < inputs.size(); i++) {
		if (inputs[i].name.size() > 0) {
			*verilogFile << "\t" << "input [" << inputs[i].size - 1 << ":0] " << inputs[i].name << ";" << endl;
		}
	}
}

void writeOutputInits(vector<Output> outputs, shared_ptr<ofstream> verilogFile) {
	for (unsigned int i = 0; i < outputs.size(); i++) {
		if (outputs[i].name.size() > 0) {
			*verilogFile << "\t" << "output [" << outputs[i].size - 1 << ":0] " << outputs[i].name << ";" << endl;
		}
	}
}

void writeWireInits(vector<Wire> wires, shared_ptr<ofstream> verilogFile) {
	for (unsigned int i = 0; i < wires.size(); i++) {
		if (wires[i].name.size() > 0) {
			*verilogFile << "\t" << "wire [" << wires[i].size - 1 << ":0] " << wires[i].name << ";" << endl;
		}
	}
}

void writeModuleHeading(vector<Input> inputs, vector<Output> outputs, shared_ptr<ofstream> verilogFile, string moduleName) {
	*verilogFile << "module " << moduleName << "(";
	for (unsigned int i = 0; i < inputs.size(); i++) {
		if (inputs[i].name.size() > 0) {
			*verilogFile << inputs[i].name;
			if (i < inputs.size() - 1 || outputs.size() > 0) {
				*verilogFile << ", ";
			}
		}
	}
	for (unsigned int i = 0; i < outputs.size(); i++) {
		if (outputs[i].name.size() > 0) {
			*verilogFile << outputs[i].name;
			if (i < outputs.size() - 1) {
				*verilogFile << ", ";
			}
		}
	}

	*verilogFile << ");" << endl;
}

void writeModuleClosing(shared_ptr<ofstream> verilogFile) {
	*verilogFile << "endmodule" << endl;
}

string checkValid(string varName, vector<Input> inputs, vector<Output> outputs, vector<Wire> wires) {
	bool found = false;
	for (unsigned int i = 0; i < inputs.size(); i++) {
		if (inputs[i].name == varName) {
			found = true;
		}
	}
	for (unsigned int i = 0; i < outputs.size(); i++) {
		if (outputs[i].name == varName) {
			found = true;
		}
	}
	for (unsigned int i = 0; i < wires.size(); i++) {
		if (wires[i].name == varName) {
			found = true;
		}
	}
	if (!found) {
		return "missing variable";
	}
	else {
		return "good";
	}
}
