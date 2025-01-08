#include "lib/priorityq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  int time = 0;

  Algorithm_eval *algorithm_data = NULL;
  Process *executing = NULL;

  pqueue *pq = pqueue_init(proc_qty, 0, &priority_compare);

  for (int i = 0; i < proc_qty; i++) {
    pqueue_enqueue(pq, &proc[i]);
  }

  while (pq->heap_size > 0) {
    if (executing == NULL &&
        ((Process *)pqueue_peek(pq))->arrivalTime <= time) {
      executing = pqueue_dequeue(pq);
      executing->responseTime =
          executing->responseTime < 0 ? time : executing->responseTime;
    }

    time++;
  }

  return algorithm_data;
};

int main() {

  int MAX_P = 100;

  Process a[MAX_P];

  for (int i = 0; i < MAX_P; i++) {
    int rand1 = random() % 5;
    int rand2 = random() % 100;

    a[i] = (Process){i, rand2, rand1, 10, 1};
  }

  priority_scheduling(a, MAX_P);
}
