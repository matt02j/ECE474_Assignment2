#include "Scheduler.h"
#include "types.h"
#include <vector>

void schedule(vector<Operation> *ops,int latency) {
	//build dependency
	//asap
	//alap
	//listr
	buildDependencyGraph(ops);
	asap(ops, latency);
	alap(ops, latency);

	
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