#include "Scheduler.h"
#include "types.h"
#include <vector>

void schedule(vector<Operation> *ops,int latency, int resources[3]) {
	//build dependency
	//asap
	//alap
	//listr
	buildDependencyGraph(ops);
	asap(ops, latency);
	alap(ops, latency);
	listr(ops, latency, resources);
}
void listr(vector<Operation>* ops, int latency, int resources[3]) {
	bool good = true;
	vector<Operation*> candidates,add,mul,logic;
	vector<Operation*> *x;
	resources[0] = 1; resources[1] = 1; resources[2] = 1;
	int inUse[3] = { 0,0,0 };
	int curr = 1,scheduled=0;
	for (int i = 0; i < ops->size(); i++) {
		switch (ops->at(i).op) {
		case ADD:
		case SUB:
			add.push_back(&(ops->at(i)));
			break;
		case MUL:
			mul.push_back(&(ops->at(i)));
			break;
		default:
			logic.push_back(&(ops->at(i)));
			break;
		}
	}
	while (scheduled < ops->size()) { //figure out termination condition
		for (int r = 0; r < 3; r++) { //r is resource, 0 is ADD/SUB, 1 is MUL, 2 is everything else/LOGIC
			candidates.clear();
			inUse[r] = 0;
			//determin candidates
			switch (r) {
			case 0:
				x = &add;
				break;
			case 1:
				x = &mul;
				break;
			case 2:
				x = &logic;
				break;
			}
			for (int i = 0; i < x->size(); i++) {
				good = true;
				for (int j = 0; j < x->at(i)->iDependOn.size(); j++) {
					if (x->at(i)->iDependOn.at(j)->listr == -1 || x->at(i)->iDependOn.at(j)->listrEnd >= curr) {
						good = false;
						break;
					}
				}
				if (good && x->at(i)->listr == -1) {
					candidates.push_back(x->at(i));
				}
			}
			
			//for each candidate
				//compute slack (alap - current)
				//schedule those with no slack, update num resources if necesary
			for (int i = 0; i < candidates.size(); i++) {
				int slack = candidates.at(i)->alap - curr;
				if (slack == 0) {
					candidates.at(i)->listr = curr;
					if (r == 1) {
						candidates.at(i)->listrEnd = curr+1;
					}
					else {
						candidates.at(i)->listrEnd = curr;
					}
					candidates.erase(candidates.begin() + i);
					scheduled++;
					inUse[r]++;
					if (inUse[r] > resources[r]) {
						resources[r]++;
					}
				}
			}
			//fill empty resources is possible
			for (int i = 0; i < resources[r] - inUse[r]; i++) {
				if (candidates.size() == 0) {
					break;
				}
				else {
					candidates.at(i)->listr = curr;
					if (r == 1) {
						candidates.at(i)->listrEnd = curr + 1;
					}
					else {
						candidates.at(i)->listrEnd = curr;
					}
					candidates.erase(candidates.begin() + i);
					scheduled++;
				}
			}
		}
		curr++;
	}
}
void asap(vector<Operation> *ops, int latency) {
	//for each node
		//if no dependencies
			//asap = 0
			//recursive add 1 to each layer of a child
	for (int i = 0; i < ops->size(); i++) {
		if ((int)ops->at(i).iDependOn.size() == 0) {
			ops->at(i).asap = 0;
			asap1(&(ops->at(i)), 1,latency); //check return
		}
	}
}
int asap1(Operation *op, int cycle, int latency) {
	if (cycle > latency) {
		//ERROR
		return 1;
	}
	if (op->asap < cycle) {
		op->asap = cycle;
		for (int i = 0; i < op->dependOnMe.size(); i++) {
			asap1(op->dependOnMe.at(i), cycle + 1,latency);
		}
	}
	return 0;
}
void alap(vector<Operation>* ops, int latency) {
	for (int i = 0; i < ops->size(); i++) {
		if ((int)ops->at(i).dependOnMe.size() == 0) {
			//ops->at(i).alap = latency;
			alap1(&(ops->at(i)), latency); //check return
		}
	}
}
int alap1(Operation *op, int cycle) {
	if (cycle < 0) {
		//ERROR
		return 1;
	}
	if (op->alap > cycle || op->alap==-1) {
		op->alap = cycle;
		for (int i = 0; i < op->iDependOn.size(); i++) {
			alap1(op->iDependOn.at(i), cycle - 1);
		}
	}
	return 0;
}
void buildDependencyGraph(vector<Operation> *ops) {
	//for each op - o, look at inputs
	//for each input - i look at all other ops after 'o'
	//for each op - p look at all outputs
	//if output is input
	//add to dependency/depend on list

	for(int o = 0; o < ops->size(); o++) {
		for (int p = o + 1; p < ops->size(); p++) {
			if (ops->at(p).out == ops->at(o).in1 || ops->at(p).out == ops->at(o).in2 || ops->at(p).out == ops->at(o).in3) {
				ops->at(o).iDependOn.push_back(&ops->at(p));
				ops->at(p).dependOnMe.push_back(&ops->at(o));
			}
			if (ops->at(o).out == ops->at(p).in1 || ops->at(o).out == ops->at(p).in2 || ops->at(o).out == ops->at(p).in3) {
				ops->at(p).iDependOn.push_back(&ops->at(o));
				ops->at(o).dependOnMe.push_back(&ops->at(p));
			}
		}
	}
}