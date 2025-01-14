#include "./lib/utils.h"
#include "lib/fort.h"
#include "lib/process.h"
#include "simulators/simulators.h"
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *argp_program_version = "v1.0";
int verbose = 0;
int context_cost = 0;

/* Program documentation. */
static char doc[] =
    "CPU Scheuling Simulator -- Program that executes simulation and execution "
    "of processes on a CPU, provides metrics and comparation of: \n"
    "-- non-preemptive priority scheduling,\n"
    "-- preemptive priority scheduling,\n"
    "-- round robin scheduling.\n";

static struct argp_option options[] = {
    {"random", 'z', "INTEGER", 0, "Generates random processes to test."},
    {"np_scheduling", 'n', 0, 0, "Simulate non-preemptive priority scheduling"},
    {"pp_scheduling", 'p', 0, 0, "Simulate preemptive priority scheduling"},
    {"round_robin", 'r', 0, 0, "Simulate round robin scheduling."},
    {"input_file", 'f', "FILE", 0, "Input file with processes to simulate."},
    {"verbose", 'v', 0, 0, "Print process execution verbosely."},
    {"quantum", 'q', "INTEGER", OPTION_ARG_OPTIONAL,
     "Quantum time for round robin schediling."},
    {"context_cost", 'c', "INTEGER", OPTION_ARG_OPTIONAL,
     "Context switching time cost, 0 by default."},
    {"process_stats", 's', 0, 0, "Show per process stats."},
    {0}};

struct arguments {
  int np_scheduling, pp_scheduling, round_robin, process_qty, quantum,
      process_stats;
  char *input_file;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
  case 'n':
    arguments->np_scheduling = 1;
    break;
  case 'p':
    arguments->pp_scheduling = 1;
    break;
  case 'r':
    arguments->round_robin = 1;
    break;
  case 's':
    arguments->process_stats = 1;
    break;
  case 'q':
    if (arg != NULL) {
      arguments->quantum = atoi(arg);
    }
    break;
  case 'c':
    if (arg != NULL) {
      context_cost = atoi(arg);
    }
    break;
  case 'f':
    arguments->input_file = arg;
    break;
  case 'v':
    verbose = 1;
    break;
  case 'z':
    arguments->process_qty = atoi(arg);
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, 0, doc};

