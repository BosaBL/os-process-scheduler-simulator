#include "../lib/priorityq.h"
#include "../lib/process.h"
#include "../lib/queue.h"
#include "./simulators.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern int verbose, context_cost;

/*
 * Comparator used to sort the process queque by arrivaTime, essentially,
 * making the soonest to execute process be the head.
 */
static int arrival_compare(void *a, void *b) {
  Process *p1 = a;
  Process *p2 = b;

  if (p1->arrivalTime < p2->arrivalTime) {
    return PQUEUE_GREATER;
  }

  if (p2->arrivalTime < p1->arrivalTime) {
    return PQUEUE_LESS;
  }

  if (p1->pid < p2->pid) {
    return PQUEUE_GREATER;
  }

  if (p2->pid < p1->pid) {
    return PQUEUE_LESS;
  }

  return PQUEUE_EQUAL;
}

/*
 * Simulates the scheduling and execution of processes using round robing
 * scheduling algorithm.
 */
algorithm_eval *round_robin_scheduling(Process proc[], int proc_qty,
                                       int quantum) {
  Process *executing = NULL;

  int time = 0;

  char *algname = "Round Robin";

  algorithm_eval *algorithm_data = initAlgorithmData(proc, algname);

  // Prepare priority queue.
  pqueue *pq = pqueue_init(proc_qty, 0, &arrival_compare);
  for (int i = 0; i < proc_qty; i++) {
    pqueue_enqueue(pq, &proc[i]);
  }

  qqueue *rq = qqueue_init(proc_qty);

  if (verbose == 1) {
    printf("\n\nSIMULATING %s\n", algname);
  }

  int used_quantum = 0;

  // Simulate while there are processes ready and a process is not already being
  // executed.
  while (pq->heap_size > 0 || !isEmpty(rq) || executing != NULL) {

    // Check if a process has become ready to be executed.
    if (pq->heap_size > 0 &&
        ((Process *)pqueue_peek(pq))->arrivalTime <= time) {
      qqueue_enqueue(rq, pqueue_dequeue(pq));
    }

    // Check if the current running process has exceeded the quantum time.
    if (executing != NULL && used_quantum >= quantum && !isEmpty(rq)) {
      qqueue_enqueue(rq, executing);
      time += context_cost;
      executing = NULL;
    }

    // Check if we can execute the smallest arrival time process.
    if (executing == NULL) {
      if (!isEmpty(rq)) {
        used_quantum = 0;
        executing = qqueue_dequeue(rq);
      }
    }

    // Intialize starting metrics for process.
    if (executing != NULL) {
      executing->startTime =
          executing->startTime < 0 ? time : executing->startTime;

      executing->responseTime = executing->responseTime < 0
                                    ? time - executing->arrivalTime
                                    : executing->responseTime;
    };

    time += 1;
    used_quantum += 1;

    // Check if current process has ended it's execution.
    if (executing != NULL) {
      executing->internalExecutionTime++;

      if (executing->CPUBurst - executing->internalExecutionTime <= 0) {
        executing->completionTime = time;
        executing->turnAroundTime = time - executing->arrivalTime;
        executing->waitingTime =
            executing->turnAroundTime - executing->CPUBurst;

        // Print details.
        if (verbose == 1) {
          char *details = endDetails(executing);
          printf("EXECUTED  %s\n", details);
          free(details);
        }

        if (pq->heap_size > 0 || !isEmpty(rq)) {
          time += context_cost;
        }

        executing = NULL;
        used_quantum = 0;
      }
    } else {
      algorithm_data->idleTime++;
    }

    // Sleep for visualization purpososes.
    // usleep(1000 * 1500);
  }

  // Calculate averages and metrics.
  algorithm_data->startTime = 0;
  algorithm_data->totalTime = time;
  algorithm_data->avgResponseTime = avgResponseTime(proc, proc_qty);
  algorithm_data->avgTurnAroundTime = avgTurnAroundTime(proc, proc_qty);
  algorithm_data->avgWaitingTime = avgWaitingTime(proc, proc_qty);

  return algorithm_data;
};
