#ifndef ALGORITHM_EVAL_H
#include "process.h"

typedef struct algorithm_eval {
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

/*
 * Dinamically allocates algortihm_eval struct.
 */
algorithm_eval *initAlgorithmData(Process proc[], char *aname);

/*
 * Prints algorthm data.
 */
void printAlgorithmData(algorithm_eval *data);

#endif // !ALGORITHM_EVAL_H
