<!--
Student Name:    Dusan Barudzija and Connor Carroll
Student ID:      3121720 and 3116723
Submission Date: May 24, 2026
File Name:       README.md
Description:     README for CMPT 360 Assignment 2 – CPU Scheduler Simulator
-->

# CMPT 360 A2 – CPU Scheduler Simulator

## Solution Logic

The program (`sched`) simulates a single-CPU scheduler in discrete ticks. Two
scheduling policies are supported:

### FCFS (First-Come, First-Served)
A simple FIFO queue. Processes are sorted by arrival time at startup. Each tick,
any process that has just arrived is enqueued. The head of the queue runs without
preemption until its entire CPU burst completes. This is non-preemptive and
trivially simple to implement.

### Round Robin (RR)
Uses the same FIFO ready queue, but adds a quantum counter. Each tick a process
is charged one tick against its quantum. When the quantum expires the process is
re-enqueued **after** any processes that arrived at that same tick boundary
(ensuring fairness). The process resumes later when it reaches the head of the
queue again.

### Data Structures
- **`Process`** struct: holds PID, arrival, total/remaining burst, first-run
  tick, completion tick, and a `has_run` flag (so first-run is recorded only
  once).
- **`Queue`**: a singly-linked FIFO list. `enqueue` appends to tail;
  `dequeue` pops from head. All nodes are `malloc`'d and freed on cleanup.

### Timeline & Metrics
After simulation, `run[]` (an int array of length = total ticks) is scanned:
- **Gantt output**: two lines, `time:` and `run:`, one column per tick.
- **Per-process**: first run, completion (one past last tick), turnaround
  `(completion − arrival)`, response `(first_run − arrival)`.
- **System**: context switches counted by scanning `run[]` for PID→PID
  changes (idle `-` transitions are ignored); average TAT and RESP.

---

## How to Compile and Run

```bash
make          # builds ./sched
make clean    # removes sched and object files
make valgrind # runs valgrind memory checks on all workloads
make test     # diffs actual output against Workload_Expected/
```

Run manually:

```bash
./sched --policy=FCFS --in=Workload_Samples/W1.txt
./sched --policy=RR   --quantum=2 --in=Workload_Samples/W1.txt
./sched --policy=FCFS --in=Workload_Samples/convoy.txt
./sched --policy=RR   --quantum=2 --in=Workload_Samples/convoy.txt
```

---

## Workload Observations

### Workload 1 – Staggered Arrivals (`W1.txt`)
```
0 0 7   (P0 arrives at 0, burst 7)
1 2 4   (P1 arrives at 2, burst 4)
2 4 1   (P2 arrives at 4, burst 1)
```

**FCFS**: P0 monopolises the CPU for 7 ticks. P1 waits 5 ticks before its
first run (RESP=5); P2 waits 7 ticks (RESP=7). Short jobs arriving later are
penalised heavily. avgTAT=8.0, avgRESP=4.0.

**RR (quantum=1)**: P0 shares the CPU with P1 and P2 as they arrive. P2
finishes after only 1 tick of waiting (RESP=1); P1 gets its first CPU tick at
t=2 (RESP=0). avgTAT=7.3, avgRESP=0.3 — dramatically better response times at
the cost of 9 context switches vs 2 for FCFS.

### Workload 2 – Convoy Effect
```
0 0 12   (P0: long job, arrives first)
1 0 3
2 0 3
3 3 2
```

**FCFS**: The long P0 (burst=12) blocks P1, P2, P3 for 12 ticks — the classic
convoy effect. P1 and P2 each wait 12 ticks (RESP=12 and RESP=15). avgTAT=15.5,
avgRESP=10.5.

**RR (quantum=2)**: P0 is preempted every 2 ticks, allowing P1, P2, and P3 to
interleave. P3 finishes at t=10 (TAT=7) instead of t=20. avgTAT=12.5,
avgRESP=2.75 — RR slices the convoy and dramatically improves fairness and
response time for shorter jobs.

---

## Submission Status

The program compiles cleanly with `make` (no warnings under `-Wall -Wextra`),
produces output exactly matching all provided expected files, and passes
`valgrind --leak-check=full` with zero memory leaks.

---

## Academic Integrity

"I certify that this submission represents entirely my own work."
