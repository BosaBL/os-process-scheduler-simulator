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

// Cola circular global
Process *queue[100];
int front = 0, rear = -1, size = 0, max_size;

// -------------------- FUNCIONES DE COLA --------------------
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
            return 1; // está en la cola
        }
    }
    return 0; // no está en cola
}

// -------------------- FUNCIÓN PARA ENCOLAR LLEGADAS --------------------
void check_arrivals(Process proc[], int proc_qty, int time) {
    // Por cada proceso, si ya llegó (arrivalTime <= time) y no ha terminado
    // y no está en la cola, se encola
    for (int i = 0; i < proc_qty; i++) {
        if (proc[i].arrivalTime <= time &&
            proc[i].remainingTime > 0 &&
            !is_in_queue(&proc[i])) 
        {
            enqueue(&proc[i]);
        }
    }
}

// -------------------- ROUND ROBIN PASO A PASO --------------------
void round_robin_tick_by_tick(Process proc[], int proc_qty, int quantum) {
    max_size = proc_qty;
    int completed = 0;
    int time = 0;

    // Inicializar cada proceso
    for (int i = 0; i < proc_qty; i++) {
        proc[i].responseTime  = -1;
        proc[i].remainingTime = proc[i].CPUBurst;
        proc[i].completionTime= 0;
        proc[i].turnAroundTime= 0;
        proc[i].waitingTime   = 0;
    }

    // Encolar los que llegan en t=0
    check_arrivals(proc, proc_qty, 0);

    // El proceso actualmente en CPU (si hay)
    Process *current = NULL;
    // Cuánto lleva consumido del quantum actual
    int usedQuantum = 0;

    while (completed < proc_qty) {
        
        // Si no hay proceso ejecutándose, se saca uno de la cola (si existe)
        if (current == NULL) {
            // Si la cola está vacía, avanza 1
            if (size == 0) {
                time++;
                // Revisar si llegan procesos a este nuevo time
                check_arrivals(proc, proc_qty, time);
                continue; 
            }
            // Se saca nuevo proceso a ejecutar
            current = dequeue();

            // Si es la primera vez que va a CPU, calcula responseTime
            if (current->responseTime == -1) {
                current->responseTime = time - current->arrivalTime;
            }
            usedQuantum = 0;
        }

        // **Ejecutar** 1 tick
        current->remainingTime--;
        usedQuantum++;

        // Imprime la ejecución de este tick
        printf("Time %d to %d: Executing P%d\n", time, time+1, current->pid);

        time++;

        // Revisa si durante este "tick" llegaron nuevos procesos
        check_arrivals(proc, proc_qty, time);

        // Verifica si el proceso terminó
        if (current->remainingTime == 0) {
            // Proceso completado
            current->completionTime = time;
            current->turnAroundTime = current->completionTime - current->arrivalTime;
            current->waitingTime = current->turnAroundTime - current->CPUBurst;
            completed++;

            // Libera 'current'
            current = NULL;
            usedQuantum = 0;
        }
        else if (usedQuantum == quantum) {
            // Si consumió su quantum, se reencola
            enqueue(current);
            current = NULL;
            usedQuantum = 0;
        }

        // Si no pasó nada, en el próximo tick se sigue el mismo proceso 'current'
    }

    // Cálculo de métricas finales
    float totalTAT = 0, totalResp = 0, totalWait = 0;
    for (int i = 0; i < proc_qty; i++) {
        totalTAT  += proc[i].turnAroundTime;
        totalResp += proc[i].responseTime;
        totalWait += proc[i].waitingTime;
    }

    printf("\nROUND ROBIN RESULTS:\n");
    printf("Total Time: %d\n", time);
    printf("Average Turnaround Time: %.2f\n", totalTAT / proc_qty);
    printf("Average Response Time: %.2f\n", totalResp / proc_qty);
    printf("Average Waiting Time: %.2f\n", totalWait / proc_qty);
}

int main() {
    int proc_qty = 5;
    int quantum = 2;
    
    // Ejemplo: P1 y P2 llegan en 0, P3,P4,P5 en 5
    Process a[5];
    a[0] = (Process){.pid = 1, .CPUBurst = 5, .arrivalTime = 0};
    a[1] = (Process){.pid = 2, .CPUBurst = 4, .arrivalTime = 0};
    a[2] = (Process){.pid = 3, .CPUBurst = 3, .arrivalTime = 5};
    a[3] = (Process){.pid = 4, .CPUBurst = 6, .arrivalTime = 5};
    a[4] = (Process){.pid = 5, .CPUBurst = 2, .arrivalTime = 5};

    round_robin_tick_by_tick(a, proc_qty, quantum);
    return 0;
}
