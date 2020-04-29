#ifndef _INFO_H_
#define _INFO_H_

#include <sys/types.h>

#define FIFO 1
#define RR 2
#define SJF	3
#define PSJF 4

#define UNIT_TIME()	{ volatile unsigned long i; for(i=0;i<1000000UL;i++); } 

typedef struct process {
	char name[32];
	int ready;
	int exec;
	pid_t pid;
} Process;

int schedule(int rule, Process *proc, int proc_num);

int use_cpu(int cpu, pid_t pid);

int wake_proc(pid_t pid);

int block_proc(pid_t pid);

pid_t exec_proc(Process proc);

#endif
