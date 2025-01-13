#ifndef PROCESS_H
#define PROCESS_H

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
  int internalExecutionTime;
} Process;

typedef struct minMaxRange {
  int min;
  int max;
} minMaxRange;

void initProcess(Process *p, int pid, int priority, int CPUBurst,
                 int arrivalTime);

void initRandomProcess(Process *p, int pid, minMaxRange priority,
                       minMaxRange CPUBurst, minMaxRange arrivalTime);

char *startDetails(Process *p);

char *endDetails(Process *p);

float avgTurnAroundTime(Process proc[], int proc_qty);

float avgResponseTime(Process proc[], int proc_qty);

float avgWaitingTime(Process proc[], int proc_qty);

#endif // !PROCESS_H
