#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "types.h"

void schedule(vector<Operation> *ops,int latency, int resources[3]);
void buildDependencyGraph(vector<Operation>* ops);
void asap(vector<Operation>* ops, int latency);
void alap(vector<Operation>* ops, int latency);
int asap1(Operation *op, int cycle,int latency);
int alap1(Operation *op, int cycle);
void listr(vector<Operation>* ops, int latency, int resources[3]);
#endif