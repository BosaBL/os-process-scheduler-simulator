#include "process.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Initializes a process with the required starting data.
 *
 * @param priority will only be used on priority based simulations.
 */
void initProcess(Process *p, int pid, int priority, int CPUBurst,
                 int arrivalTime) {
  p->pid = pid;
  p->priority = priority;
  p->CPUBurst = CPUBurst;
  p->arrivalTime = arrivalTime;
  p->responseTime = -1;
  p->turnAroundTime = 0;
  p->waitingTime = 0;
  p->startTime = -1;
  p->completionTime = 0;
  p->internalExecutionTime = 0;
}
/*
 * Dynamically allocates a string that contains all the starting details of the
 * process.
 * Usually the starting details are the parameters of initProcess function.
 *
 * @return starting details of the process.
 */
char *startDetails(Process *p) {
  char *str;
  if (asprintf(
          &str, "P%d (PID: %d, priority: %d, CPUBurst: %d, arrivalTime: %d)",
          p->pid, p->pid, p->priority, p->CPUBurst, p->arrivalTime) == -1) {
    fprintf(stderr, "FATAL: Could not allocate memory.");
    exit(EXIT_FAILURE);
  };

  return str;
}

/*
 * Dynamically allocates a string that contains all the starting details of the
 * process.
 *
 *
 * @return starting details of the process (usually the ones passed to
 * initProcess).
 */
char *endDetails(Process *p) {
  char *str;
  if (asprintf(
          &str,
          "P%d (PID: %d, priority: %d, CPUBurst: %d, arrivalTime: %d, "
          "responseTime: %d, turnAroundTime: %d, waitingTime: %d, startTime: "
          "%d, completionTime: %d)",
          p->pid, p->pid, p->priority, p->CPUBurst, p->arrivalTime,
          p->responseTime, p->turnAroundTime, p->waitingTime, p->startTime,
          p->completionTime) == -1) {
    fprintf(stderr, "FATAL: Could not allocate memory.");
    exit(EXIT_FAILURE);
  };
  return str;
}

/*
 * Calculates the average turnAround time of an array of proccesses.
 */
float avgTurnAroundTime(Process proc[], int proc_qty) {
  float total = 0;
  for (int i = 0; i < proc_qty; i++) {
    total += proc[i].turnAroundTime;
  }
  return total / proc_qty;
};

/*
 * Calculates the average responseTime time of an array of proccesses.
 */
float avgResponseTime(Process proc[], int proc_qty) {
  float total = 0;
  for (int i = 0; i < proc_qty; i++) {
    total += proc[i].responseTime;
  }
  return total / proc_qty;
};

/*
 * Calculates the average waitingTime of an array of proccesses.
 */
float avgWaitingTime(Process proc[], int proc_qty) {
  float total = 0;
  for (int i = 0; i < proc_qty; i++) {
    total += proc[i].waitingTime;
  }
  return total / proc_qty;
};
