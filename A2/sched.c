/*
 * Student Name:    Dusan Barudzija and Connor Carroll
 * Student ID:      3121720 and 3116723
 * Submission Date: May 24, 2026
 * File Name:       sched.c
 * Description:     CPU scheduler simulator (FCFS policy).
 *                  Reads a workload file (ignoring # comment lines),
 *                  simulates single-CPU execution tick by tick, and prints a
 *                  Gantt-style timeline followed by per-process and system-level
 *                  metrics.
 */

#include "sched.h"

// Queue creation
// Param: *q - Pointer to the queue
// void return
void queue_init(Queue *q)
{
    q->head = q->tail = NULL; // Empty Queue creation
}

// Add element to queue
// Param: *q - Queue pointer, index - The current process index
// Void return
void queue_enqueue(Queue *q, int index)
{
    QNode *node = malloc(sizeof(QNode)); // Allocate mem to new node
    if (!node) { perror("malloc"); exit(1); } // Crash if cannot allocate memory
    node->index = index; // Index of the process
    node->next  = NULL; 
    if (q->tail) q->tail->next = node; // If not empty, old tail links to new node
    else         q->head       = node; // If empty, new node = head
    q->tail = node; // New node always new tail
}

// Remove and return element from queue
// Param: *q - pointer to queue 
// Return: index of next process in queue, -1 if null
int queue_dequeue(Queue *q)
{
    if (!q->head) return -1;
    QNode *tmp = q->head;
    int    idx = tmp->index;
    q->head    = tmp->next; 
    if (!q->head) q->tail = NULL; // if queue empty, tail also NULL
    free(tmp); // free old front node
    return idx; // Return process index
}

// Check if queue is empty
// Param: *q - pointer to queue
// Return: 1 if queue is empty, 0 elsewise
int queue_empty(Queue *q) { return q->head == NULL; } // True if queue is empty

// Free the address space of the queue
// Param *q - pointer to queue
// Void return
void queue_free(Queue *q) 
{
    while (!queue_empty(q)) queue_dequeue(q); // keep dequeuing until queue is empty
}

// Output helper, prints cpu run timeline
// Param: *run - pointer to array of which process ran at each tick, total_ticks - total cpu ticks in run
// Void return
void print_timeline(int *run, int total_ticks)
{
    printf("time:");
    for (int t = 0; t < total_ticks; t++) printf(" %d", t); // Prints tick numbers
    printf("\n");
    printf("run :");
    for (int t = 0; t < total_ticks; t++) {
        if (run[t] == -1) printf(" -"); // If CPU was idle (-1), print "-"
        else              printf(" %d", run[t]); // Otherwise print the PID of the tick
    }
    printf("\n");
}

// Output helper, prints run metrics
// Param: *procs - pointer to process list, n - number of processes, *run - array of which process ran at each tick, total_ticks - total cpu ticks in run
// Void return
void print_metrics(Process *procs, int n, int *run, int total_ticks)
{
    double sum_tat = 0.0, sum_resp = 0.0; // Doubles for calculating averages
    for (int i = 0; i < n; i++) {
        int tat  = procs[i].completion - procs[i].arrival; // turnaround time calc
        int resp = procs[i].first_run  - procs[i].arrival; // response time calc
        printf("P%d: first run=%d completion=%d TAT=%d RESP=%d\n",
               procs[i].pid, procs[i].first_run,
               procs[i].completion, tat, resp); // print per process line
        sum_tat  += tat;
        sum_resp += resp; 
    }
	// count context switches
    int ctx = 0, prev = -1;
    for (int t = 0; t < total_ticks; t++) {
        int cur = run[t];
        if (cur >= 0 && prev >= 0 && cur != prev) ctx++; // PID changed and niether idle -> switch
        if (cur >= 0) prev = cur; // update prev, but only if not idle
    }
    printf("System: ctx_switches=%d, avgTAT=%.3f, avgRESP=%.3f\n",
           ctx, sum_tat / n, sum_resp / n); // print system wide averages
}


 // The FCFS scheduling method. run each process to completion then go to next.
