#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include "info.h"

#define GET_NUM 333
#define PRINT_NUM 334

int use_cpu(int cpu, pid_t pid) {
	/*
	if (cpu > sizeof(cpu_set_t)) {
		fprintf(stderr, "Cpu index error.");
		return -1;
	}
	*/
	cpu_set_t my_set;
	CPU_ZERO(&my_set);
	CPU_SET(cpu, &my_set);
		
	if (sched_setaffinity(pid, sizeof(cpu_set_t), &my_set) < 0) 
		perror("affinity");

	return 0;
}

int wake_proc(pid_t pid) {
	struct sched_param p;
	p.sched_priority = 0;
	
	int a;
	if (a = sched_setscheduler(pid, SCHED_OTHER, &p) < 0) {
		perror("wake");
	}
	return a;
}

int block_proc(pid_t pid) {
	struct sched_param p;
	p.sched_priority = 0;
	
	int a;
	if (a = sched_setscheduler(pid, SCHED_IDLE, &p) < 0) {
		perror("block");
	}
	return a;
}

pid_t exec_proc(Process proc) {
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
	}
	else if (pid == 0) { //child process
		//fprintf(stderr, "hi\n");
		long stime = syscall(GET_NUM);
		//fprintf(stderr, "%s start!!!\n", proc.name);
		for (int i = 0; i < proc.exec; i++)
			UNIT_TIME();
		//fprintf(stderr, "%s finish!!!\n", proc.name);
		long etime = syscall(GET_NUM);
		syscall(PRINT_NUM, getpid(), stime, etime);
		exit(0);
	}
	//parent process
	use_cpu(1, pid);
	return pid;
	
}

