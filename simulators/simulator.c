#include "./simulator.h"
#include "../lib/priorityq.h"
#include "../lib/process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
 * Dinamically allocates algortihm_eval struct.
 */
static algorithm_eval *initAlgortihmData(Process proc[]) {

  algorithm_eval *algorithm_data = malloc(sizeof(algorithm_eval));

  algorithm_data->alg = 0;
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

/*
 * Priority compare used for priority scheduler simulator.
 * Compares by arrival time, then if there are two proccesses that arrived a the
 * same time, it compares by priority.
 */
static int arrival_priority_compare(void *a, void *b) {
  Process *p1 = a;
  Process *p2 = b;

  if (p1->arrivalTime < p2->arrivalTime) {
    return PQUEUE_GREATER;
  }

  if (p2->arrivalTime < p1->arrivalTime) {
    return PQUEUE_LESS;
  }

  if (p1->priority > p2->priority) {
    return PQUEUE_LESS;
  }

  if (p2->priority > p1->priority) {
    return PQUEUE_GREATER;
  }

  return PQUEUE_EQUAL;
}

static int priority_compare(void *a, void *b) {
  Process *p1 = a;
  Process *p2 = b;

  if (p1->priority > p2->priority) {
    return PQUEUE_LESS;
  }

  if (p2->priority > p1->priority) {
    return PQUEUE_GREATER;
  }

  return PQUEUE_EQUAL;
}

/*
 * Simulates the scheduling and execution of processes using non-preemptive or
 * preemptive priority scheduling.
 *
 * @param preemptive 0 for non-preemptive, 1 for preemptive.
 */
algorithm_eval *priority_scheduling(Process proc[], int proc_qty,
                                    int preemptive) {
  Process *executing = NULL;

  int time = 0;

  algorithm_eval *algorithm_data = initAlgortihmData(proc);

  // Prepare priority queue.
  pqueue *pq = pqueue_init(proc_qty, 0, &arrival_priority_compare);
  for (int i = 0; i < proc_qty; i++) {
    pqueue_enqueue(pq, &proc[i]);
  }

  pqueue *rq = pqueue_init(proc_qty, 0, &priority_compare);

  // Simulate while there are processes ready and a process is not already being
  // executed.
  while (pq->heap_size > 0 || rq->heap_size > 0 || executing != NULL) {
    if (pq->heap_size > 0 &&
        ((Process *)pqueue_peek(pq))->arrivalTime <= time) {
      pqueue_enqueue(rq, pqueue_dequeue(pq));
    }

    // Check if we can execute the smallest arrival time process.
    if (executing == NULL) {

      if (rq->heap_size > 0) {
        executing = pqueue_dequeue(rq);

        char *details = startDetails(executing);
        printf("EXECUTING %s", details);
        free(details);
        fflush(stdout);
      }
    } else if (rq->heap_size > 0 &&
               ((Process *)pqueue_peek(rq))->priority < executing->priority &&
               preemptive == 1) {

      pqueue_enqueue(rq, executing);
      executing = pqueue_dequeue(rq);

      char *details = startDetails(executing);
      printf("EXECUTING %s", details);
      free(details);
      fflush(stdout);
    }

    if (executing != NULL) {
      executing->startTime =
          executing->startTime < 0 ? time : executing->startTime;

      executing->responseTime = executing->responseTime < 0
                                    ? time - executing->arrivalTime
                                    : executing->responseTime;
    };

    time += 1;

    if (executing != NULL) {
      executing->internalExecutionTime++;

      if (executing->CPUBurst - executing->internalExecutionTime <= 0) {
        executing->completionTime = time;
        executing->turnAroundTime = time - executing->arrivalTime;
        executing->waitingTime =
            executing->turnAroundTime - executing->CPUBurst;

        // Print details.
        char *details = endDetails(executing);
        printf("\33[2K\r"); // Delete last line.
        printf("EXECUTED  %s\n", details);
        free(details);
        executing = NULL;
      }
    } else {
      algorithm_data->idleTime++;
    }

    // Sleep for visualization purpososes.
    usleep(1000 * 1500);
  }

  // Calculate averages and metrics.
  algorithm_data->alg = 0;
  algorithm_data->startTime = 0;
  algorithm_data->totalTime = time;
  algorithm_data->avgResponseTime = avgResponseTime(proc, proc_qty);
  algorithm_data->avgTurnAroundTime = avgTurnAroundTime(proc, proc_qty);
  algorithm_data->avgWaitingTime = avgWaitingTime(proc, proc_qty);

  deallocate_pqueue(pq);

  return algorithm_data;
};
