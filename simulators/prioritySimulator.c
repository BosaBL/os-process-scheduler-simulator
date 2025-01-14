#include "../lib/priorityq.h"
#include "../lib/process.h"
#include "./simulators.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern int verbose, context_cost;

/*
 * Comparator used for priority, first it compares elements by arrival
 * time ascendantly and if two elements have the same arrival time it compares
 * them descendantly . Lower priority number means higher importance.
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

/*
 * Comparator used for priority queue, compares elements by priority.
 * Lower priority number means higher importance.
 */
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
 * Lower priority number means higher importance, thus being executed first.
 *
 * @param preemptive 0 for non-preemptive, 1 for preemptive.
 */
algorithm_eval *priority_scheduling(Process proc[], int proc_qty,
                                    int preemptive) {
  Process *executing = NULL;

  int time = 0;

  char *algname = preemptive == 0 ? "Non-Preemptive Priority Scheduling"
                                  : "Preemptive Priority Scheduling";

  algorithm_eval *algorithm_data = initAlgorithmData(proc, algname);

  // Prepare priority queue.
  pqueue *pq = pqueue_init(proc_qty, 0, &arrival_priority_compare);
  for (int i = 0; i < proc_qty; i++) {
    pqueue_enqueue(pq, &proc[i]);
  }

  pqueue *rq = pqueue_init(proc_qty, 0, &priority_compare);

  if (verbose == 1) {
    printf("\n\nSIMULATING %s\n", algname);
  }

  // Simulate while there are processes ready and a process is not already being
  // executed.
  while (pq->heap_size > 0 || rq->heap_size > 0 || executing != NULL) {

    // Check if a process has become ready to be executed.
    if (pq->heap_size > 0 &&
        ((Process *)pqueue_peek(pq))->arrivalTime <= time) {
      pqueue_enqueue(rq, pqueue_dequeue(pq));
    }

    // Check if we can execute the smallest arrival time process.
    if (executing == NULL) {
      if (rq->heap_size > 0) {
        executing = pqueue_dequeue(rq);
      }
      // If theres a process executing, check if algorithm is in preemptive
      // mode, if it is, check if the current process in the queue has higher
      // priority.
    } else if (rq->heap_size > 0 &&
               ((Process *)pqueue_peek(rq))->priority < executing->priority &&
               preemptive == 1) {

      pqueue_enqueue(rq, executing);
      executing = pqueue_dequeue(rq);
    }

    // Set starting metrics for the process.
    if (executing != NULL) {
      executing->startTime =
          executing->startTime < 0 ? time : executing->startTime;

      executing->responseTime = executing->responseTime < 0
                                    ? time - executing->arrivalTime
                                    : executing->responseTime;
    };

    time += 1;

    // Check if process' execution has been completed.
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

        if (pq->heap_size > 0 || rq->heap_size > 0) {
          time += context_cost;
        }
        executing = NULL;
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

  deallocate_pqueue(pq);

  return algorithm_data;
};
