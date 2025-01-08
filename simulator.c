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
  Algorithm_eval *algorithm_data = NULL;

  pqueue *pq = pqueue_init(proc_qty, 0, &priority_compare);
  int timer = 0;
  Process *executing = NULL;

  for (int i = 0; i < proc_qty; i++) {
    pqueue_enqueue(pq, &proc[i]);
  }

  for (int i = 0; i < proc_qty; i++) {
    Process *p = pqueue_dequeue(pq);
    printf("(%d,%d)\n", p->arrivalTime, p->priority);
  }

  // while (pq->heap_size > 0) {
  //
  //   if (((Process *)pqueue_peek(pq))->arrivalTime < timer &&
  //       executing == NULL) {
  //     executing = (Process *)pqueue_dequeue(pq);
  //     executing->responseTime = timer;
  //   }
  //
  //   if (executing == NULL) {
  //     algorithm_data->idleTime++;
  //     timer++;
  //     continue;
  //   }
  //
  //   algorithm_data->busyTime++;
  //   timer++;
  // }
  //
  // return algorithm_data;
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
