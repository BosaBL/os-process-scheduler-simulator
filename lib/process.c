#include "process.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Initializes a process with the required starting data.
 * @param priority will only be used on priority based simulations. */
void initRandomProcess(Process *p, int pid, minMaxRange priority,
                       minMaxRange CPUBurst, minMaxRange arrivalTime) {

  p->pid = pid;
  p->priority = random() % (priority.max - priority.min + 1) + priority.min;
  p->CPUBurst = random() % (CPUBurst.max - CPUBurst.min + 1) + CPUBurst.min;
  p->arrivalTime =
      random() % (arrivalTime.max - arrivalTime.min + 1) + arrivalTime.min;
  p->responseTime = -1;
  p->turnAroundTime = 0;
  p->waitingTime = 0;
  p->startTime = -1;
  p->completionTime = 0;
  p->internalExecutionTime = 0;
}

/*
 * Initializes the passed array of processes with the data extracted from a
 * file.
 */
void initProcessesFromFile(Process p[], char *file) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(file, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  int i = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    if (line[0] == '-' || read <= 1) {
      continue;
    }
    char parse[256];

    parseString(line, parse);
    char *tok = strtok(parse, ",");

    int priority = atoi(tok);
    int CPUBurst = atoi(strtok(NULL, ","));
    int arrivalTime = atoi(strtok(NULL, ","));

    initProcess(&p[i], i, priority, CPUBurst, arrivalTime);
    printf("%s\n", startDetails(&p[i]));
    i++;
  }

  fclose(fp);
  if (line)
    free(line);
};

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
