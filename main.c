#include "lib/fort.h"
#include "lib/process.h"
#include "lib/utils.h"
#include "simulators/simulators.h"
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *argp_program_version = "v1.0";
int verbose = 0;

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
    {"vervose", 'v', 0, 0, "Print process execution verbosely."},
    {0}};

struct arguments {
  int np_scheduling, pp_scheduling, round_robin, process_qty;
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
  clearScreen();
  struct arguments arguments;
  arguments.np_scheduling = 0;
  arguments.pp_scheduling = 0;
  arguments.round_robin = 0;
  arguments.process_qty = 0;
  arguments.input_file = NULL;

  argp_parse(&argp, argc, argv, ARGP_NO_ARGS, 0, &arguments);

  if (arguments.process_qty <= 0 && arguments.input_file == NULL) {
    fprintf(stderr, "ERROR: Random process generation (-z) XOR input file (-f "
                    "FILE) needs to be defined.\n");
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
  algorithm_eval algorithm_data_array[3] = {};

  srandom(time(NULL));

  // Instantiate processes, if both file and random flag are given, it will use
  // the data from the file.
  if (arguments.input_file != NULL) {
    int file_lines = countFileLines(arguments.input_file);
    proc_qty = &file_lines;
    proc_array = malloc((int)sizeof(Process) * *(proc_qty));
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

  // for (int i = 0; i < *proc_qty; i++) {
  //   printf("%d ", proc_array[i].arrivalTime);
  // }
  // printf("\n");
  // for (int i = 0; i < *proc_qty; i++) {
  //   printf("%d ", proc_array[i].CPUBurst);
  // }
  // printf("\n");
  // for (int i = 0; i < *proc_qty; i++) {
  //   printf("%d ", proc_array[i].priority);
  // }
  // printf("\n");

  // Initialize table
  ft_table_t *table = ft_create_table();
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_write_ln(table, "Algorithm", "ProccesQTY", "totalTime", "IdleTime",
              "avgResponseTime", "avgTurnAroundTime", "avgWaitingTime");

  if (arguments.np_scheduling) {
    Process np_array[*proc_qty] = {};
    memcpy(np_array, proc_array, sizeof(Process) * *(proc_qty));

    np_priority_data = priority_scheduling(np_array, *(proc_qty), 0);

    ft_printf_ln(table, "%s|%d|%d|%d|%f|%f|%f", np_priority_data->alg,
                 *proc_qty, np_priority_data->totalTime,
                 np_priority_data->idleTime, np_priority_data->avgResponseTime,
                 np_priority_data->avgTurnAroundTime,
                 np_priority_data->avgWaitingTime);

    algorithm_qty++;
  }

  if (arguments.pp_scheduling) {
    Process pp_array[*proc_qty] = {};
    memcpy(pp_array, proc_array, sizeof(Process) * *(proc_qty));

    pp_priority_data = priority_scheduling(pp_array, *(proc_qty), 1);

    ft_printf_ln(table, "%s|%d|%d|%d|%f|%f|%f", pp_priority_data->alg,
                 *proc_qty, pp_priority_data->totalTime,
                 pp_priority_data->idleTime, pp_priority_data->avgResponseTime,
                 pp_priority_data->avgTurnAroundTime,
                 pp_priority_data->avgWaitingTime);
  }

  if (arguments.round_robin) {
  }

  printf("======= METRICS =======\n");
  printf("%s\n", ft_to_string(table));
  ft_destroy_table(table);

  exit(EXIT_SUCCESS);
}
