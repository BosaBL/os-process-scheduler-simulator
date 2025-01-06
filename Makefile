default: all

all: simulator

simulator: priorityq.o
	gcc priorityq.o -o simulator
	./simulator

clean:
	rm -rfd *.o