// Param: *procs - pointer to process array, n - number of processes
// void return
void run_fcfs(Process *procs, int n)
{
    int *run = malloc(sizeof(int) * MAX_TIME); // allocate for an array for which PID runs each tick
    if (!run) { perror("malloc"); exit(1); }

    /* insertion sort used for arrival time */
    for (int i = 1; i < n; i++) {
        Process key = procs[i]; int j = i - 1;
        while (j >= 0 && procs[j].arrival > key.arrival) { procs[j+1] = procs[j]; j--; }
        procs[j+1] = key;
    }

    Queue ready; queue_init(&ready);
    int completed = 0, enqueued = 0, current = -1, t = 0;

    while (completed < n) { // Loop until all processes finish
        while (enqueued < n && procs[enqueued].arrival <= t) {
            queue_enqueue(&ready, enqueued++); // any process that arrived by tick t goes into ready queue
        }
        if (current == -1 && !queue_empty(&ready))
            current = queue_dequeue(&ready); // grab next process in front of queue

        if (current == -1) {
            run[t] = -1; // If no process, CPU stays idle on this tick
        } else {
            if (!procs[current].has_run) {
                procs[current].first_run = t; // record very first tick this process ran
                procs[current].has_run   = 1; // flag so we don't overwrite
            }
            run[t] = procs[current].pid; // Record the pid that ran this tick
            if (--procs[current].remaining == 0) { // decrement
                procs[current].completion = t + 1; // completion means one past the last tick it ran
                completed++; // Keep going until complete
                current = -1; // free the CPU
            }
        }
        t++;
    }

    print_timeline(run, t); // Print Gantt chart
    print_metrics(procs, n, run, t); // print TAT, RESP, and context switches
    queue_free(&ready); // Free all queue nodes
    free(run); // free timeline array
}

// print usage instructions.
// Param: program name
static void usage(const char *prog)
	// Prints error response in case invalid format is inputted
{
    fprintf(stderr, "Usage: %s --policy=FCFS|RR [--quantum=N] --in=FILE\n", prog);
    exit(1);
}

// Main takes and parses input, sets up data and runs appropriate scheduler
// Param: argument data, count and arguements
// Return: status
int main(int argc, char *argv[])
{
    char policy[16]  = {0}; // holds "FCFS" or "RR"
    char infile[256] = {0}; // holds the file name 

    for (int i = 1; i < argc; i++) {
        if      (strncmp(argv[i], "--policy=",  9) == 0) strncpy(policy, argv[i]+9,  sizeof(policy)-1); // extract "FCFS" or "RR"
        else if (strncmp(argv[i], "--in=",      5) == 0) strncpy(infile, argv[i]+5,  sizeof(infile)-1); // extract filename
        else { fprintf(stderr, "Unknown argument: %s\n", argv[i]); usage(argv[0]); } // Throw error (run usage function) if invalid inputs
    }

    if (policy[0] == '\0' || infile[0] == '\0') usage(argv[0]); // if inputs missing run usage function

    FILE *fp = fopen(infile, "r");
    if (!fp) { perror(infile); exit(1); } // crash with message if file doesn't exist

    Process procs[MAX_PROCESSES];
    int  n = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++; // Skip leading whitespace
        if (*p == '#' || *p == '\n' || *p == '\r' || *p == '\0') continue; // skip comment/blank lines
        if (sscanf(p, "%d %d %d",
                   &procs[n].pid, &procs[n].arrival, &procs[n].cpu_time) == 3) {
            procs[n].remaining  = procs[n].cpu_time; // remaining burst starts at full cpu_time
            procs[n].first_run  = 0; // placeholder, updates when ran
            procs[n].completion = 0; // placeholder, updates when ran
            procs[n].has_run    = 0; // flag = 0 = has not run yet
            n++;
            if (n >= MAX_PROCESSES) { // safety check
                fprintf(stderr, "Too many processes (max %d)\n", MAX_PROCESSES); exit(1);
            }
        }
    }
    fclose(fp);

    if (n == 0) { fprintf(stderr, "No processes in workload file.\n"); exit(1); }

    if      (strcmp(policy, "FCFS") == 0) run_fcfs(procs, n); // run FCFS scheduler
    else    { fprintf(stderr, "Unknown policy: %s\n", policy); usage(argv[0]); } // If anything else, throw usage error

    return 0;
}
