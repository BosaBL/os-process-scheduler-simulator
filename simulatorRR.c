#include <stdio.h>
#include <stdlib.h>

// Definición del tipo Process
typedef struct Process {
    int pid;
    int CPUBurst;
    int arrivalTime;
    int responseTime;
    int turnAroundTime;
    int waitingTime;
    int completionTime;
    int remainingTime;
} Process;

// Variables globales para la cola circular
Process *queue[100];
int front = 0, rear = -1, size = 0, max_size;

// -------------------- Funciones de cola circular --------------------
void enqueue(Process *p) {
    rear = (rear + 1) % max_size;
    queue[rear] = p;
    size++;
}

Process *dequeue() {
    Process *p = queue[front];
    front = (front + 1) % max_size;
    size--;
    return p;
}

int is_in_queue(Process *p) {
    for (int i = 0; i < size; i++) {
        int idx = (front + i) % max_size;
        if (queue[idx]->pid == p->pid) {
            return 1; // está en cola
        }
    }
    return 0; // no está en cola
}

// -------------------- Algoritmo Round Robin --------------------
void round_robin(Process proc[], int proc_qty, int quantum) {
    int time = 0, completed = 0;
    max_size = proc_qty;

    // Inicializar procesos y encolar los que llegan en t=0
    for (int i = 0; i < proc_qty; i++) {
        proc[i].responseTime  = -1;
        proc[i].remainingTime = proc[i].CPUBurst;
        proc[i].completionTime= 0;
        proc[i].turnAroundTime= 0;
        proc[i].waitingTime   = 0;

        if (proc[i].arrivalTime == 0) {
            enqueue(&proc[i]);
        }
    }

    // Mientras no se hayan completado todos
    while (completed < proc_qty) {

        // Si la cola está vacía, se adelanta el tiempo
        if (size == 0) {
            time++;
            // Encolar procesos que lleguen a este nuevo 'time'
            for (int i = 0; i < proc_qty; i++) {
                if (proc[i].arrivalTime <= time &&
                    proc[i].remainingTime > 0 &&
                    !is_in_queue(&proc[i])) 
                {
                    enqueue(&proc[i]);
                }
            }
            continue;
        }

        // Sacar el primero de la cola
        Process *executing = dequeue();

        // Cuánto va a ejecutarse: quantum o lo que quede
        int exec_time = (executing->remainingTime > quantum) ? quantum : executing->remainingTime;

        // Marcar responseTime si es la primera vez en CPU
        if (executing->responseTime == -1) {
            executing->responseTime = time - executing->arrivalTime;
        }

        int start_time = time;
        time += exec_time;
        executing->remainingTime -= exec_time;

        // **Imprimir** el intervalo de ejecución
        printf("Time %d to %d: Executing P%d\n",
               start_time, time, executing->pid);

        // Encolar procesos que llegaron durante [start_time+1 .. time],
        // EXCLUYENDO el proceso 'executing' (para evitar doble encolado)
        for (int i = 0; i < proc_qty; i++) {
            if (&proc[i] == executing) {
                continue; // No se considera re-encolar el mismo proceso en arrival check
            }
            if (proc[i].arrivalTime <= time &&
                proc[i].remainingTime > 0 &&
                !is_in_queue(&proc[i])) 
            {
                enqueue(&proc[i]);
            }
        }

        // Si el proceso no ha terminado, se reencola
        if (executing->remainingTime > 0) {
            enqueue(executing);
        } else {
            // Calculo de métricas
            executing->completionTime = time;
            executing->turnAroundTime = executing->completionTime - executing->arrivalTime;
            executing->waitingTime = executing->turnAroundTime - executing->CPUBurst;
            completed++;
        }
    }

    // Calculo promedios finales
    float totalTAT = 0, totalResp = 0, totalWait = 0;
    for (int i = 0; i < proc_qty; i++) {
        totalTAT  += proc[i].turnAroundTime;
        totalResp += proc[i].responseTime;
        totalWait += proc[i].waitingTime;
    }

    // Muestra de resultados
    printf("\nROUND ROBIN RESULTS:\n");
    printf("Total Time: %d\n", time);
    printf("Average Turnaround Time: %.2f\n", totalTAT / proc_qty);
    printf("Average Response Time: %.2f\n", totalResp / proc_qty);
    printf("Average Waiting Time: %.2f\n", totalWait / proc_qty);
}

// -------------------- Funcion main() --------------------
int main() {
    int MAX_P = 5;
    int quantum = 2;
    Process a[MAX_P];

    a[0] = (Process){.pid = 1, .arrivalTime = 0, .CPUBurst = 5};
    a[1] = (Process){.pid = 2, .arrivalTime = 0, .CPUBurst = 4};
    a[2] = (Process){.pid = 3, .arrivalTime = 0, .CPUBurst = 3};
    a[3] = (Process){.pid = 4, .arrivalTime = 0, .CPUBurst = 6};
    a[4] = (Process){.pid = 5, .arrivalTime = 0, .CPUBurst = 2};

    round_robin(a, MAX_P, quantum);
    return 0;
}
