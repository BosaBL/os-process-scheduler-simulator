#include "lib/priorityq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process {
  int pid;
  int priority;
  int arrivalTime;
  int responseTime;
  int CPUBurst;
} Process;

typedef struct Algorithm_eval_t {
  int startTime;
  int totalTime;
  int busyTime;
  int idleTime;
  int avgWaitingTime;
  int avgTurnAroundTime;
  int avgResponseTime;
  Process **processArray;
  char alg[];
} Algorithm_eval;

int priority_compare(void *a, void *b) {
  Process *p1 = a;
  Process *p2 = b;

  if (p1->arrivalTime < p2->arrivalTime) {
    return PQUEUE_GREATER;
  }

  if (p2->arrivalTime < p1->arrivalTime) {
    return PQUEUE_LESS;
  }

  if (p1->priority < p2->priority) {
    return PQUEUE_LESS;
  }

  if (p2->priority < p1->priority) {
    return PQUEUE_GREATER;
  }

  return PQUEUE_EQUAL;
}

Algorithm_eval *priority_scheduling(Process proc[], int proc_qty) {
  Algorithm_eval *algorithm_data = NULL;

  pqueue *pq = pqueue_init(proc_qty, 0, &priority_compare);
  int timer = 0;
  int execution_time;

  for (int i = 0; i < proc_qty; i++) {
    pqueue_enqueue(pq, &proc[i]);
  }

  while (pq->heap_size > 0) {
    execution_time = 0;

    if (((Process *)pqueue_peek(pq))->arrivalTime > timer) {
      timer++;
      continue;
    }

    timer++;
    execution_time++;
  }

  return algorithm_data;
};

int main() {

  Process p3 = {3, 3, 2, 10, 1};

  Process p1 = {1, 10, 2, -1, 1};

  Process p2 = {2, 90, 2, 1, 1};

  Process p4 = {4, 1, 2, 10, 1};

  Process p5 = {5, 4, 2, 10, 1};

  Process p6 = {6, 10000, 2, 10, 1};
  Process a[6] = {
      p3, p1, p4, p6, p2, p5,
  };

  priority_scheduling(a, 6);
}
