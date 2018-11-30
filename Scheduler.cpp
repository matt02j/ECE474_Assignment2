#include "Scheduler.h"
#include "types.h"
#include <vector>

void schedule(vector<Operation> *ops,int latency) {
	//build dependency
	//asap
	//alap
	//listr

}


void buildDependencyGraph(vector<Operation> *ops) {
	//for each op - o, look at inputs
	//for each input - i look at all other ops after 'o'
	//for each op - p look at all outputs
	//if output is input
	//add to dependency/depend on list

	for(int o = 0; o < ops->size; o++) {
		for (int p = o + 1; p < ops->size; p++) {
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