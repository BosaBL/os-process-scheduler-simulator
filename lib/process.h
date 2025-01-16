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

/*
 * Initializes a process with the required starting data.
 *
 * @param priority will only be used on priority based simulations.
 */
void initProcess(Process *p, int pid, int priority, int CPUBurst,
                 int arrivalTime);

/*
 * Initializes a process with the required starting data.
 * @param priority will only be used on priority based simulations.
 */
void initRandomProcess(Process *p, int pid, minMaxRange priority,
                       minMaxRange CPUBurst, minMaxRange arrivalTime);

/*
 * Initializes the passed array of processes with the data extracted from a
 * file.
 */
void initProcessesFromFile(Process p[], char *file);

/*
 * Dynamically allocates a string that contains all the starting details of the
 * process.
 * Usually the starting details are the parameters of initProcess function.
 *
 * @return starting details of the process.
 */
char *startDetails(Process *p);

/*
 * Dynamically allocates a string that contains all the starting details of the
 * process.
 *
 *
 * @return starting details of the process (usually the ones passed to
 * initProcess).
 */
char *endDetails(Process *p);

/*
 * Calculates the average turnAround time of an array of proccesses.
 */
float avgTurnAroundTime(Process proc[], int proc_qty);

/*
 * Calculates the average responseTime time of an array of proccesses.
 */
float avgResponseTime(Process proc[], int proc_qty);

/*
 * Calculates the average waitingTime of an array of proccesses.
 */
float avgWaitingTime(Process proc[], int proc_qty);

#endif // !PROCESS_H
