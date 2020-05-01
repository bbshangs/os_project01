#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include "info.h"

static int current, running, complete, last;
int queue[100];
int f = 0, b = 0;
int isempty(void) {
	if (f == b)
		return 1;
	else
		return 0;
}
int pop(void) {
	int now = queue[f];
	f = (f + 1) % 100;
	return now;
}
void push(int idx) {
	queue[b] = idx;
	b = (b + 1) % 100;
	return;
}

int compare(const void *a, const void *b) {
	return ((Process *)a)->ready - ((Process *)b)->ready;
}

int next_proc(int rule, Process *proc, int proc_num); 

int schedule(int rule, Process *proc, int proc_num)
{
	//fprintf(stderr, "hi\n");
	qsort(proc, proc_num, sizeof(Process), compare);

	//for (int i = 0; i < proc_num; i++)
	//	fprintf(stderr, "%s\n", proc[i].name);

	for (int i = 0; i < proc_num; i++) {
		proc[i].pid = -1;
	}

	use_cpu(0, getpid());
	wake_proc(getpid());

	current = 0;
	complete = 0;
	running = -1;

	while (1) {
		//fprintf(stderr, "\nTIME %d\n", current);
		if (running != -1 && proc[running].exec == 0) {
			//fprintf(stderr, "%s complete!\n", proc[running].name);

			if (waitpid(proc[running].pid, NULL, 0) < 0)
				perror("waitpid");

			printf("%s %d\n", proc[running].name, proc[running].pid);
			running = -1;
			complete++;
			if (complete == proc_num)
				break;
		}

		for (int i = 0; i < proc_num; i++) {
			if (proc[i].ready == current) {
				push(i);
				proc[i].pid = exec_proc(proc[i]);
				block_proc(proc[i].pid);
			}
		}

		int next = next_proc(rule, proc, proc_num);
		if (next != -1) {
			if (running != next) {
				wake_proc(proc[next].pid);
				block_proc(proc[running].pid);
				running = next;	
				last = current;
			}
		}

		UNIT_TIME();
		if (running != -1)
			proc[running].exec--;
		current++;			
	}
	return 0;
}

int next_proc(int rule, Process *proc, int proc_num) 
{
	int idx = -1;
	if (rule == FIFO) {
		//fprintf(stderr, "In FIFO rule!!!!!!!\n");
		if (running != -1)
			return running;
		for (int i = 0; i < proc_num; i++) {
			if (proc[i].pid == -1 || proc[i].exec == 0)
				continue;
			if (idx == -1 || proc[i].ready < proc[idx].ready)
				idx = i;
		}
	}
	else if (rule == RR) {	
		if (running == -1) {
			if (isempty())
				idx = -1;
			else
				idx = pop();
		}
		else if ((current - last) % 500 == 0) {
			if (isempty())
				idx = running;
			else {
				push(running);
				idx = pop();
			}
		}
		else
			idx = running;
		
		/*	
		if (running == -1) {
			for (int i = 0; i < proc_num; i++) {
				if (proc[i].exec > 0 && proc[i].pid != -1) {
					idx = i;
					break;
				}
			}
		}
		else if ((current - last) % 500 == 0) {
			idx = (running + 1) % proc_num;
			while (proc[idx].exec == 0 || proc[idx].pid == -1)
			   idx = (idx + 1) % proc_num;	
			
		}
		else 
			idx = running;
		*/
	}
	else if (rule == SJF) {
		if (running != -1)
			return running;
		for (int i = 0; i < proc_num; i++) {
			if (proc[i].pid == -1 || proc[i].exec == 0)
				continue;
			if (idx == -1 || proc[i].exec < proc[idx].exec)
				idx = i;
		}

	}
	else if (rule == PSJF) {
		for (int i = 0; i < proc_num; i++) {
			if (proc[i].pid == -1 || proc[i].exec == 0)
				continue;
			if (idx == -1 || proc[i].exec < proc[idx].exec)
				idx = i;
		}
	}
	//fprintf(stderr, "%s's turn\n", proc[idx].name);
	return idx;
}


