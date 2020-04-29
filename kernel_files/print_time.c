//334
#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_print_time(int pid, long stime, long etime) 
{
	static const long big = 1000000000;
	printk("[Project1] %d %ld.%09ld %ld.%09ld\n", pid, stime / big, stime % big, etime / big, etime % big);
}

