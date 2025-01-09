#include "lib/priorityq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct Process {
  int pid;
  int priority;
  int arrivalTime;
  int responseTime;
  int CPUBurst;
  int turnAroundTime;
} Process;

typedef struct Algorithm_eval_t {
  int startTime;
  int totalTime;
  int busyTime;
  int idleTime;
  int avgTurnAroundTime;
  int avgResponseTime;
  Process **processArray;
  int alg;
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
  algorithm_data = (Algorithm_eval *)malloc(sizeof(Algorithm_eval));

  algorithm_data->alg = 1;
  algorithm_data->startTime = 0;
  algorithm_data->totalTime = 0;
  algorithm_data->busyTime = 0;
  algorithm_data->idleTime = 0;
  algorithm_data->avgTurnAroundTime = 0;
  algorithm_data->avgResponseTime = 0;
  algorithm_data->processArray = &proc;

  Process *executing = NULL;

  pqueue *pq = pqueue_init(proc_qty, 0, &priority_compare);

  for (int i = 0; i < proc_qty; i++) {
    pqueue_enqueue(pq, &proc[i]);
  }

  while (pq->heap_size > 0 || executing != NULL) {
    printf("Time: %d\n", time);
    Process *queue_peek = (Process *)pqueue_peek(pq);

    if (executing == NULL && queue_peek->arrivalTime <= time) {

      executing = pqueue_dequeue(pq);
      executing->responseTime =
          executing->responseTime < 0 ? time : executing->responseTime;

      printf("DEQUEUED: (pid:%d,arrival:%d,priority:%d,resp:%d,burst:%d)\n",
             executing->pid, executing->arrivalTime, executing->priority,
             executing->responseTime, executing->CPUBurst);
    }

    if (executing == NULL && queue_peek->arrivalTime > time) {
      algorithm_data->idleTime += 1;
    }

    if (executing == NULL) {
      algorithm_data->idleTime++;
    }

    if (executing != NULL &&
        (executing->CPUBurst + executing->responseTime - time) <= 0) {
      executing->turnAroundTime = time - executing->arrivalTime;
      printf("End exec %d\n", executing->pid);
      executing = NULL;
    }

    time++;
    sleep(1);
  }

  return algorithm_data;
};

int main() {

  int MAX_P = 5;

  Process a[MAX_P];

  a[0] = (Process){0, 1000, 0, -1, 1};
  a[1] = (Process){1, 100, 0, -1, 1};
  a[2] = (Process){2, 1, 0, -1, 1};
  a[3] = (Process){3, 2, 3, -1, 1};
  a[4] = (Process){4, 3, 3, -1, 1};

  priority_scheduling(a, MAX_P);
}
