#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "types.h"

void schedule(vector<Operation> *ops,int latency);
void buildDependencyGraph(vector<Operation>* ops);


#endif