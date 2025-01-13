VAR=gcc main.c $\
		lib/priorityq.c $\
		lib/process.c $\
		lib/algorithm_eval.c $\
		lib/utils.c $\
		lib/fort.c $\
		lib/queue.c $\
		simulators/prioritySimulator.c $\
		simulators/roundRobinSimulator.c $\
		-o main.o

default: all

all: main.o
	$(VAR)

clean:
	rm -rfd *.o
