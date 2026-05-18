# Student Name:    Dusan Barudzija + Connor Carroll
# Student ID:      3121720
# Submission Date: May 24, 2026
# File Name:       Makefile
# Description:     Build system for the sched CPU scheduler simulator

CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = sched
SRCS   = sched.c

.PHONY: all clean valgrind test

all: $(TARGET)

$(TARGET): $(SRCS) sched.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

# ---- Run all tests against provided expected outputs ----
test: all
	@echo "=== W1 FCFS ===" && \
	 ./$(TARGET) --policy=FCFS --in=Workload_Samples/W1.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W1_FCFS.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W1 RR q1 ===" && \
	 ./$(TARGET) --policy=RR --quantum=1 --in=Workload_Samples/W1.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W1_RR_q1.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W1 RR q2 ===" && \
	 ./$(TARGET) --policy=RR --quantum=2 --in=Workload_Samples/W1.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W1_RR_q2.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W1 RR q4 ===" && \
	 ./$(TARGET) --policy=RR --quantum=4 --in=Workload_Samples/W1.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W1_RR_q4.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W2 FCFS ===" && \
	 ./$(TARGET) --policy=FCFS --in=Workload_Samples/W2.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W2_FCFS.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W2 RR q1 ===" && \
	 ./$(TARGET) --policy=RR --quantum=1 --in=Workload_Samples/W2.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W2_RR_q1.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W2 RR q2 ===" && \
	 ./$(TARGET) --policy=RR --quantum=2 --in=Workload_Samples/W2.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W2_RR_q2.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W2 RR q4 ===" && \
	 ./$(TARGET) --policy=RR --quantum=4 --in=Workload_Samples/W2.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W2_RR_q4.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W3 FCFS ===" && \
	 ./$(TARGET) --policy=FCFS --in=Workload_Samples/W3.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W3_FCFS.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"
	@echo "=== W3 RR q1 ===" && \
	 ./$(TARGET) --policy=RR --quantum=1 --in=Workload_Samples/W3.txt > /tmp/_actual.txt && \
	 diff Workload_Expected/W3_RR_q1.txt /tmp/_actual.txt && echo "PASS" || echo "FAIL"

# ---- Valgrind memory leak check ----
valgrind: all
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) --policy=FCFS --in=Workload_Samples/W1.txt
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) --policy=RR --quantum=2 --in=Workload_Samples/W1.txt
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) --policy=FCFS --in=Workload_Samples/W2.txt
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) --policy=RR --quantum=2 --in=Workload_Samples/W2.txt
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) --policy=FCFS --in=Workload_Samples/W3.txt
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) --policy=RR --quantum=1 --in=Workload_Samples/W3.txt

clean:
	rm -f $(TARGET) sched.o /tmp/_actual.txt
