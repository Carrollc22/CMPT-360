/*
 * Student Name:    Dusan Barudzija + Connor Carroll
 * Student ID:      3121720 and 3116723
 * Submission Date: May 24, 2026
 * File Name:       sched.h
 * Description:     Header file for CPU scheduler simulator (FCFS and RR)
 */

#ifndef SCHED_H
#define SCHED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 1024
#define MAX_TIME      100000

/* ---------- Process descriptor ---------- */
typedef struct {
    int pid;
    int arrival;
    int cpu_time;       /* total CPU burst required          */
    int remaining;      /* CPU burst still needed            */
    int first_run;      /* tick when process first ran       */
    int completion;     /* one past the last tick it ran     */
    int has_run;        /* flag: 0 = never scheduled yet     */
} Process;

/* ---------- Queue node ---------- */
typedef struct QNode {
    int           index;   /* index into procs[]             */
    struct QNode *next;
} QNode;

/* ---------- Simple FIFO queue ---------- */
typedef struct {
    QNode *head;
    QNode *tail;
} Queue;

/* Queue operations */
void  queue_init   (Queue *q);
void  queue_enqueue(Queue *q, int index);
int   queue_dequeue(Queue *q);          /* returns index, or -1 if empty */
int   queue_empty  (Queue *q);
void  queue_free   (Queue *q);

/* Scheduler entry point */
void run_fcfs(Process *procs, int n);

/* Output helpers */
void print_timeline(int *run, int total_ticks);
void print_metrics (Process *procs, int n, int *run, int total_ticks);

#endif /* SCHED_H */
