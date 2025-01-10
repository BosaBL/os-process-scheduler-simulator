#include "lib/priorityq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct Process {
  int pid;
  int priority;
  int CPUBurst;
  int arrivalTime;
  int responseTime;   // responseTime = time of 1st exec - arrivalTime
  int turnAroundTime; // turnAroundTime = completionTime - arrivalTime
  int waitingTime;    // waitingTime = turnAroundTime - CPUBurst
  int startTime;      // waitingTime = turnAroundTime - CPUBurst
  int completionTime;
} Process;

typedef struct Algorithm_eval_t {
  int startTime;
  int totalTime;
  int busyTime;
  int idleTime;
  float avgTurnAroundTime;
  float avgResponseTime;
  float avgWaitingTime;
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

float avgTurnAroundTime(Process proc[], int proc_qty) {
  float total = 0;
  for (int i = 0; i < proc_qty; i++) {
    total += proc[i].turnAroundTime;
  }
  return total / proc_qty;
};

float avgResponseTime(Process proc[], int proc_qty) {
  float total = 0;
  for (int i = 0; i < proc_qty; i++) {
    total += proc[i].responseTime;
  }
  return total / proc_qty;
};

float avgWaitingTime(Process proc[], int proc_qty) {
  float total = 0;
  for (int i = 0; i < proc_qty; i++) {
    total += proc[i].waitingTime;
  }
  return total / proc_qty;
};

Algorithm_eval *priority_scheduling(Process proc[], int proc_qty) {
  int time = 0;

  Process *rq[proc_qty];

  Algorithm_eval *algorithm_data = NULL;
  algorithm_data = (Algorithm_eval *)malloc(sizeof(Algorithm_eval));

  algorithm_data->alg = 0;
  algorithm_data->startTime = 0;
  algorithm_data->totalTime = 0;
  algorithm_data->busyTime = 0;
  algorithm_data->idleTime = 0;
  algorithm_data->avgTurnAroundTime = 0;
  algorithm_data->avgResponseTime = 0;
  algorithm_data->avgWaitingTime = 0;
  algorithm_data->processArray = &proc;

  Process *executing = NULL;

  pqueue *pq = pqueue_init(proc_qty, 0, &priority_compare);
  for (int i = 0; i < proc_qty; i++) {
    pqueue_enqueue(pq, &proc[i]);
  }

  // Preparing queue sorted ascendingly by arrivalTime then by priority.
  // Sorting will be done by using heap sort by using pqueue_dequeue from pq.
  for (int i = proc_qty - 1; i >= 0; i--) {
    rq[i] = (Process *)pqueue_dequeue(pq);
  }

  int rq_idx = proc_qty - 1;
  int executing_time;

  // Simulate while there are processes ready and a process is not already being
  // executed.
  while (rq_idx >= 0 || executing != NULL) {
    Process *queue_peek = rq[rq_idx];

    // Check if we can execute the smallest arrival time process.
    if (executing == NULL && queue_peek->arrivalTime <= time) {
      executing_time = 0;
      executing = rq[rq_idx];
      executing->startTime = time;
      executing->responseTime = executing->responseTime < 0
                                    ? time - executing->arrivalTime
                                    : executing->responseTime;

      printf("EXECUTING (pid:%d,arrival:%d,priority:%d,resp:%d,burst:%d)",
             executing->pid, executing->arrivalTime, executing->priority,
             executing->responseTime, executing->CPUBurst);
      fflush(stdout);
    }

    if (executing == NULL) {
      algorithm_data->idleTime++;
    } else {
      algorithm_data->busyTime++;
    }

    time++;
    executing_time++;

    if (executing != NULL && (executing->CPUBurst - executing_time) <= 0) {
      executing->completionTime = time;
      executing->turnAroundTime = time - executing->arrivalTime;
      executing->waitingTime = executing->turnAroundTime - executing->CPUBurst;

      printf("\33[2K\r"); // Delete last line.
      printf("EXECUTED  "
             "(pid:%d,arrival:%d,priority:%d,resp:%d,burst:%d,turnAroundTime:%"
             "d,waitingTime:%d,completionTime:%d,start:%d)\n",
             executing->pid, executing->arrivalTime, executing->priority,
             executing->responseTime, executing->CPUBurst,
             executing->turnAroundTime, executing->waitingTime,
             executing->completionTime, executing->startTime);
      rq_idx--;
      executing = NULL;
    }

    usleep(1000 * 2);
  }

  // Calculate averages and metrics
  algorithm_data->alg = 0;
  algorithm_data->startTime = 0;
  algorithm_data->totalTime = time;
  algorithm_data->avgResponseTime = avgResponseTime(proc, proc_qty);
  algorithm_data->avgTurnAroundTime = avgTurnAroundTime(proc, proc_qty);
  algorithm_data->avgWaitingTime = avgWaitingTime(proc, proc_qty);

  return algorithm_data;
};

int main() {

  // int MAX_P = 5;
  int MAX_P = 30;
  Process a[MAX_P];

  for (int i = 0; i < MAX_P; i++) {
    a[i] = (Process){
        .pid = 0,
        .priority = random() % 1000,
        .arrivalTime = random() % 4,
        .responseTime = -1,
        .CPUBurst = random() % 100,
    };
  }

  // a[0] = (Process){
  //     .pid = 0,
  //     .priority = 1000,
  //     .arrivalTime = 0,
  //     .responseTime = -1,
  //     .CPUBurst = 1,
  // };
  // a[1] = (Process){
  //     .pid = 1,
  //     .priority = 100,
  //     .arrivalTime = 0,
  //     .responseTime = -1,
  //     .CPUBurst = 3,
  // };
  // a[2] = (Process){
  //     .pid = 2,
  //     .priority = 1,
  //     .arrivalTime = 0,
  //     .responseTime = -1,
  //     .CPUBurst = 1,
  // };
  // a[3] = (Process){
  //     .pid = 3,
  //     .priority = 2,
  //     .arrivalTime = 3,
  //     .responseTime = -1,
  //     .CPUBurst = 1,
  // };
  // a[4] = (Process){
  //     .pid = 4,
  //     .priority = 3,
  //     .arrivalTime = 3,
  //     .responseTime = -1,
  //     .CPUBurst = 1,
  // };

  Algorithm_eval *sim = priority_scheduling(a, MAX_P);

  printf("ALGORITHM METRIC: PRIORITY SCHEDULING\n");
  printf("(startTime:%d,totalTime:%d,ideTime:%d,avgResponseTime:%f,"
         "avgTurnAroundTime:%f,avgWaitingTime:%f)",
         sim->startTime, sim->totalTime, sim->idleTime, sim->avgResponseTime,
         sim->avgTurnAroundTime, sim->avgWaitingTime);
}
