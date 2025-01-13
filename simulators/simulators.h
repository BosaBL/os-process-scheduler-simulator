#ifndef SIMULATORS_H
#include "../lib/algorithm_eval.h"
#include "../lib/process.h"

algorithm_eval *priority_scheduling(Process proc[], int proc_qty,
                                    int preemptive);

algorithm_eval *round_robin_scheduling(Process proc[], int proc_qty,
                                       int quantum);
#endif // !SIMULATORS_H
