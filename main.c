#include "lib/process.h"
#include "simulators/simulator.h"
#include <stdio.h>

int main() {

  int MAX_P = 5;
  Process a[MAX_P] = {};

  // for (int i = 0; i < MAX_P; i++) {
  //   a[i] = (Process){
  //       .pid = 0,
  //       .priority = random() % 1000,
  //       .arrivalTime = random() % 4,
  //       .responseTime = -1,
  //       .CPUBurst = random() % 100,
  //   };
  // }

  initProcess(&a[0], 0, 100, 1, 0);

  initProcess(&a[1], 1, 50, 3, 1);

  initProcess(&a[2], 2, 0, 1, 2);

  initProcess(&a[3], 3, 105, 1, 3);

  initProcess(&a[4], 4, 102, 1, 3);

  algorithm_eval *sim = priority_scheduling(a, 5, 0);

  printf("ALGORITHM METRIC: PRIORITY SCHEDULING\n");
  printf("%s (startTime:%d,totalTime:%d,ideTime:%d,avgResponseTime:%f,"
         "avgTurnAroundTime:%f,avgWaitingTime:%f)",
         sim->alg, sim->startTime, sim->totalTime, sim->idleTime,
         sim->avgResponseTime, sim->avgTurnAroundTime, sim->avgWaitingTime);
}