int main(int argc, char **argv) {

  struct arguments arguments;
  arguments.np_scheduling = 0;
  arguments.pp_scheduling = 0;
  arguments.round_robin = 0;
  arguments.quantum = 0;
  arguments.process_qty = 0;
  arguments.process_stats = 0;
  arguments.input_file = NULL;

  argp_parse(&argp, argc, argv, ARGP_NO_ARGS, 0, &arguments);
  clearScreen();
  if (arguments.process_qty <= 0 && arguments.input_file == NULL) {
    fprintf(stderr, "ERROR: Random process generation (-z) XOR input file "
                    "(-f FILE) needs to be defined.\n");
    exit(EXIT_FAILURE);
  }

  if (arguments.round_robin && arguments.quantum < 1) {
    fprintf(stderr, "ERROR: Quantum (-q) must be an integer greater than 0.");
    exit(EXIT_FAILURE);
  }

  if (arguments.np_scheduling == 0 && arguments.pp_scheduling == 0 &&
      arguments.round_robin == 0) {
    fprintf(stderr,
            "ERROR: At least one algorithm to simulate should be chosen;\n"
            "\tNon-preemptive priority scheduling (-n)\n"
            "\tPreemptive priority scheduling (-p)\n"
            "\tRound robin (-r)\n");
    exit(EXIT_FAILURE);
  }

  // Initializing all the process related data.
  algorithm_eval *np_priority_data = NULL;
  algorithm_eval *pp_priority_data = NULL;
  algorithm_eval *rr_data = NULL;
  Process *proc_array = NULL;
  int *proc_qty = NULL;

  // Intializing data related to the quantity of algorithms to simulate.
  int algorithm_qty = 0;
  Process *process_data_array[3] = {};
  algorithm_eval *algorithm_data_array[3] = {};

  srandom(time(NULL));

  // Instantiate processes, if both file and random flag are given, it will
  // use the data from the file.
  if (arguments.input_file != NULL) {
    int file_lines = countFileLines(arguments.input_file);
    proc_qty = &file_lines;
    proc_array = malloc((int)sizeof(Process) * *(proc_qty));

    initProcessesFromFile(proc_array, arguments.input_file);

  } else if (arguments.process_qty > 0) {
    proc_qty = &arguments.process_qty;
    proc_array = malloc((int)sizeof(Process) * *(proc_qty));

    for (int i = 0; i < *(proc_qty); i++) {
      initRandomProcess(&proc_array[i], i, (minMaxRange){1, 100},
                        (minMaxRange){1, 50}, (minMaxRange){1, 30});
    }
  }

  // Checking if proccesses have been instantiated correctly.
  if (proc_array == NULL || proc_qty == NULL) {
    fprintf(
        stderr,
        "FATAL: There was an error during allocation of process details.\n");
    exit(EXIT_FAILURE);
  }

  clearScreen();

  // Initialize table
  ft_table_t *table = ft_create_table();
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_write_ln(table, "Algorithm", "ProccesQTY", "totalTime", "IdleTime",
              "avgResponseTime", "avgTurnAroundTime", "avgWaitingTime");
  Process np_array[*proc_qty] = {};
  Process pp_array[*proc_qty] = {};
  Process rr_array[*proc_qty] = {};

  if (arguments.np_scheduling) {
    memcpy(np_array, proc_array, sizeof(Process) * *(proc_qty));

    np_priority_data = priority_scheduling(np_array, *(proc_qty), 0);

    np_priority_data->alg = "NP";
    ft_printf_ln(table, "%s|%d|%d|%d|%f|%f|%f", np_priority_data->alg,
                 *proc_qty, np_priority_data->totalTime,
                 np_priority_data->idleTime, np_priority_data->avgResponseTime,
                 np_priority_data->avgTurnAroundTime,
                 np_priority_data->avgWaitingTime);

    algorithm_data_array[algorithm_qty] = np_priority_data;
    process_data_array[algorithm_qty] = np_array;
    algorithm_qty++;
  }

  if (arguments.pp_scheduling) {
    memcpy(pp_array, proc_array, sizeof(Process) * *(proc_qty));

    pp_priority_data = priority_scheduling(pp_array, *(proc_qty), 1);

    pp_priority_data->alg = "PP";
    ft_printf_ln(table, "%s|%d|%d|%d|%f|%f|%f", pp_priority_data->alg,
                 *proc_qty, pp_priority_data->totalTime,
                 pp_priority_data->idleTime, pp_priority_data->avgResponseTime,
                 pp_priority_data->avgTurnAroundTime,
                 pp_priority_data->avgWaitingTime);

    algorithm_data_array[algorithm_qty] = pp_priority_data;
    process_data_array[algorithm_qty] = pp_array;
    algorithm_qty++;
  }

  if (arguments.round_robin) {
    memcpy(rr_array, proc_array, sizeof(Process) * *(proc_qty));

    rr_data = round_robin_scheduling(rr_array, *(proc_qty), arguments.quantum);

    rr_data->alg = "RR";
    ft_printf_ln(table, "%s|%d|%d|%d|%f|%f|%f", rr_data->alg, *proc_qty,
                 rr_data->totalTime, rr_data->idleTime,
                 rr_data->avgResponseTime, rr_data->avgTurnAroundTime,
                 rr_data->avgWaitingTime);

    algorithm_data_array[algorithm_qty] = rr_data;
    process_data_array[algorithm_qty] = rr_array;
    algorithm_qty++;
  }

  ft_table_t *table2 = ft_create_table();
  ft_set_cell_prop(table2, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table2, 1, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);

  printf("\n\n=======PROCESS METRICS=======\n");

  switch (algorithm_qty) {

    // Print table for 1 process.
  case (1):

    ft_write_ln(table2, "", algorithm_data_array[0]->alg,
                algorithm_data_array[0]->alg, algorithm_data_array[0]->alg);
    ft_write_ln(table2, "Process", "FinishTime", "TurnAroundTime",
                "WaitingTime");
    for (int i = 0; i < *(proc_qty); i++) {
      Process p0 = process_data_array[0][i];

      ft_printf_ln(table2, "%d|%d|%d|%d", p0.pid, p0.completionTime,
                   p0.turnAroundTime, p0.waitingTime);
    }

    break;

    // Print table for 2 processes.
  case (2):
    ft_write_ln(table2, "", algorithm_data_array[0]->alg,
                algorithm_data_array[0]->alg, algorithm_data_array[0]->alg,
                algorithm_data_array[1]->alg, algorithm_data_array[1]->alg,
                algorithm_data_array[1]->alg);
    ft_write_ln(table2, "Process", "FinishTime", "TurnAroundTime",
                "WaitingTime", "FinishTime", "TurnAroundTime", "WaitingTime");

    for (int i = 0; i < *(proc_qty); i++) {
      Process p0 = process_data_array[0][i];
      Process p1 = process_data_array[1][i];

      ft_printf_ln(table2, "%d|%d|%d|%d|%d|%d|%d", p0.pid, p0.completionTime,
                   p0.turnAroundTime, p0.waitingTime, p1.completionTime,
                   p1.turnAroundTime, p1.waitingTime);
    }
    break;

    // Print table for 3 processes.
  case (3):
    ft_write_ln(table2, "", algorithm_data_array[0]->alg,
                algorithm_data_array[0]->alg, algorithm_data_array[0]->alg,
                algorithm_data_array[1]->alg, algorithm_data_array[1]->alg,
                algorithm_data_array[1]->alg, algorithm_data_array[2]->alg,
                algorithm_data_array[2]->alg, algorithm_data_array[2]->alg);
    ft_write_ln(table2, "Process", "FinishTime", "TurnAroundTime",
                "WaitingTime", "FinishTime", "TurnAroundTime", "WaitingTime",
                "FinishTime", "TurnAroundTime", "WaitingTime");

    for (int i = 0; i < *(proc_qty); i++) {
      Process p0 = process_data_array[0][i];
      Process p1 = process_data_array[1][i];
      Process p2 = process_data_array[2][i];

      ft_printf_ln(table2, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d", p0.pid,
                   p0.completionTime, p0.turnAroundTime, p0.waitingTime,
                   p1.completionTime, p1.turnAroundTime, p1.waitingTime,
                   p2.completionTime, p2.turnAroundTime, p2.waitingTime);
    }
    break;
  }
  if (arguments.process_stats) {
    printf("%s\n\n", ft_to_string(table2));
  }

  printf("=======ALGORITHM METRICS=======\n");
  printf("%s\n", ft_to_string(table));

  ft_destroy_table(table);
  exit(EXIT_SUCCESS);
}
