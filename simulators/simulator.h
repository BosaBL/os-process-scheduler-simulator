#ifndef SIMULATOR_H
#include "../lib/process.h"

typedef struct algorithm_eval_t {
  int startTime;
  int totalTime;
  int busyTime;
  int idleTime;
  float avgTurnAroundTime;
  float avgResponseTime;
  float avgWaitingTime;
  Process **processArray;
  char *alg;
} algorithm_eval;

algorithm_eval *priority_scheduling(Process proc[], int proc_qty);

algorithm_eval *round_robin_scheduling(Process proc[], int proc_qty,
                                       int quantum);
#endif // !SIMULATOR_H
