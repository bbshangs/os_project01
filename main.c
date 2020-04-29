#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "info.h"

int main(int argc, char* argv[]) 
{
	char sched_rule[256];
	int proc_num;
	scanf("%s", sched_rule);
	scanf("%d", &proc_num);
	Process *proc;
	proc = (Process*)malloc(proc_num * sizeof(Process)); //weird!!!

	for (int i = 0; i < proc_num; i++) {
		scanf("%s%d%d", proc[i].name, &proc[i].ready, &proc[i].exec);
	}

	int rule;
	if (strcmp(sched_rule, "FIFO") == 0) 
		rule = FIFO;
	else if (strcmp(sched_rule, "RR") == 0)
		rule = RR;
	else if (strcmp(sched_rule, "SJF") == 0)
		rule = SJF;
	else if (strcmp(sched_rule, "PSJF") == 0)
		rule = PSJF;
	else {
		fprintf(stderr, "Invalid rule!!!\n");
		exit(0);
	}

	schedule(rule, proc, proc_num);
	return 0;
}
