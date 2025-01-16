#include "algorithm_eval.h"
#include <stdio.h>
#include <stdlib.h>

algorithm_eval *initAlgorithmData(Process proc[], char *aname) {

  algorithm_eval *algorithm_data = malloc(sizeof(algorithm_eval));

  algorithm_data->alg = aname;
  algorithm_data->startTime = 0;
  algorithm_data->totalTime = 0;
  algorithm_data->busyTime = 0;
  algorithm_data->idleTime = 0;
  algorithm_data->avgTurnAroundTime = 0;
  algorithm_data->avgResponseTime = 0;
  algorithm_data->avgWaitingTime = 0;
  algorithm_data->processArray = &proc;

  return algorithm_data;
}

void printAlgorithmData(algorithm_eval *data) {
  printf("\n\nAlgortihm Metric: %s\n", data->alg);
  printf("%s (startTime:%d,totalTime:%d,ideTime:%d,avgResponseTime:%f,"
         "avgTurnAroundTime:%f,avgWaitingTime:%f)\n",
         data->alg, data->startTime, data->totalTime, data->idleTime,
         data->avgResponseTime, data->avgTurnAroundTime, data->avgWaitingTime);
}
